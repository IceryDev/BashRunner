#include "bashrunner.h"
#include "config.h"
#include <QStandardPaths>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KIO/OpenUrlJob>
#include <KNotificationJobUiDelegate>
#include <QMimeDatabase>
#include <QIcon>
#include <QDir>

BashRunner::BashRunner(QObject *parent, const KPluginMetaData &data)
    : AbstractRunner(parent, data)
{
    //setTriggerWords(Config::items);
}

void BashRunner::init()
{

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

    // This should not get in the way of the Help-Runner which gets triggered by queries starting with '?'
    if (query.startsWith(QLatin1Char('?'))) {
        return;
    }

    bool matched {false};
    for (QString q : m_triggerWords) {
        if (query.startsWith(q)) {
            query.remove(0, q.length());
            matched = true;
        }
    }
    if (!matched) return;

    if (query.length() > 2) {
        query.prepend(QLatin1Char('*')).append(QLatin1Char('*'));
    }

    QDir dir {m_path};
    QList<KRunner::QueryMatch> matches;

    qWarning() << query << m_path;
    for (const QString &file : dir.entryList(QStringList(query))) {
        qWarning() << "match called, query:" << context.query();
        const QString path = dir.absoluteFilePath(file);
        if (!path.startsWith(m_path) || !path.endsWith(QLatin1String(".sh"), Qt::CaseInsensitive)) {
            continue;
        }


        if (!context.isValid()) {
            return;
        }


        KRunner::QueryMatch match(this);
        match.setText(i18n("Run %1", path.split(QLatin1Char('/')).last()));
        match.setData(path);
        match.setId(path);
        //Change both of the literals below for any change
        if (QIcon::hasThemeIcon(QStringLiteral("utilities-terminal"))) {
            match.setIconName(QStringLiteral("utilities-terminal"));
        } else {
            QMimeDatabase mimeDb {};
            QIcon icon = QIcon::fromTheme(mimeDb.mimeTypeForFile(path).iconName());
            match.setIcon(icon);
        }


        if (file.compare(query, Qt::CaseInsensitive)) {
            match.setRelevance(1.0);
        } else {
            match.setRelevance(0.8);
        }
        match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);

        matches.append(match);
    }

    context.addMatches(matches);
}

void BashRunner::run(const KRunner::RunnerContext & /*context*/, const KRunner::QueryMatch &match)
{
    // KIO::OpenUrlJob autodeletes itself, so we can just create it and forget it!
    auto *job = new KIO::OpenUrlJob(QUrl::fromLocalFile(match.data().toString()));
    job->setUiDelegate(new KNotificationJobUiDelegate(KJobUiDelegate::AutoErrorHandlingEnabled));
    job->setRunExecutables(false);
    job->start();
}

void BashRunner::reloadConfiguration()
{
    KConfigGroup c = config();
    m_triggerWords = c.readEntry("trigger", QStringList());

    for (qsizetype i = 0; i < m_triggerWords.length();) {
        auto& word = m_triggerWords[i];
        if (!word.isEmpty()) {
            word.append(QLatin1Char(' '));
            ++i;
        }
        else {
            m_triggerWords.remove(i);
        }
    }

    if (m_triggerWords.isEmpty()) m_triggerWords.append(Config::defaultPrefix);

    m_path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    m_path.append(QLatin1String("/bashrunner_data/"));

    QDir dir {m_path};
    if (!dir.exists()) {
        dir.mkpath(QLatin1String("."));
    }
    
    QList<KRunner::RunnerSyntax> syntaxes;
    KRunner::RunnerSyntax syntax(QStringLiteral("%1 :q:").arg(m_triggerWords.constFirst()), i18n("Run script :q: in the %1 folder", m_path));
    syntaxes.append(syntax);
    setSyntaxes(syntaxes);

    qWarning() << "I came this far!";

    setTriggerWords(m_triggerWords);
}

K_PLUGIN_CLASS_WITH_JSON(BashRunner, "bashrunner.json")
#include "bashrunner.moc"