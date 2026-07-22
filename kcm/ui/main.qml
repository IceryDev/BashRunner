import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts as Layouts
import QtQuick.Dialogs as Dialogs

import org.kde.kirigami as Kirigami
import org.kde.kcmutils as KCMUtils

import QtQuick.Controls 2.12 as QQC2

KCMUtils.SimpleKCM {
    Kirigami.FormLayout {
        anchors.fill: parent

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: i18n("Command Syntax")
        }
        Controls.TextField {
            Kirigami.FormData.label: "Trigger Prefix:"
            text: kcm.config.trigger
            onTextChanged: kcm.config.trigger = text
        }
        Controls.Switch {
            Kirigami.FormData.label: "Trailing Space:"
            checked: kcm.config.appendTrailingSpace
            onClicked: kcm.config.appendTrailingSpace = checked
        }

        Kirigami.Separator {
            Kirigami.FormData.isSection: true
            Kirigami.FormData.label: i18n("Script Folder")
        }

        Controls.Button {
            text: "Browse..."
            Kirigami.FormData.label: "Target Folder:"
            //anchors.horizontalCenter: parent.horizontalCenter
            onClicked:
            {
                folderDialog.open()
            }
        }

        Layouts.ColumnLayout {
            Kirigami.FormData.label: "Selected:"
            Kirigami.FormData.labelAlignment: Qt.AlignTop

            Controls.Label {
                id: lbl
                text: kcm.config.filePath
                elide: Text.elideLeft
            }
        }

        Dialogs.FolderDialog {
            id: folderDialog
            title: "Select Target Folder..."
            currentFolder: "file://" + kcm.config.filePath
            onAccepted:
            {
                kcm.config.filePath = selectedFolder.toString().replace("file://", "")
                lbl.text = kcm.config.filePath
            }
        }
    }
}