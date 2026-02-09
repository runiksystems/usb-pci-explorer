/*
 * usb-pci-explorer - A lightweight utility to correlate USB and PCI topology
 * Copyright (C) 2026 Nicolas Dupuis (RunikSystems)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef USB_MAPPER_H
#define USB_MAPPER_H

#include <string>
#include <vector>

/**
 * @brief Structure contenant les informations brutes d'un bus USB.
 * Utilise uniquement les types standards (STL) pour rester indépendant de Qt.
 */
struct UsbBusInfo {
    int busNum;
    std::string pciAddr;
    std::string driver;
    std::vector<std::string> devices;
};

/**
 * @brief Classe responsable de l'exploration du système Linux (/sys).
 * Cette classe est le "moteur" du programme.
 */
class UsbMapper {
public:
    UsbMapper() = default;

    /**
     * @brief Scanne le système pour mapper les bus USB aux contrôleurs PCI.
     * @return Un vecteur de structures UsbBusInfo.
     */
    std::vector<UsbBusInfo> scan();

private:
    /**
     * @brief Exécute une commande shell (ex: lspci, lsusb) et récupère sa sortie.
     * @param cmd La commande à exécuter.
     * @return La sortie standard de la commande sous forme de string.
     */
    std::string executeCommand(const std::string& cmd);
};

#endif // USB_MAPPER_H
