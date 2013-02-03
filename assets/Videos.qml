import bb.cascades 1.0

Page {
    id: videosPage
    
    signal updateVideos(bool activity)
    signal searchText(string text)
    signal deleteVideo(string vid)
    signal addVideo(string vid, string oid)
    
    function startActivity() {
        activity.visible = true;
        activity.start();
    }
    
    function stopActivity() {
        activity.visible = false;
        activity.stop();
    }
    
    Container {
        layout: DockLayout {
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
	                imageSource: search.focused ? "asset:///images/search_background_focused.png" 
	                                            : "asset:///images/search_background_unfocused.png"
	                
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
			            hintText: qsTr("Search videos")
			            
			            onFocusedChanged : {
			                if (focused && search.text.length == 0) _videoPlaylist.clear();
			                else if (!focused && search.text.length == 0) videosPage.updateVideos(true);
			            }
			            	        
			            onTextChanging: {
			                console.log("onTextChanging: " + text);
			                doneTyping.stop();
			            
			                if (!text || 0 === text.length)
			                {
			                    _videoPlaylist.clear();
			                }
			                else
			                {
			                    doneTyping.start(600);
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
        	        property variant listImageManager: videoImageManager
        	            
        	        signal itemDeleted(string vid)
        	        signal itemAdded(string vid, string oid)
        	        
        	        onItemDeleted : videosPage.deleteVideo(vid);
        	        onItemAdded: videosPage.addVideo(vid, oid);
        	        
        	        dataModel: _videoPlaylist
        	        
        	        listItemComponents: [
        	            ListItemComponent {
        	                VideoListItem {
        	                }                
        	            }
        	        ]
        	        
        	        leadingVisual : LeadingVisual {
        	            id: loader
        	            onUpdate : if (search.text.length == 0) videosPage.updateVideos(false);
        	        }
        	        
        	        onTriggered: {
        	            console.log("onTriggered");
        	            var item = _videoPlaylist.value(_videoPlaylist.indexOfItem(indexPath));
        	            invoke.invokeMediaPlayer(item["player"]);
        	        }
        	        
        	        attachedObjects: [
                        // Images from the internet are tracked via a NetImageTracker and it 
                        // in turn NetImageManager to make requests and cache images. Be careful
                        // not to use NetImageManager with many different cacheId:s since the cached
                        // images are stored and use up disk space on the device. 
                        NetImageManager {
                            id: videoImageManager
                            cacheId: "videoImageManager"
                        }
                    ]
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
                videosPage.searchText(search.text);
            }
        },
        
        InvokeManager {
            id: invoke
        }
    ]
}  
