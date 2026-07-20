#pragma once
#include <QString>

namespace Config {
    //inline const QStringList items {"/run"};
    inline const QString defaultPrefix {QLatin1String("/sh ")};
    inline const QString argsPlaceholder {QLatin1String("<args>")};
    inline const QString argsPrefix {QLatin1String(" params(%1): %2")};
    inline const QString paramsDefinition {QLatin1String("#params: ")};
}
