import QtQuick 2.0
import Sailfish.Silica 1.0
import Sailfish.Accounts 1.0
import com.jolla.settings.accounts 1.0
import org.SfietKonstantin.friends.integration 1.0

AccountCreationDialog {
    id: container
    property bool _exists: false
    property bool _canImport: false
    backNavigation: true
    canAccept: false

    FriendsClientIdProvider {
        id: idProvider
    }

    TokenManager {
        id: tokenManager
    }

    Component.onCompleted: {
        // Check if we already have a Friends account
        var existingAccountIds = accountManager.providerAccountIdentifiers("friends")
        if (existingAccountIds.length > 0) {
            container._exists = true
        } else {
            if (tokenManager.token != "") {
                container._canImport = true
            }
        }
    }

    SilicaFlickable {
        id: view
        anchors.fill: parent

        ViewPlaceholder {
            enabled: container._exists
            //: A placeholder text to show that an account already exists
            //% "An account linked with Friends already exists"
            text: qsTrId("friends-integration-account_placeholder-already-exists")
        }

        VerticalScrollDecorator {}

        Column {
            id: column
            width: view.width
            visible: !container._exists

            DialogHeader {
                dialog: container
                //: A text to indicate that this dialog can be skipped
                //% "Skip"
                acceptText: qsTrId("friends-integration-account_dialog-skip")
            }

            Item {
                width: view.width
                height: text.height + 2 * Theme.paddingMedium

                Label {
                    id: text
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right; anchors.rightMargin: Theme.paddingMedium
                    //: A text to indicate what is the goal of this account: deeper integration
                    //% "Connect to Facebook via Friends application. This enables a deeper integration between Friends and the OS."
                    property string _loginText: qsTrId("friends-integration-account_label-connect-description")
                    //: A text to indicate what is the goal of this account: deeper integration, and indicate that you can directly import an account from Friends.
                    //% "Connect to Facebook via Friends application or import from existing Friends connection. This enables a deeper integration between Friends and the OS."
                    property string _loginOrImportText: qsTrId("friends-integration-account_label-connect-or-import-description")
                    wrapMode: Text.WordWrap
                    color: Theme.secondaryHighlightColor
                    text: container._canImport ? _loginOrImportText : _loginText
                }
            }

            BackgroundItem {
                id: connectButton
                enabled: false
                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    //: A text on the connect button
                    //% "Connect to Facebook via Friends"
                    text: qsTrId("friends-integration-account_label-connect")
                    // color: connectButton.highlighted ? Theme.highlightColor: Theme.primaryColor
                    color: Theme.secondaryColor
                }
            }
            BackgroundItem {
                id: importButton
                Label {
                    anchors.left: parent.left; anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    //: A text on the import button
                    //% "Import from Friends"
                    text: qsTrId("friends-integration-account_label-import")
                    color: importButton.highlighted ? Theme.highlightColor: Theme.primaryColor
                }
                onClicked: {
                    container.accountManager.createAccount(container.accountProvider.name)
                    container.canAccept = true
                    container.accept()
                }
            }
        }
    }

    Connections {
        target: container.accountManager
        onAccountCreated: {
            if (providerName == container.accountProvider.name) {
                account.hasSetName = false
                account.identifier = accountId
            }
        }
    }

    Account {
        id: account
        property bool hasSetName: false
        onStatusChanged: {
            if (status == Account.Initialized || status == Account.Synced) {
                if (!hasSetName) {
                    me.load()
                } else {
                    // container._accountCreatedFinished(account.identifier)
                    container.accountCreated(account.identifier)
                }
            }
        }
    }

    Facebook {
        id: facebook
        accessToken: tokenManager.token
    }

    FacebookUser {
        id: me
        socialNetwork: facebook
        filter: FacebookItemFilter {
            identifier: "me"
            fields: "name"
        }

        onLoaded: {
            if (ok) {
                if (me.name == "") {
                    container.accountCreated(account.identifier)
                } else {
                    account.hasSetName = true
                    account.displayName = me.name
                    account.sync()
                }
            } else {
                container.accountCreated(account.identifier)
            }
        }
    }


//    Component {
//        id: importPageComponent

//        AccountCreationDialog {
//            rootAccountCreationPage: container
//            Component.onCompleted: {
//                console.debug("Creating account")
//                container.accountManager.createAccount("friends")
//            }
//        }
//    }
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
//                            _accountCreatedFinished(identifier)
//                        }
//                    }
//                }
//            }

//            SocialNetwork {
//                id: sni
//                onArbitraryRequestResponseReceived: {
//                    var userName = data["name"]
//                    if (userName == undefined || userName == "") {
//                        _accountCreatedFinished(account.identifier)
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
