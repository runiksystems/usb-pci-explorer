/*
 * usb-pci-explorer - A lightweight utility to correlate USB and PCI topology
 * Copyright (C) 2026 Nicolas Dupuis (RunikSystems)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#ifndef USB_ADAPTER_H
#define USB_ADAPTER_H

#include <QObject>
#include <QStringList>
#include <QList>

/**
 * @brief Structure de données formatée pour l'affichage dans l'interface Qt.
 */
struct UsbDataQt {
    int bus;
    QString pci;
    QString driver;
    QString devices;
};

/**
 * @brief La classe UsbAdapter sert de pont entre le backend C++ pur et l'UI Qt.
 * Elle transforme les types STL (std::string, std::vector) en types Qt.
 */
class UsbAdapter : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Constructeur avec gestion du parent pour la mémoire Qt.
     * @param parent Le widget ou l'objet parent (optionnel).
     */
    explicit UsbAdapter(QObject *parent = nullptr);

    /**
     * @brief Scanne le système et convertit les résultats pour les widgets Qt.
     * @return Une liste d'objets UsbDataQt prêts à être affichés dans un tableau.
     */
    QList<UsbDataQt> getFormattedData();
};

#endif // USB_ADAPTER_H
