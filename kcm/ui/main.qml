import QtQuick
import QtQuick.Controls as Controls

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils

import QtQuick.Controls 2.12 as QQC2

KCMUtils.SimpleKCM {
    Kirigami.FormLayout {
        anchors.fill: parent

        Kirigami.FormData.label: i18n("Scripts folder:")
        Controls.TextField {
            Kirigami.FormData.label: "Trigger Prefix:"
            text: kcm.config.trigger
            onTextChanged: kcm.config.trigger = text
        }
        Controls.TextField {
            Kirigami.FormData.label: "Target Folder:"
        }
    }
}