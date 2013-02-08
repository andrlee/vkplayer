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
