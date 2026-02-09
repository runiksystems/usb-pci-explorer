/*
 * usb-pci-explorer - A lightweight utility to correlate USB and PCI topology
 * Copyright (C) 2026 Nicolas Dupuis (RunikSystems)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "usb_mapper.h"
#include <cstdio>
#include <memory>
#include <array>
#include <filesystem>
#include <algorithm>
#include <sstream>

namespace fs = std::filesystem;

/**
 * @brief Structure pour fermer proprement le pipe (pclose) sans warning GCC.
 */
struct PipeDeleter {
    void operator()(FILE* p) const {
        if (p) pclose(p);
    }
};

/**
 * @brief Exécute une commande système et capture sa sortie.
 */
std::string UsbMapper::executeCommand(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;

    std::unique_ptr<FILE, PipeDeleter> pipe(popen(cmd.c_str(), "r"));
    if (!pipe) return "";

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

/**
 * @brief Scanne le système pour lier l'USB au PCI sans aucun filtrage.
 * Capture l'intégralité des périphériques vus par lsusb.
 */
std::vector<UsbBusInfo> UsbMapper::scan() {
    std::vector<UsbBusInfo> results;
    const std::string usbPath = "/sys/bus/usb/devices/";

    if (!fs::exists(usbPath)) return results;

    for (const auto& entry : fs::directory_iterator(usbPath)) {
        std::string name = entry.path().filename().string();

        // On cible les bus racines (usb1, usb2...)
        if (name.compare(0, 3, "usb") == 0 && name.find('-') == std::string::npos && std::isdigit(name[3])) {
            UsbBusInfo bus;
            try {
                bus.busNum = std::stoi(name.substr(3));
            } catch (...) { continue; }

            // 1. Détermination de l'adresse PCI
            try {
                fs::path realPath = fs::canonical(entry.path());
                // Le parent du dossier usbX dans /sys/devices/... est le contrôleur PCI
                bus.pciAddr = realPath.parent_path().filename().string();
            } catch (...) {
                bus.pciAddr = "Inconnue";
            }

            // 2. Identification du driver PCI via lspci
            if (bus.pciAddr.find(':') != std::string::npos) {
                std::string cmd = "lspci -nk -s " + bus.pciAddr;
                std::string output = executeCommand(cmd);
                size_t pos = output.find("Kernel driver in use:");
                if (pos != std::string::npos) {
                    bus.driver = output.substr(pos + 22);
                    size_t endLine = bus.driver.find('\n');
                    if (endLine != std::string::npos) bus.driver = bus.driver.substr(0, endLine);
                } else {
                    bus.driver = "non détecté";
                }
            }

            // 3. Récupération EXHAUSTIVE via lsusb
            bus.devices.clear();
            // On force lsusb à lister tous les devices du bus spécifique
            std::string lsusbCmd = "lsusb -s " + std::to_string(bus.busNum) + ":";
            std::string lsusbOut = executeCommand(lsusbCmd);

            std::stringstream ss(lsusbOut);
            std::string line;
            while (std::getline(ss, line)) {
                if (line.empty()) continue;

                // Capture brute de la description
                // On cherche l'ID pour isoler le nom du périphérique
                size_t idPos = line.find("ID ");
                if (idPos != std::string::npos) {
                    // On prend tout ce qui suit l'ID (ex: 1d6b:0003 Linux Foundation...)
                    std::string deviceDetail = line.substr(idPos + 13);
                    if (!deviceDetail.empty()) {
                        bus.devices.push_back(deviceDetail);
                    }
                } else {
                    // Si le format est inhabituel mais que la ligne n'est pas vide
                    bus.devices.push_back(line);
                }
            }

            if (bus.devices.empty()) {
                bus.devices.push_back("Aucun périphérique détecté");
            }

            results.push_back(bus);
        }
    }

    // Tri pour un affichage stable
    std::sort(results.begin(), results.end(), [](const UsbBusInfo& a, const UsbBusInfo& b) {
        return a.busNum < b.busNum;
    });

    return results;
}
