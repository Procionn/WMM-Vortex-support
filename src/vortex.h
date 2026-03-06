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
#ifndef VORTEX_H
#define VORTEX_H

#include "PluginInterface.h"
#include <QObject>
#include <vector>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCore/qglobal.h>
#include <QDialog>

#if defined(VORTEX_LIBRARY)
    #define VORTEX_EXPORT Q_DECL_EXPORT
#else
    #define VORTEX_EXPORT Q_DECL_IMPORT
#endif

struct ir {
    const uint64_t modId;
    const QString modName, modVersion, stringName;
    bool target = true;
    ir (const uint64_t& id, const QString& name, const QString& version, const QString& string)
        : modId(id), modName(name), modVersion(version), stringName(string) {}
};


class id : public QFrame
{
public:
    QVBoxLayout* list;
    ir* ptr;

    id(ir& ref);
};
class mod : public QLabel
{
    Q_OBJECT

public:
    ir* ptr;

    mod(ir& ref);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void recolor();
};



class VORTEX_EXPORT Vortex : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)
    QVBoxLayout* list;
    std::vector<ir*> data;
    QDialog* window;
    QString dir;

    void get_ir();
    void gen_front();

public:
    Vortex() = default;
    virtual ~Vortex();
    virtual void main() override;
    virtual QString name() override ;
    virtual QString description() override;

protected slots:
    void installing();
    void destruct();
};


class QPushButton;
class QHBoxLayout;
class ButtonBox : public QFrame {
    Q_OBJECT
    QPushButton* cansel;
    QPushButton* apply;
    QHBoxLayout* layout;

public:
    ButtonBox();

signals:
    void apply_clicked();
    void cancel_clicked();
};


class ERRORdialog : public QDialog {
    Q_OBJECT
public:
    ERRORdialog(const QString& str);
};

inline void error_dialog(const QString& str) {
    ERRORdialog dialog(str);
    dialog.exec();
}

#endif // VORTEX_H
