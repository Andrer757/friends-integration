import QtQuick 2.0
import Sailfish.Silica 1.0
import "shared"

BackgroundItem {
    id: item

    width: parent.width
    height: Screen.width * .2

    property Page feedPage
    property Item subviewModel
    property int unseenPostCount: feedPage ? feedPage.unseenPostCount : 0

    onClicked: {
        // TODO
    }

    Label {
        anchors.right: icon.left
        anchors.rightMargin: Theme.paddingLarge
        anchors.verticalCenter: parent.verticalCenter
        color: item.pressed ? Theme.highlightColor : Theme.primaryColor
        text: item.unseenPostCount
        visible: item.unseenPostCount > 0
    }

    SocialImage {
        id: icon
        x: item.height
        width: height
        height: item.height
        connectedToNetwork: feedPage ? feedPage.connectedToNetwork : false
        source: "image://theme/graphic-service-friends"
    }

    Label {
        id: label
        color: item.pressed ? Theme.highlightColor : Theme.primaryColor
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: icon.right
        anchors.leftMargin: Theme.paddingLarge
    }

    function sync() {
        feedPage.sync()
    }
}
