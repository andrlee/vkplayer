import bb.cascades 1.0

StandardListItem {
    id: item
    title: ListItemData.artist
    description: ListItemData.title
   
    contextActions: [
        ActionSet {
            title: ListItemData.title
            subtitle: ListItemData.artist
            
            DeleteActionItem {
                title: "Delete"
                onTriggered: {
                    item.ListItem.view.itemDeleted(ListItemData.aid);
                }
            }
        }
    ]
}

//Container {
//    id: item
//    property alias artist: artist.text
//    property alias title: title.text
//    
//    signal deleteTrack()
//    
//    layout: StackLayout {
//    }
//    
//    Divider {}
//    
//    Container {
//        layout : DockLayout {
//        }
//    
//        horizontalAlignment: HorizontalAlignment.Fill
//        verticalAlignment: VerticalAlignment.Center
//    
//        // Artist and title
//        Container {
//            layout: StackLayout {
//            }
//        
//            rightPadding: 5
//        
//            Label {
//                id: title
//                textStyle {
//                    base: SystemDefaults.TextStyles.TitleText
//                    fontWeight: FontWeight.Bold
//                }
//            }
//        
//            Label {
//                id: artist
//                textStyle {
//                    color: Color.Gray    
//                }
//            }
//        
//        }
//    
//        Label {
//            id: duration
//            textStyle {
//                color: Color.Gray    
//            }
//        
//            horizontalAlignment: HorizontalAlignment.Right
//        }
//    }
//
//    Divider {}
//    
//    contextActions: [
//        ActionSet {
//            title: title.text
//            subtitle: artist.text
//                                             
//            DeleteActionItem {
//                title: qsTr("Delete")
//                onTriggered: {
//                    item.deleteTrack();               
//                }
//            }
//         }
//    ]
//}