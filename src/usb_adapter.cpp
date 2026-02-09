/*
 * usb-pci-explorer - A lightweight utility to correlate USB and PCI topology
 * Copyright (C) 2026 Nicolas Dupuis (RunikSystems)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "usb_adapter.h"
#include "usb_mapper.h"

/**
 * @brief Constructeur de l'adaptateur.
 */
UsbAdapter::UsbAdapter(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief Logique de conversion et de formatage.
 * C'est ici que la décision du formatage (liste verticale) est prise.
 */
QList<UsbDataQt> UsbAdapter::getFormattedData() {
    UsbMapper coreMapper;

    // Récupération des données brutes du backend (STL)
    std::vector<UsbBusInfo> rawData = coreMapper.scan();

    QList<UsbDataQt> results;

    for (const auto& item : rawData) {
        UsbDataQt qtItem;

        // Conversion des types simples
        qtItem.bus = item.busNum;
        qtItem.pci = QString::fromStdString(item.pciAddr);
        qtItem.driver = QString::fromStdString(item.driver);

        // Transformation de la liste de périphériques
        QStringList devList;
        for(const std::string& deviceName : item.devices) {
            devList << QString::fromStdString(deviceName);
        }

        // FORMATAGE : On utilise "\n" pour l'affichage en liste dans la cellule
        qtItem.devices = devList.join("\n");

        results.append(qtItem);
    }

    return results;
}
