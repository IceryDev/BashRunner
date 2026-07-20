#pragma once
#include <QString>

namespace Config {
    //inline const QStringList items {"/run"};
    inline const QString defaultPrefix {QLatin1String("/sh ")};
    inline const QString argsPlaceholder {QLatin1String(" params(1): %1")};
}
