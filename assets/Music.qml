import bb.cascades 1.0

Page {
    id: musicPage
    property variant player
    
    signal updateMusic(bool activity)
    signal searchText(string text)
    signal deleteMusic(string aid)
    signal addMusic(string aid, string oid)
    
    function startActivity() {
        activity.visible = true;
        activity.start();
    }
    
    function stopActivity() {
        activity.visible = false;
        activity.stop();
    }
   
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
        layout: DockLayout  {
        }
    	
    	Container {
    	    layout: StackLayout {
    	    }
    	    
        	horizontalAlignment: HorizontalAlignment.Fill
        	
        	Container {
        	    layout : DockLayout {
        	    }
        	    
                preferredHeight: search.preferredHeight + searchContainer.topPadding
        	    
        	    ImageView {
	                imageSource: "asset:///images/search_background.png"
	                verticalAlignment: VerticalAlignment.Fill
	                horizontalAlignment: HorizontalAlignment.Fill
        	    }
        	    
	        	Container {
	        	    id: searchContainer
	        	    topPadding: 20
	        		bottomPadding: 20
	        		rightPadding: 15
	        		leftPadding: 15
	        		
	        		TextField {
			            id: search
			            hintText: qsTr("Search music")
			            
			            onFocusedChanged : {
			                if (focused && search.text.length == 0) _musicPlaylist.clear();
			                else if (!focused && search.text.length == 0) musicPage.updateMusic(true);
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
	        }
        	
        	Container {
        	    layout: DockLayout {
        	    }
        	    
        	    Container {
        	        topPadding: 20
        	        rightPadding: searchContainer.rightPadding
	        		leftPadding: searchContainer.leftPadding
        	        horizontalAlignment: HorizontalAlignment.Center
        	        
        	        ActivityIndicator {
    	                id: activity
    	            }
    	        }
    	        
    	        ListView {
	                id: list
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
	                    onUpdate : if (search.text.length == 0) musicPage.updateMusic(false);
	                }
	                
	                onTriggered: {
	                    player.open();
	                    player.playTrack(_musicPlaylist.indexOfItem(indexPath));
	                }
	            }     	    
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