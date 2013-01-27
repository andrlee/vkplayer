import bb.cascades 1.0

Page {
    id: searchPage
    property variant player
    property alias dataModel: playlist
    
    signal searchText(string text)
    
    actions: [                
        ActionItem {
            title: qsTr("Now Playing")
            imageSource: "asset:///images/action_playing.png"
            onTriggered: {
                player.open();
            }
        }
    ]  
    
    Container {
        layout: StackLayout {
        }
    	    
    	verticalAlignment: VerticalAlignment.Center
    	horizontalAlignment: HorizontalAlignment.Fill
    	    
    	    Container {
    	        topPadding: 10
    	        bottomPadding: 10
    	        rightPadding: 15
    	        leftPadding: 15
    	        
    	        TextField {
    	            id: search
    	            hintText: "Search music"
    	            	        
    	            onTextChanging: {
    	                console.log("onTextChanging: " + text);
    	                doneTyping.stop();
    	            
    	                if (!text || 0 === text.length)
    	                {
    	                    playlist.clear();
    	                }
    	                else
    	                {
    	                    doneTyping.start(300);
    	                }
    	            }    
    	        }
    	    }

    	    ListView {
                dataModel: VkontaktePlaylistModel {
                    id: playlist
                    filename: "search_playlist.m3u"
                }
                
                listItemComponents: [
                    ListItemComponent {
                        MusicListItem {
                        }
                    }
                ]
                 
                onTriggered: {
                    player.open();
                    player.mylist = false;
                    player.play(playlist.path(), playlist.indexOfItem(indexPath));
                }
            }
    }
    
    attachedObjects: [
        QTimer {
            id: doneTyping
            singleShot: true
            onTimeout: {
                console.log("onTimeout");
                searchPage.searchText(search.text);
            }
        }
    ]
}