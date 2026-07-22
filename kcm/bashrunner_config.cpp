#include "bashrunner_config.h"

#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(BashRunnerConfig, "kcm_bashrunner.json")

BashRunnerConfig::BashRunnerConfig(QObject *parent, const KPluginMetaData &data)
    : KQuickConfigModule(parent, data)
{
    setButtons(Help | Apply | Default);
}

#include "bashrunner_config.moc"
