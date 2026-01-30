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


void Vortex::plugin_main () {
    QDialog* window = new QDialog();
    window->setWindowTitle("Vortex Support module");
    window->show();
    list = new QVBoxLayout();
    addScrollable(window, list);
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dir.cdUp();
    QString buffer;
    QFileDialog* fileDialog = new QFileDialog(nullptr, "",
                dir.absolutePath() + "/Vortex/downloads/cyberpunk2077/");
    fileDialog->setOption(QFileDialog::DontUseNativeDialog, true);
    fileDialog->setModal(false);
    list->addWidget(fileDialog);

    fileDialog->setFileMode(QFileDialog::Directory);
    fileDialog->setOption(QFileDialog::ShowDirsOnly, true);
    window->setWindowTitle("VSM — select vortex downloads folder");

    connect(fileDialog, &QFileDialog::fileSelected, [&buffer, fileDialog](QString path){
        buffer = path;
        fileDialog->reject();
    });
    fileDialog->exec();
    window->setWindowTitle("Vortex Support module");

    gen_front(get_ir(buffer));
}


std::vector<ir*> Vortex::get_ir(const QString& dir) {
    std::vector<ir*> list;
    QRegularExpression regex(R"(^(.+?)-(\d+)-(\d+(?:-\d+)*)-(\d+)(?:\((\d+)\))?.(\w+)$)");
    QRegularExpressionMatch match;
    for (QString& entry : QDir(dir).entryList(QStringList("*"), QDir::Files)) {
        match = regex.match(entry);
        if (match.isValid()) {
            list.emplace_back(new ir(match.captured(2).toULongLong(), match.captured(1), match.captured(3)));
        }
        else {
            std::cerr << regex.errorString().toStdString() << std::endl;
        }
    }
    std::sort(list.begin(), list.end(), [](const ir* a, const ir* b){
        return (a->modId > b->modId);
    });
    return list;
}


void Vortex::gen_front(std::vector<ir*> data) {
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

