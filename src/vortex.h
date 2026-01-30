#ifndef VORTEX_H
#define VORTEX_H

#include "PluginInterface.h"
#include <QObject>
#include <vector>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCore/qglobal.h>

#if defined(VORTEX_LIBRARY)
    #define VORTEX_EXPORT Q_DECL_EXPORT
#else
    #define VORTEX_EXPORT Q_DECL_IMPORT
#endif

struct ir {
    const uint64_t modId;
    const QString modName, modVersion;
    ir (const uint64_t& id, const QString& name, const QString& version)
        : modId(id), modName(name), modVersion(version) {}
};


class id : public QFrame
{
public:
    QVBoxLayout* list;
    const ir* ptr;

    id(const ir& ref);
};
class mod : public QLabel
{
public:
    const ir* ptr;

    mod(const ir& ref);
};



class VORTEX_EXPORT Vortex : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID PluginInterface_iid)
    Q_INTERFACES(PluginInterface)
    QVBoxLayout* list;

    std::vector<ir*> get_ir(const QString& dir);
    void gen_front(std::vector<ir*>);

public:
    Vortex() = default;
    virtual ~Vortex() = default;
    virtual void plugin_main();
    virtual QString name() {
        return "Vortex Support";
    }
};

#endif // VORTEX_H
