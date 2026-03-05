/*
 *  Copyright (C) 2025 Procion ByProcion@gmail.com
 *
 *  This file is part of WMM-Vortex-support.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the LICENSE file for more details.
 *
 */
#include "vortex.h"
#include "Core.h"
#include "CScrollWindow.h"
#include <QDialog>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>
#include <QRegularExpression>
#include <string>
#include <iostream>

#include "ModManager.h"

void Vortex::main () {
    WMM::APICore::load_new_lang_pack("plugins/Vortex/" + WMM::APICore::get_config(QString("WMM_CONFIG_LANGUAGES")));
    window = new QDialog();
    window->setWindowTitle("Vortex Support module");
    window->setFixedSize(800, 600);
    window->hide();
    QVBoxLayout* mainLay = new QVBoxLayout(window);
    QWidget* mainWidget = new QWidget;
    mainWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    mainLay->addWidget(mainWidget);
    list = new QVBoxLayout();
    addScrollable(mainWidget, list);
    QDir directory(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    directory.cdUp();
    QFileDialog* fileDialog = new QFileDialog(nullptr, "",
                directory.absolutePath() + "/Vortex/downloads/cyberpunk2077/");
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);
    fileDialog->setModal(false);

    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    window->setWindowTitle("VSM — select vortex downloads folder");

    connect(fileDialog, &QFileDialog::fileSelected, [this, fileDialog](QString path){
        this->dir = std::move(path);
        fileDialog->reject();
    });
    connect(fileDialog, &QFileDialog::rejected, [fileDialog] { fileDialog->deleteLater(); });
    fileDialog->setAttribute(Qt::WA_DeleteOnClose);
    fileDialog->exec();
    window->setWindowTitle("Vortex Support module");
    window->show();

    list->addWidget(new QLabel(WMM::APICore::tr("VORTEX_LABEL_CHOOSE_MODS")));
    get_ir();
    gen_front();
    ButtonBox* buttonBox = new ButtonBox();
    mainLay->addWidget(buttonBox);
    connect(buttonBox, &ButtonBox::cancel_clicked, this, &Vortex::destruct);
    connect(buttonBox, &ButtonBox::apply_clicked, this, &Vortex::installing);
}

void Vortex::destruct() {
    delete window;
    for (auto* entry : data)
        delete entry;
    data.clear();
    list = nullptr;
}

void Vortex::installing() {
    QString path;
    if (WMM::APICore::get_config("WMM_CONFIG_GAME") == "None") {
        error_dialog(WMM::APICore::tr("LANG_LABEL_R37"));
        return;
    }

    for (auto entry : data) {
        if (entry->target) {
            if (WMM::APIModManager::exists(entry->modId, entry->modVersion)) {
                error_dialog(WMM::APICore::tr("LANG_LABEL_MOD_EXISTS") + " Mod name:" + entry->stringName);
                continue;
            }
            path = dir + "/" + entry->stringName;
            std::cout << "Loaded: " << path.toStdString() << std::endl;
            WMM::APIModManager::load(path);
        }
    }
    error_dialog(WMM::APICore::tr("VORTEX_LABEL_IMPORT_SUCCES"));
    destruct();
}


void Vortex::get_ir() {
    QRegularExpression regex(R"(^(.+?)-(\d+)-(\d+(?:-\d+)*)-(\d+)(?:\((\d+)\))?.(\w+)$)");
    QRegularExpressionMatch match;
    for (QString& entry : QDir(dir).entryList(QStringList("*"), QDir::Files)) {
        match = regex.match(entry);
        if (match.isValid()) {
            data.emplace_back(new ir(match.captured(2).toULongLong(), match.captured(1), match.captured(3), entry));
        }
        else {
            std::cerr << regex.errorString().toStdString() << std::endl;
        }
    }
    std::sort(data.begin(), data.end(), [](const ir* a, const ir* b){
        return (a->modId > b->modId);
    });
}


void Vortex::gen_front() {
    id* last = nullptr;
    for (auto* entry : data) {
        if (last && entry->modId == last->ptr->modId) {
            last->list->addWidget(new mod(*entry));
        }
        else {
            last = new id(*entry);
            list->addWidget(last);
        }
    }
}

