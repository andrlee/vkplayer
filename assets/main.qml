import bb.cascades 1.0
import bb.system 1.0
import SocialConnect 1.0

TabbedPane {
    id: tabs
    showTabsOnActionBar: true
    
    Tab {
        id: music
        title: qsTr("Music")
        imageSource : "asset:///images/ic_tab_music_unselected.png"
        
        Music {
            id: musicPage
            player: media
                
            onUpdateMusic : {
                vkConnection.retrieveMusic();
            }
            
            onSearchText : {
                console.log("onSearchText: " + text);
                vkConnection.search(text, 100);
            }
            
            onDeleteMusic: {        
                 vkConnection.deleteMusic(aid);
            }
            
            onAddMusic : {
                console.log("onAddMusic");                
                vkConnection.addMusic(aid, oid);
            }
        }
    }
    
    Tab {
        id: playlist
        title: qsTr("Playlist")
    }
    
    onActiveTabChanged: {
    }
    
    attachedObjects: [
        Sheet {
            id: loginDialog
            Page {
                Container {
                    background: Color.LightGray
                    
                    ScrollView {
                        visible: true
                    		        
                         scrollViewProperties {
                    	     scrollMode: ScrollMode.Vertical
                    	 }
                    	 
                    	 WebView {
                             id: webView
                             preferredHeight: parent.height
                             preferredWidth: parent.width
                             onUrlChanged: {
                                 console.log("WebView onUrlChanged")
                                 webInterface.url = url;
                             }
                             
                             onNavigationRequested: {
                                 console.log("NavigationRequested: " + request.url + " navigationType=" + request.navigationType)
                             }   
                         }
                     }
                 }
            }
        },
                
        SystemToast {
            id: toast
                        
            function push(text) {
                body = qsTr(text);
                show();
            }
            
            function pop() {
                cancel();
            }
        },       
        
        WebInterface {
            id: webInterface
            onUrlChanged: { 
                console.log("WebInterface onUrlChanged")
                webView.url = url;
            }
        },
        
        VKMediaPlayer {
            id: media
                                        
            onAddMusic : {
                console.log("onAddMusic");                
                vkConnection.addMusic(aid, oid);
            }
        },
                
        VKontakteConnection {
            id: vkConnection
            webInterface: webInterface 
            permissions: ["wall", "status", "friends", "audio", "video", "offline"]
            clientId: "3269076"
            
            onAuthenticateCompleted: {
                console.log("onAuthenticateCompleted");
                if (success)
                {
                    console.log("Authentication success");                    
                    loginDialog.close();
                    retrieveMusic();
                } 
                else
                { 
                    toast.push("Login Failed. Try Again!");
                    authenticate();
                }
            }
            
            onDeauthenticateCompleted: {
                console.log("onDeauthenticateCompleted");
                if (success)
                {
                    removeCredentials();
                    loginDialog.open();
                }
            }
            
            onMusicLoaded : {
                console.log("onMusicLoaded");
                if (success)
                {
                    console.log("Music loaded");   
                    _playlistModel.appendItem(audioList);
                }
            }
            
            onSearchMusicLoaded : {
                console.log("onSearchMusicLoaded");
                if (success)
                {   
                    _playlistModel.appendItem(audioList);
                }
            }
            
            onMusicAdded : {
                if (success) 
                {
                    media.musicAdded();
                }
            }
            
            onMusicDeleted : {
                console.log("onMusicDeleted");
                if (success) 
                {
                    retrieveMusic();
                }
            }
            
            function retrieveMusic() {
                 vkConnection.loadMusic(100);
            }
        }
    ]
    
    onCreationCompleted: {
        console.log("Authenticate");
        
        // Restore credentials
        vkConnection.restoreCredentials();
        
        // Authenticate if credentials invalid
        if (!vkConnection.authenticated)
        {
            console.log("Authentication required");
            
            loginDialog.open();
            vkConnection.authenticate();       
        }
    }
}