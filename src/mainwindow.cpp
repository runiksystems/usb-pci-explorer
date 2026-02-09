/*
 * usb-pci-explorer - A lightweight utility to correlate USB and PCI topology
 * Copyright (C) 2026 Nicolas Dupuis (RunikSystems)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "usb_adapter.h"
#include <QHeaderView>
#include <QTimer>
#include <QPalette>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 1. CONFIGURATION DU TABLEAU (SANS CSS)
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels({"Bus", "Adresse PCI", "Pilote", "Périphériques"});

    // CAUSE DU PROBLÈME : On force la désactivation de l'alternance
    ui->tableWidget->setAlternatingRowColors(false);

    // On réinitialise la palette du widget pour s'assurer que 'Base' et 'AlternateBase'
    // sont identiques, ce qui neutralise le bug visuel du thème système.
    QPalette p = ui->tableWidget->palette();
    p.setColor(QPalette::AlternateBase, p.color(QPalette::Base));
    ui->tableWidget->setPalette(p);

    // Grille native
    ui->tableWidget->setShowGrid(true);

    // Configuration de l'affichage
    ui->tableWidget->setWordWrap(true);
    ui->tableWidget->setTextElideMode(Qt::ElideNone);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);

    // On nettoie toute feuille de style qui pourrait rester en mémoire
    ui->tableWidget->setStyleSheet("");

    // 2. CONNEXIONS
    connect(ui->btnScan, &QPushButton::clicked, this, &MainWindow::onRefresh);

    // Lancement automatique
    QTimer::singleShot(150, this, &MainWindow::onRefresh);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onRefresh()
{
    ui->btnScan->setEnabled(false);
    ui->btnScan->setText("Analyse en cours...");

    ui->tableWidget->setUpdatesEnabled(false);
    ui->tableWidget->setRowCount(0);

    UsbAdapter adapter;
    auto data = adapter.getFormattedData();

    const Qt::Alignment myAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    for (const auto& item : std::as_const(data)) {
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *busItem = new QTableWidgetItem(QString::number(item.bus));
        QTableWidgetItem *pciItem = new QTableWidgetItem(item.pci);
        QTableWidgetItem *driverItem = new QTableWidgetItem(item.driver);
        QTableWidgetItem *deviceItem = new QTableWidgetItem(item.devices);

        busItem->setTextAlignment(myAlignment);
        pciItem->setTextAlignment(myAlignment);
        driverItem->setTextAlignment(myAlignment);
        deviceItem->setTextAlignment(myAlignment);

        ui->tableWidget->setItem(row, 0, busItem);
        ui->tableWidget->setItem(row, 1, pciItem);
        ui->tableWidget->setItem(row, 2, driverItem);
        ui->tableWidget->setItem(row, 3, deviceItem);
    }

    ui->tableWidget->resizeRowsToContents();

    // Padding vertical natif pour aérer
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHeight(i, ui->tableWidget->rowHeight(i) + 10);
    }

    ui->tableWidget->setUpdatesEnabled(true);
    ui->btnScan->setEnabled(true);
    ui->btnScan->setText("Lancer l'analyse du matériel");
}
