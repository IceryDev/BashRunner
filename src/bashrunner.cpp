#include "bashrunner.h"
#include "../config.h"
#include <QStandardPaths>
#include <KLocalizedString>
#include <KConfigGroup>
#include <KIO/OpenUrlJob>
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
    connect(this, &AbstractRunner::prepare, this, []() {});
    connect(this, &AbstractRunner::teardown, this, []() {});
}

void BashRunner::match(KRunner::RunnerContext &context) {
    QString query = context.query();

    // Avoid collision with helper runner
    if (query.startsWith(QLatin1Char('?'))) {
        return;
    }

    QString usedPrefix {};
    if (query.startsWith(m_triggerWord)) {
        if (usedPrefix.isEmpty() || usedPrefix.length() < m_triggerWord.length()) {
            usedPrefix = m_triggerWord;
        }
    }
    if (usedPrefix.isEmpty()) return;

    //Check whether the user is still changing parameters or started a new search (erasing the param block or changing the file name)
    if (m_paramStartIndex != -1 && (query.length() < m_paramStartIndex || !query.startsWith(m_userInput))) m_paramStartIndex = -1;

    QList<KRunner::QueryMatch> matches;
    query.remove(0, usedPrefix.length());

    if (query == Config::dirFlag) {
        KRunner::QueryMatch match(this);
        match.setData(QStringList({usedPrefix}));
        match.setIconName(QStringLiteral("user-home"));
        match.setText(QStringLiteral("Open script folder for Bash Runner."));
        match.setId(m_path);
        matches.append(match);
        context.addMatches(matches);
        return;
    }

    if (m_paramStartIndex == -1) {
        if (query.length() > 1) {
            query.prepend(QLatin1Char('*')).append(QLatin1Char('*'));
        }

        QDir dir {m_path};

        //qWarning() << query << m_path;
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
            addIcon(match, path);

            if (file.compare(query, Qt::CaseInsensitive)) {
                match.setRelevance(1.0);
            } else {
                match.setRelevance(0.8);
            }
            match.setCategoryRelevance(KRunner::QueryMatch::CategoryRelevance::Highest);

            matches.append(match);
        }
    }
    else {
        KRunner::QueryMatch match(this);
        //Send the parameters to ::run and it will process them
        match.setText(i18n("Enter parameters for %1 : accepts %2", m_selectedPath.split(QLatin1Char('/')).last(),
                ((m_params == Config::argsPlaceholder) ? QLatin1String("?") : QString::number(getParams(m_params).length()))));
        match.setData(QStringList({usedPrefix, m_selectedPath, query.slice(m_paramStartIndex-usedPrefix.length())}));
        match.setId(m_selectedPath);
        addIcon(match, m_selectedPath);

        matches.append(match);
    }

    context.addMatches(matches);
}

void BashRunner::run(const KRunner::RunnerContext & context, const KRunner::QueryMatch &match)
{
    if (!match.data().isValid() || match.data().isNull()) return;

    QStringList args {match.data().toStringList()};
    //If there are only two parameters it means this is step 1 or takes no parameters.
    if (args.length() <= 1) {

        openExplorer();
    }
    else if (args.length() == 2) {

        QString command {args.at(0)};
        command.append(args.at(1).split(QLatin1Char('/')).last());
        m_selectedPath = args.at(1);
        m_userInput = command;

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

            if (line.startsWith(Config::emptyParams)) {
                runBash(args.at(1), std::nullopt);
                return;
            }

            if (!line.trimmed().isEmpty() && !line.startsWith(QLatin1Char('#'))) break;
        } 

        command.append(Config::argsPrefix.arg(m_params));

        m_paramStartIndex = static_cast<int>(command.length()-m_params.length());
        context.requestQueryStringUpdate(command, m_paramStartIndex);
    }
    else {

        if (QStringList params {getParams(args.at(2))}; !params.isEmpty())
            runBash(args.at(1), params);
        else runBash(args.at(1), std::nullopt);
    }
}

void BashRunner::reloadConfiguration()
{
    KConfigGroup c = config();
    m_triggerWord = c.readEntry("trigger", QString());

    if (m_triggerWord.trimmed().isEmpty()) m_triggerWord = Config::defaultPrefix;

    m_path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
    m_path.append(QLatin1String("/bashrunner_data/"));

    QDir dir {m_path};
    if (!dir.exists()) {
        dir.mkpath(QLatin1String("."));
    }
    
    QList<KRunner::RunnerSyntax> syntaxes;
    KRunner::RunnerSyntax syntax(QStringLiteral("%1 :q:").arg(m_triggerWord), i18n("Run script :q: in the %1 folder", m_path));
    syntaxes.append(syntax);
    setSyntaxes(syntaxes);

    setTriggerWords(QStringList {m_triggerWord});
}

K_PLUGIN_CLASS_WITH_JSON(BashRunner, "bashrunner.json")
#include "bashrunner.moc"