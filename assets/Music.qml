import bb.cascades 1.0

Page {
    id: musicPage
    property variant player
    
    signal updateMusic()
    signal searchText(string text)
    signal deleteMusic(string aid)
    signal addMusic(string aid, string oid)
   
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
	            
	            onFocusedChanged : {
	                if (focused && search.text.length == 0) _playlistModel.clear();
	                else if (!focused && search.text.length == 0) musicPage.updateMusic();
	            }
	            	        
	            onTextChanging: {
	                console.log("onTextChanging: " + text);
	                doneTyping.stop();
	            
	                if (!text || 0 === text.length)
	                {
	                    _musicPlaylist.clear();
	                }
	                else
	                {
	                    doneTyping.start(300);
	                }
	            }    
	        }
	    }
    	
        ListView {
            signal itemDeleted(string aid)
            signal itemAdded(string aid, string oid)
            
            onItemDeleted : musicPage.deleteMusic(aid);
            onItemAdded: musicPage.addMusic(aid, oid);
            
            dataModel: _musicPlaylist
            
            listItemComponents: [
                ListItemComponent {
                    MusicListItem {
                    }                       
                }
            ]
            
            leadingVisual : LeadingVisual {
                id: loader
                onUpdate : if (search.text.length == 0) musicPage.updateMusic();
            }
            
            onTriggered: {
                player.open();
                player.playTrack(_musicPlaylist.indexOfItem(indexPath));
            }
        }
    }
    
    attachedObjects: [
        QTimer {
            id: doneTyping
            singleShot: true
            onTimeout: {
                console.log("onTimeout");
                musicPage.searchText(search.text);
            }
        }
    ]
}