#pragma once

#include <KRunner/AbstractRunner>
#include <KIO/CommandLauncherJob>
#include <KIO/OpenUrlJob>
#include <KIO/JobUiDelegateFactory>
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
    QString m_triggerWord;

    static qsizetype getParamCount(const QString& s) {
        return s.count(QLatin1Char('<'));
    }

    static QStringList getParams(const QString& s) {
        //Match non-quoted args or quoted args with escaped characters.
        const QRegularExpression reg {QStringLiteral(R"(\"((?:[^\"\\]|\\.)*)\"|(\S+))")};
        QRegularExpressionMatchIterator regMatch = reg.globalMatch(s);

        QStringList params {};
        params.clear();
        while (regMatch.hasNext()) {
            if (QRegularExpressionMatch m = regMatch.next(); m.hasCaptured(1)) {
                params.append(m.captured(1));
            } else {
                params.append(m.captured(2));
            }
        }
        return params;
    }

    void runBash(const QString& path, const std::optional<QStringList>& params) {
        auto* job = new KIO::CommandLauncherJob(path, params.value_or(QStringList{}));
        job->setUiDelegate(new KNotificationJobUiDelegate(KJobUiDelegate::AutoErrorHandlingEnabled));
        job->start();
        m_paramStartIndex = -1;
    }

    void openExplorer() const {
        auto *job = new KIO::OpenUrlJob(QUrl::fromLocalFile(m_path));
        job->setUiDelegate(KIO::createDefaultJobUiDelegate());
        job->start();
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