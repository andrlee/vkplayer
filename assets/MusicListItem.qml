import bb.cascades 1.0

StandardListItem {
    id: item
    title: ListItemData.artist
    description: ListItemData.title
    
    contextActions: [
        ActionSet {
            title: ListItemData.title
            subtitle: ListItemData.artist
            
            ActionItem {
                title: ListItemData.owned ? qsTr("Delete"): qsTr("Add")
                imageSource: ListItemData.owned ? "asset:///images/action_delete.png" : "asset:///images/action_add.png"
                onTriggered: {
                    if (ListItemData.owned)
                    {
                        item.ListItem.view.itemDeleted(ListItemData.aid);
                    }
                    else
                    {
                        item.ListItem.view.itemAdded(ListItemData.aid, ListItemData.owner_id);
                    }
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