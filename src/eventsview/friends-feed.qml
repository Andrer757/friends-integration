import QtQuick 2.0
import Sailfish.Silica 1.0
import org.SfietKonstantin.friends.integration 1.0
import org.SfietKonstantin.friends.integration.eventsview 1.0
import org.nemomobile.dbus 1.0
import "shared"

SocialMediaFeedPage {
    id: page
    //% Friends service name
    //: "Friends"
    function sync() {
        if (page.connectedToNetwork) {
            syncHelper.sync()
        } else {
            if (page.listModel) {
                // we may have old data in the database anyway.
                // attempt to refresh the list model with that data.
                page.listModel.refresh()
            }

            // also attempt to connect to the network,
            // and queue a sync for when (if) it succeeds.
            page._needToSync = true
            connectionHelper.attemptToConnectNetwork()
        }
    }

    //: Title for the events view page
    //% "Friends Facebook feeds"
    headerTitle: qsTrId("friends-integration-eventsview_feed-title")
    configKey: "/sailfish/events_view/friends"
    timestampRole: FriendsPostModel.TimestampRole
    listModel: FriendsPostModel {}
    listDelegate: PostDelegate {
        post: model.contentItem
        to: model.contentItem.to.length > 0 ? model.contentItem.to[0] : null
        fancy: false
        pushComments: false
        onClicked: dbusInterface.call("openFacebookEntity", [{"type": "s", "value": model.identifier}])
    }

    SyncHelper {
        id: syncHelper
        onLoadingChanged: {
            if (loaded) {
                page.listModel.refresh()
            }
        }
    }

    TokenManager {
        id: tokenManager
    }

    Facebook {
        id: facebook
        accessToken: tokenManager.token
    }

    ViewPlaceholder {
        flickable: page.listView
        enabled: page.listModel.count == 0
        //: Empty state for the events page: no feeds
        //% No feeds
        text: qsTrId("friends-integration-eventsview_feed-placeholder")
    }

    DBusInterface {
        id: dbusInterface
        destination: "harbour.friends"
        path: "/"
        iface: "harbour.friends"
        busType: DBusInterface.SessionBus
    }
}
