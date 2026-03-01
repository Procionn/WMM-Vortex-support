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
#include <QMouseEvent>
#include <QPushButton>


id::id (ir& ref) : ptr(&ref) {
    QHBoxLayout* main = new QHBoxLayout(this);
    main->addWidget(new QLabel(QString::number(ref.modId)));
    list = new QVBoxLayout();
    main->addLayout(list);
    list->addWidget(new mod(ref));
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}

mod::mod (ir& ref) : QLabel(ref.modName + " - " + ref.modVersion), ptr(&ref) {
    recolor();
}

void mod::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        ptr->target = !(ptr->target);
        recolor();
    }
}

void mod::recolor() {
    if (ptr->target)
        setStyleSheet("background-color: #272a30;");
    else
        setStyleSheet("background-color: #cc5656;");
}



ButtonBox::ButtonBox() {
    layout = new QHBoxLayout();
    setLayout(layout);
    apply = new QPushButton("Apply");
    cansel = new QPushButton("Cancel");
    layout->addWidget(apply);
    layout->addWidget(cansel);
    connect(apply, &QPushButton::clicked, [this]{emit apply_clicked();});
    connect(cansel, &QPushButton::clicked, [this]{emit cancel_clicked();});
}


ERRORdialog::ERRORdialog (const QString& text) {
    resize(600, 400);
    QLabel* label = new QLabel(text, this);
    connect(this, &QDialog::rejected, [this]{deleteLater();});
}
