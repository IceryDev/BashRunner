import QtQuick
import QtQuick.Controls as Controls

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils

KCMUtils.SimpleKCM {
    Kirigami.FormLayout {
        anchors.fill: parent

        Controls.TextField {
            Kirigami.FormData.label: "Trigger Prefix:"
            text: Config.trigger
        }
        Controls.TextField {
            Kirigami.FormData.label: "Target Folder:"
        }
    }
}