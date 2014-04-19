import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Accounts 1.0
import com.jolla.settings.accounts 1.0
import org.nemomobile.social 1.0
import QtWebKit.experimental 1.0
import org.SfietKonstantin.friends.integration 1.0

OAuthAccountCreationDialog {
    id: container

    FriendsClientIdProvider {
        id: idProvider
    }

    signonSessionData: {
        "Display": "touch",
        "ClientId": idProvider.clientId()
    }
}

//AccountCreationDialog {
//    id: root

//    anchors.fill: parent

//    // override acceptDestination to be the oauth dialog
//    acceptDestination: authDialogComponent
//    canSkip: false

//    forwardNavigation: true
//    backNavigation: true
//    property bool _haveSetAcceptDestination: false
//    property bool _canNavigate: termsView._isFinishedPanning &&
//                                (termsView._isScrolledToBeginning || termsView._isScrolledToEnd)
//    on_CanNavigateChanged: {
//        // don't bind, as sometimes _canNavigate starts as false for some reason.
//        forwardNavigation = _canNavigate
//        backNavigation = _canNavigate
//    }

//    onAcceptDestinationInstanceChanged: {
//        if (status == PageStatus.Active && acceptDestinationInstance != null) {
//            acceptDestinationInstance.loadWebView()
//        }
//    }

//    onRejected: {
//        if (acceptDestinationInstance != null) {
//            acceptDestinationInstance.cancelWebViewLoading()
//        }
//    }

//    onStatusChanged: {
//        if (status == PageStatus.Active && acceptDestinationInstance != null) {
//            acceptDestinationInstance.loadWebView()
//        }
//    }

//    DialogHeader {
//        id: header
//        //: The "accept terms / data usage" dialog header
//        //% "Consent"
//        title: qsTrId("jolla_settings_accounts_extensions-facebook-consent_header")
//        dialog: root
//    }

//    SilicaFlickable {
//        id: flick
//        anchors {
//            top: header.bottom
//            bottom: parent.bottom
//            left: parent.left
//            right: parent.right
//        }

//        clip: true
//        contentHeight: termsButton.height + consentLabel.height + Theme.paddingLarge * 2
//        contentWidth: parent.width

//        VerticalScrollDecorator {}

//        Label {
//            id: consentLabel
//            anchors.top: parent.top
//            width: parent.width - Theme.paddingLarge*2
//            x: Theme.paddingLarge
//            color: Theme.rgba(Theme.highlightColor, 0.6)

//            //: The text explaining how user's Facebook data will be used on the device
//            //% "When you add a Facebook account, information from this account will be added to the phone to provide a faster and better experience:<br><br> - Friends will be added to the People app and linked with existing contacts, you'll be able to send and receive Facebook Chat messages using the Messages app<br><br> - Facebook events will be added to the Calendar app<br><br> - Facebook posts and notifications will be added to the Feeds view, and you will be able to post status updates on Facebook<br><br> - Facebook photos will be available from the Gallery app, and you will be able to share photos on Facebook<br><br>Some of this data will be cached to make it available when offline. This cache can be cleared at any time by disabling the `Synchronization` service in the account settings page.<br><br>Adding a Facebook account on your device means that you agree to Facebook's Terms of Service."
//            text: qsTrId("jolla_settings_accounts_extensions-facebook-consent_text")
//            wrapMode: Text.WordWrap
//            font.pixelSize: Theme.fontSizeSmall
//        }

//        Button {
//            id: termsButton
//            anchors {
//                topMargin: Theme.paddingLarge*2
//                top: consentLabel.bottom
//                horizontalCenter: consentLabel.horizontalCenter
//            }

//            //: Button which the user presses to view Facebook Terms Of Service webpage
//            //% "Facebook's Terms"
//            text: qsTrId("jolla_settings_accounts_extensions-facebook-btn-view_terms_btn")
//            onClicked: {
//                flick.visible = false
//                consentLabel.visible = false
//                termsButton.visible = false
//                termsView.visible = true
//            }
//        }
//    }

//    SilicaWebView {
//        id: termsView
//        visible: false
//        overridePageStackNavigation: true
//        property bool _isScrolledToEnd: (termsView.contentY + termsView.height + 2) >= termsView.contentHeight
//        property bool _isScrolledToBeginning: termsView.contentY <= 2
//        property bool _isFinishedPanning: termsView.atXBeginning && termsView.atXEnd && !termsView.moving
//        property bool _isFullyZoomedOut: (termsView.contentWidth <= termsView.width - 2) || (termsView.contentWidth <= termsView.width + 2)
//        experimental.temporaryCookies: true
//        experimental.deviceWidth: termsView.width
//        experimental.deviceHeight: termsView.height
//        experimental.userAgent: "Mozilla/5.0 (Linux; U; Jolla; Sailfish; Mobile; rv:20.0) Gecko/20.0 Firefox/20.0 Sailfish Browser/1.0 like Safari/535.19"
//        url: "https://m.facebook.com/legal/terms"
//        anchors {
//            top: header.bottom
//            bottom: root.bottom
//            left: root.left
//            right: root.right
//        }
//    }

//    Component {
//        id: authDialogComponent

//        OAuthAccountCreationDialog {
//            // automatically set account creation parameters provided to the root creation page
//            rootAccountCreationPage: root

//            property QtObject _skp: StoredKeyProvider {}
//            signonSessionData: {
//                "Display": "touch",
//                "ClientId": _skp.storedKey("facebook", "facebook-sync", "client_id")
//            }
//            signonServiceName: "facebook-sync" // sync requests the most scopes

//            function postSignIn(signInResponseData, newAccountId) {
//                account.hasSetName = false
//                account.accessToken = signInResponseData["AccessToken"]
//                account.identifier = newAccountId  // this will trigger account sync
//            }

//            Account {
//                id: account
//                property string accessToken
//                property bool hasSetName: false
//                onStatusChanged: {
//                    if (status == Account.Initialized || status == Account.Synced) {
//                        if (!hasSetName) {
//                            var queryItems = {"access_token":accessToken}
//                            sni.arbitraryRequest(SocialNetwork.Get, "https://graph.facebook.com/me?fields=name", queryItems)
//                        } else {
//                            postSignInFinished(identifier)
//                        }
//                    }
//                }
//            }

//            SocialNetwork {
//                id: sni
//                onArbitraryRequestResponseReceived: {
//                    var userName = data["name"]
//                    if (userName == undefined || userName == "") {
//                        postSignInFinished(account.identifier)
//                    } else {
//                        account.hasSetName = true
//                        account.displayName = userName
//                        account.sync()
//                    }
//                }
//            }
//        }
//    }
//}
