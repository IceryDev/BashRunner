#pragma once
#include <KQuickManagedConfigModule>
#include "config.h"

class BashRunnerConfig : public KQuickManagedConfigModule
{
    Q_OBJECT
    Q_PROPERTY(Config *config READ config CONSTANT)
public:
    BashRunnerConfig(QObject *parent, const KPluginMetaData &data);
    Config *config() const;
    void save() override;
private:
    Config *m_config;
};