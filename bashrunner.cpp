#include "bashrunner.h"
#include "config.h"
#include <QStandardPaths>
#include <QDir>

BashRunner::BashRunner(QObject *parent, const KPluginMetaData &data)
    : AbstractRunner(parent, data)
{
    setTriggerWords(Config::items);
}

void BashRunner::init()
{

    QString folderPath { QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) };
    folderPath.append("/bashrunner_data/");

    QDir dir {folderPath};
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    reloadConfiguration();
    connect(this, &AbstractRunner::prepare, this, []() {
        // Initialize data for the match session. This gets called from the main thread
    });
    connect(this, &AbstractRunner::teardown, this, []() {
        // Cleanup data from the match session. This gets called from the main thread
    });
}

void BashRunner::match(KRunner::RunnerContext &context) {
    QString query = context.query();
    if (query == QLatin1Char('.') || query == QLatin1String("..")) {
        return;
    }
    // This should not get in the way of the Help-Runner which gets triggered by queries starting with '?'
    if (query.startsWith(QLatin1Char('?'))) {
        return;
    }

    if (!m_triggerWord.isEmpty()) {
        if (!query.startsWith(m_triggerWord)) {
            return;
        }

        query.remove(0, m_triggerWord.length());
    }

    if (query.length() > 2) {
        query.prepend(QLatin1Char('*')).append(QLatin1Char('*'));
    }
}


K_PLUGIN_CLASS_WITH_JSON(BashRunner, "bashrunner.json")
#include "bashrunner.moc"