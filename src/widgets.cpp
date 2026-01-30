#include "vortex.h"


id::id (const ir& ref) : ptr(&ref) {
    QHBoxLayout* main = new QHBoxLayout(this);
    main->addWidget(new QLabel(QString::number(ref.modId)));
    list = new QVBoxLayout();
    main->addLayout(list);
    list->addWidget(new mod(ref));
    setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
}

mod::mod (const ir& ref) : QLabel(ref.modName), ptr(&ref) {

}
