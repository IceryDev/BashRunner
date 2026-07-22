#include "bashrunner_config.h"
#include <KPluginFactory>

K_PLUGIN_CLASS_WITH_JSON(BashRunnerConfig, "kcm_bashrunner.json")

BashRunnerConfig::BashRunnerConfig(QObject *parent, const KPluginMetaData &data)
    : KQuickManagedConfigModule(parent, data), m_config(new Config(this))
{
    qmlRegisterAnonymousType<Config>("org.kde.kcm_bashrunner.private", 1);
}

Config *BashRunnerConfig::config() const { return m_config; }

void BashRunnerConfig::save()
{
    KQuickManagedConfigModule::save();
    KConfigGroup group(KSharedConfig::openConfig(QStringLiteral("krunnerrc")), QStringLiteral("Runners"));
    group.writeEntry("runner_bashrunner", true, KConfig::Notify);
    group.sync();
}

#include "bashrunner_config.moc"
