import QtQuick 2.0
import Sailfish.Silica 1.0
import "shared"

SocialMediaAccountDelegate {
    id: delegate
    iconSource: "image://theme/graphic-service-friends"
    //: New posts
    //% "New posts"
    text: unseenPostCount > 0 ? qsTrId("friends-integration-eventsview_delegate-new-posts")
                                //: Facebook posts
                                //% "Facebook posts"
                              : qsTrId("friends-integration-eventsview_delegate-posts")
}
