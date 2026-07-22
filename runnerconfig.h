#pragma once
#include <QString>
#include <QStandardPaths>

namespace RunnerConfig {
    //inline const QStringList items {"/run"};
    inline const QString defaultPrefix {QLatin1String("/sh")};
    inline const QString argsPlaceholder {QLatin1String("<args>")};
    inline const QString argsPrefix {QLatin1String(" =%1")};
    inline const QString paramsDefinition {QLatin1String("#params: ")};
    inline const QString emptyParams {QLatin1String("#noparam")};
    inline const QString dirFlag {QLatin1String("-d")};
    inline const QString defaultDir {QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation).append(QLatin1String("/bashrunner_data/"))};
}
