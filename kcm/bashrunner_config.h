#pragma once
#include <KQuickConfigModule>

class BashRunnerConfig : public KQuickConfigModule
{
    Q_OBJECT
    public:
    BashRunnerConfig(QObject *parent, const KPluginMetaData &data);
};