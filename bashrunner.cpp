#include "bashrunner.h"
#include "config.h"
#include <QStandardPaths>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KIO/CommandLauncherJob>
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

    QString usedPrefix {};
    bool matched {false};
    for (QString q : m_triggerWords) {
        if (query.startsWith(q)) {
            query.remove(0, q.length());
            usedPrefix = q;
            matched = true;
        }
    }
    if (!matched) return;

    if (query.length() > 1) {
        query.prepend(QLatin1Char('*')).append(QLatin1Char('*'));
    }

    QDir dir {m_path};
    QList<KRunner::QueryMatch> matches;

    qWarning() << query << m_path;
    for (const QString &file : dir.entryList(QStringList(query))) {
        const QString path = dir.absoluteFilePath(file);
        if (!path.startsWith(m_path) || !path.endsWith(QLatin1String(".sh"), Qt::CaseInsensitive)) {
            continue;
        }


        if (!context.isValid()) {
            return;
        }


        KRunner::QueryMatch match(this);
        match.setText(i18n("Run %1", path.split(QLatin1Char('/')).last()));
        match.setData(QStringList({usedPrefix, path}));
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

void BashRunner::run(const KRunner::RunnerContext & context, const KRunner::QueryMatch &match)
{
    if (!match.data().isValid() || match.data().isNull()) return;

    QStringList args {match.data().toStringList()};
    //If there are only two parameters it means this is step 1.
    if (args.length() <= 2) {
        QString command {args.at(0)};
        command.append(args.at(1).split(QLatin1Char('/')).last());

        QFile file(args.at(1));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        m_params = Config::argsPlaceholder;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith(Config::paramsDefinition)) {
                m_params = line.remove(0, Config::paramsDefinition.length());
                break;
            }

            if (!line.trimmed().isEmpty() && !line.startsWith(QLatin1Char('#'))) break;
        } 

        command.append(Config::argsPrefix.arg(QString::number(getParamCount(m_params)), m_params));

        context.requestQueryStringUpdate(command, static_cast<int>(command.length()-m_params.length()));
    }
    else {
        const QStringList params {args.at(3)};
        auto *job = new KIO::CommandLauncherJob(args.at(2), params);
        job->setUiDelegate(new KNotificationJobUiDelegate(KJobUiDelegate::AutoErrorHandlingEnabled));
        job->start();
    }
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