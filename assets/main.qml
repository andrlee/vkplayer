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
                vkConnection.retrieveMusic(activity);
            }
            
            onSearchText : {
                console.log("onSearchText: " + text);
                musicPage.startActivity();
                vkConnection.search(text, 200);
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
        id: videos
        title: qsTr("Videos")
        imageSource : "asset:///images/ic_tab_videos_unselected.png"
        
        Videos {
            id: videoPage
            
            onUpdateVideos : {
                vkConnection.retrieveVideos(activity);
            }
            
            onSearchText : {
                console.log("onSearchText: " + text);
                videoPage.startActivity();
                vkConnection.searchVideo(text, 50);
            }
            
            onAddVideo : {
                console.log("onAddMusic");                
                vkConnection.addVideo(vid, oid);
            }
            
            onDeleteVideo: {        
                vkConnection.deleteVideo(vid);
            }
        }
    }
    
    onActiveTabChanged: {
        if (activeTab == music)
        {
            vkConnection.retrieveMusic(false);
        }
        else if (activeTab == videos)
        {
            vkConnection.retrieveVideos(false);
        }
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
                    retrieveMusic(true);
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
                musicPage.stopActivity();  
                if (success)
                {
                    console.log("Music loaded");  
                    _musicPlaylist.appendItem(audioList);
                }
            }
            
            onSearchMusicLoaded : {
                console.log("onSearchMusicLoaded");
                musicPage.stopActivity();   
                if (success)
                {   
                    _musicPlaylist.appendItem(audioList);
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
                    retrieveMusic(true);
                }
            }
            
            onVideosLoaded : {
                console.log("onVideosLoaded");
                videoPage.stopActivity();
                if (success)
                {
                    console.log("Videos loaded");   
                    _videoPlaylist.appendItem(videoList);
                }
            }
            
            onSearchVideoLoaded : {
                console.log("onSearchVideoLoaded");
                videoPage.stopActivity();
                if (success)
                {   
                    _videoPlaylist.appendItem(videoList);
                }
            }
            
            onVideoAdded : if (success) console.log("onVideoAdded");
            
            onVideoDeleted : {
                console.log("onVideoDeleted");
                if (success) 
                {
                    retrieveVideos(true);
                }
            }
            
            function retrieveVideos(activity) {
                if (activity)
                    videoPage.startActivity();
                    
                vkConnection.loadVideos(50);
            }
            
            function retrieveMusic(activity) {
                if (activity)
                    musicPage.startActivity();
                    
                vkConnection.loadMusic(200);
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