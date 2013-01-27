import bb.cascades 1.0

Page {
    id: musicPage
    property variant player
    property alias dataModel : playlist
    
    signal updateMusic()
    signal deleteMusic(string aid)
   
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
        ListView {
            signal itemDeleted(string aid)
            onItemDeleted : musicPage.deleteMusic(aid);
            
            dataModel: VkontaktePlaylistModel {
                id: playlist
                filename: "my_playlist.m3u"
                
                onItemsChanged : {                    
                    loader.stop();
                }
            }
            
            listItemComponents: [
                ListItemComponent {
                    MusicListItem {
                    }
                }
            ]
            
            leadingVisual : LeadingVisual {
                id: loader
                onUpdate : musicPage.updateMusic();
            }
            
            onTriggered: {
                player.open();
                player.mylist = true;
                player.play(playlist.path(), playlist.indexOfItem(indexPath));
            }
        }
    }
}