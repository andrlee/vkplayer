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
            
            onDeleteMusic: {        
                 vkConnection.deleteMusic(aid, vkConnection.userId);
            }
        }
    }
    
    Tab {
        id: playlist
        title: qsTr("Playlist")
    }
    
    Tab {
        id: search
        title: qsTr("Search")
        imageSource : "asset:///images/ic_tab_search_unselected.png"
        
        Search {
            id: searchPage
            player: media
            
            onSearchText : {
                console.log("onSearchText: " + text);
                vkConnection.search(text, 100);
            }
        }
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
                var item = searchPage.dataModel.value(index);
                
                vkConnection.addMusic(item["aid"], item["owner_id"]);
            }
            
            onTrackChanged: {
                console.log("onTrackChanged");
                var item = media.mylist ? musicPage.dataModel.value(index) : searchPage.dataModel.value(index);                
                media.artist = item["artist"]; 
                media.title = item["title"];
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
                    musicPage.dataModel.appendItem(audioList);
                }
            }
            
            onSearchMusicLoaded : {
                console.log("onSearchMusicLoaded");
                if (success)
                {   
                    searchPage.dataModel.appendItem(audioList);
                }
            }
            
            onMusicAdded : {
                if (success) 
                {
                    retrieveMusic();
                    media.musicAdded();
                }
            }
            
            onMusicDeleted : {
                console.log("onMusicDeleted");
                if (success) 
                {
                    retrieveMusic();
                    if (media.mylist)
                    {
                        media.play(musicPage.dataModel.path(), 0);
                    }
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