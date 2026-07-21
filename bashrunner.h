#pragma once

#include <KRunner/AbstractRunner>
#include <KIO/CommandLauncherJob>
#include <KNotificationJobUiDelegate>
#include <QIcon>
#include <QMimeDatabase>

class BashRunner : public KRunner::AbstractRunner
{
    Q_OBJECT

public:
    BashRunner(QObject *parent, const KPluginMetaData &data);

    void match(KRunner::RunnerContext &context) override;
    void run(const KRunner::RunnerContext &context, const KRunner::QueryMatch &match) override;
    void reloadConfiguration() override;

protected:
    void init() override;

private:
    int m_paramStartIndex {-1};
    QString m_selectedPath; //Selected script's file path
    QString m_params; //Parameter structure of the script
    QString m_path; //Working dir
    QString m_userInput;
    QStringList m_triggerWords;

    static qsizetype getParamCount(const QString& s) {
        return s.count(QLatin1Char('<'));
    }

    void runBash(const QString& path, const std::optional<QStringList>& params) {
        auto* job = new KIO::CommandLauncherJob(path, params.value_or(QStringList{}));
        job->setUiDelegate(new KNotificationJobUiDelegate(KJobUiDelegate::AutoErrorHandlingEnabled));
        job->start();
        m_paramStartIndex = -1;
    }

    static void addIcon(KRunner::QueryMatch& match, const QString& path) {
        //Change both of the literals below for any change
        if (QIcon::hasThemeIcon(QStringLiteral("utilities-terminal"))) {
            match.setIconName(QStringLiteral("utilities-terminal"));
        } else {
            const QMimeDatabase mimeDb {};
            const QIcon icon = QIcon::fromTheme(mimeDb.mimeTypeForFile(path).iconName());
            match.setIcon(icon);
        }
    }

};