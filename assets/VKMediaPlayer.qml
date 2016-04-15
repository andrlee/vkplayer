import bb.cascades 1.0
import bb.multimedia 1.0

Sheet {
    id: mediaDialog
                    
    signal addMusic(string aid, string oid)
        
    function musicAdded() {
        add.enabled = false;
        add.checked = false;
    }
    
    function playTrack(index) {
        var item = _musicPlaylist.value(index);
        
        artist.text = item["artist"]; 
        title.text = item["title"];
        player.sourceUrl = item["url"];
                 
        if (MediaError.None == player.play())
        {
            _musicPlaylist.playingTrack = index;
        }
    }
    
    function stop() {
        player.stop();
        duration.value = 0;
    }
                
    Page {
        titleBar: TitleBar {
            appearance: TitleBarAppearance.Default
                    
            dismissAction: ActionItem {
                title: qsTr("Done")
                onTriggered: {
                    mediaDialog.close();
                }
            }
        }    
        
        // Control tab        
        Container {
            layout: DockLayout  {
            }
            
            ImageView {
                imageSource: "asset:///images/background_fancy.jpg"
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
            }
            
            Container {
                layout: StackLayout {
                }
                
                horizontalAlignment: HorizontalAlignment.Fill
                                            
                topPadding: 40
                leftPadding: 30
                rightPadding: 30
                              
               Container {
                   layout: DockLayout  {
                   }
                   
                   horizontalAlignment: HorizontalAlignment.Fill
                   
                   Label {
                       id: elapsed
                       text: duration.msToString(toValue, false)
                       textStyle {
                          color: Color.White
                       }
                       
                       verticalAlignment: VerticalAlignment.Center 
                   }
                   
                   Slider {
                       id: duration
                       property bool seekInProgress: false
                       property int requestedValue
                       property bool wasPlaying: false
                       
                       enabled: false
                       fromValue: 0
                       toValue: player.duration
                       preferredWidth: 530
                    
                       horizontalAlignment: HorizontalAlignment.Center
                       verticalAlignment: VerticalAlignment.Center
                       
                       onTouch: {
	                       if (event.isDown()) {	                           
	                           seekInProgress = true;
	                           if (!play.cheked) {
	                               wasPlaying = true;
	                               // pause the playback so that the it doesn't keep trying to play it while being dragged/seeked (which causes audio disrupt)
	                               player.pause();
	                           }
	                       } else if (event.isUp() || event.isCancel()) {	                           
	                           seekInProgress = false;
	                           if (wasPlaying) {
	                               player.play();
	                           }
	                           wasPlaying = false;
	                           player.seekTime(requestedValue);
	                       } // else if
	                   }
                       
                       onImmediateValueChanged	: {
                           elapsed.text = msToString(immediateValue, false);
                           left.text = msToString(toValue - immediateValue, true);
                           requestedValue = immediateValue;
                       }
                       
                       function msToString(milliseconds, negative) {
                           var seconds = parseInt(milliseconds / 1000) % 60;
                           var minutes = parseInt(milliseconds / 60000);
                           
                           return (negative ? '-':'') + minutes + ':' + (seconds < 10 ? '0' + seconds : seconds);
                       }
                   }
                   
                   Label {
                       id: left
                       text: duration.msToString(fromValue, true)
                       textStyle {
                           color: Color.White
                       } 
                       
                       horizontalAlignment: HorizontalAlignment.Right
                       verticalAlignment: VerticalAlignment.Center
                   }
               }
               
               // Control buttons
               Container {
                   layout: StackLayout {
                       orientation: LayoutOrientation.LeftToRight
                   }
                   
                   topPadding: 20
                   
                   horizontalAlignment: HorizontalAlignment.Center
                   
                   ImageToggleButton {
	                   id: repeat
	                   imageSourceDefault: "asset:///images/ic_tab_repeat_selected.png"
	                   imageSourceChecked: "asset:///images/ic_tab_repeat_checked.png"
	                   
	                   rightMargin: 150
	                   leftMargin: 150
	                   
	                   onCheckedChanged : {}
	               }
	               
	               ImageToggleButton {
   	                   id: add
   	                   enabled: !_musicPlaylist.myTrack
   	                   imageSourceDefault: "asset:///images/ic_tab_add_selected.png"
   	                   imageSourceDisabledUnchecked: "asset:///images/ic_tab_add_unselected.png"
   	                   
   	                   rightMargin: 150
   	                   leftMargin: 150
   	               
   	                   onCheckedChanged : {
   	                       if (add.enabled)
   	                       {
   	                           var item = _musicPlaylist.value(_musicPlaylist.playingTrack);
   	                           mediaDialog.addMusic(item["aid"], item["owner_id"]);
   	                       }
   	                   }
   	               }
   	               
//   	               ImageToggleButton {
//  	                   id: broadcast
//  	                   imageSourceDefault: "asset:///images/ic_tab_broadcast_selected.png"
//  	                   imageSourceChecked: "asset:///images/ic_tab_broadcast_selected.png"
//  	                   
//  	                   rightMargin: 150
//                       leftMargin: 150
//                       
//  	                   onCheckedChanged : {
//  	                   }
//  	               } 
  	               
  	               ImageToggleButton {
 	                   id: shuffle
 	                   imageSourceDefault: "asset:///images/ic_tab_shuffle_selected.png"
 	                   imageSourceChecked: "asset:///images/ic_tab_shuffle_checked.png"
 	                            
 	                   rightMargin: 150
   	                   leftMargin: 150
 	               } 
               }   
            }
            
            // Artist and song title
            Container {
                layout: StackLayout {
                }
                
                bottomPadding: 100                
                leftPadding: 30
                rightPadding: 30 
                
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                
                Label {
                    id: artist
                    multiline: true
                    textStyle {
                        base: SystemDefaults.TextStyles.TitleText
			            fontWeight: FontWeight.Bold
			            fontSize: FontSize.XLarge
                        color: Color.White
                        textAlign: TextAlign.Center
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                }
                
                Label {
                    id: title
                    multiline: true
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.White
                        textAlign: TextAlign.Center
                    }
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
            
            // Buttons
            Container {
                layout : StackLayout {
                }
                
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Bottom
                
                Container {
                    layout: DockLayout{
                    }
                
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Bottom
                
                    bottomPadding: 60
                    leftPadding: 100
                    rightPadding: 100
                
                    ImageButton {
                        id: rewind
                        defaultImageSource: "asset:///images/ic_rewind.png"
                        pressedImageSource: "asset:///images/ic_rewind.png"
                    
                        onClicked : {
                            mediaDialog.playTrack(_musicPlaylist.previousTrack());
                        }
                    }
                
                    ImageToggleButton {
                        id: play
                        checked: true
                        imageSourceDefault: "asset:///images/ic_play.png"
                        imageSourcePressedUnchecked: "asset:///images/ic_play.png"
                        
                        imageSourceChecked: "asset:///images/ic_pause.png"
                        imageSourcePressedChecked: "asset:///images/ic_pause.png"
                    
                        horizontalAlignment: HorizontalAlignment.Center
                    
                        onCheckedChanged : checked ? player.play() : player.pause()
                    }
                
                    ImageButton {
                        id: forward
                        defaultImageSource: "asset:///images/ic_forward.png"
                        pressedImageSource: "asset:///images/ic_forward.png"
                        
                        horizontalAlignment: HorizontalAlignment.Right
                    
                        onClicked : {
                            mediaDialog.playTrack(_musicPlaylist.nextTrack());
                        }
                    }
                }
                
                Container {
                    layout: DockLayout{
                    }
                                
                    horizontalAlignment: HorizontalAlignment.Center
                    
                    bottomPadding: 60
            
                    VolumeSlider {
                    }
                }
            }
        }
     }
             
     attachedObjects: [
         ForeignWindowControl {
             id: videoSurface
             windowId: "myVideoSurface"
             updatedProperties: WindowProperty.Size | WindowProperty.Position | WindowProperty.Visible
             preferredWidth: parent.width
             preferredHeight: parent.height
         },
         
         MediaPlayer {
             id: player
             autoPause: true
             videoOutput: VideoOutput.PrimaryDisplay
             windowId: videoSurface.windowId
             
             onSpeedChanged : {
                 console.log("onSpeedChanged: " + player.speed);
                 if (player.speed == 1)
                 {
                      play.checked = true;
                 }
                 else
                 {
                      play.checked = false;
                 }
             }
             
             onBuffering : {
//                 buffering.text = "Buffering " + parseInt(percentage*100) + " %";
//                 
//                 if (percentage == 1 || percentage == 0)
//                 {
//                     buffering.visible = false;
//                 }
//                 else
//                 {
//                     buffering.visible = true;
//                 }
             }
             
             onTrackChanged : {
                 duration.value = 0;
                 if (_musicPlaylist.myTrack)
                 {
                     add.enabled = false;
                     add.checked = false;
                 }
                 else
                 {
                     add.enabled = true;
                 }
             }
             
             onMetaDataChanged : {
                 console.log("onMetaDataChanged");
             }
             
             onPositionChanged: {
                 if (!duration.seekInProgress) {
                     duration.value = position;
                 }
             }
             
             onPlaybackCompleted : {
                 if (shuffle.checked)
                 {
                     mediaDialog.playTrack(_musicPlaylist.randomTrack())
                     return;
                 }
                 
                 if (repeat.checked)
                 {
                     player.seekTime(0);
                     player.play();
                 }
                 else
                 {
                     mediaDialog.playTrack(_musicPlaylist.nextTrack());
                 }
             }
             
             onSeekableChanged: duration.enabled = player.seekable;
         }
     ]
}