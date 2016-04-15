import bb.cascades 1.0

StandardListItem {
    id: item
    title: ListItemData.title
    description: ListItemData.duration
    image: tracker.image
    
    contextActions: [
        ActionSet {
            title: ListItemData.title
            subtitle: ListItemData.description
            
            ActionItem {
                title: ListItemData.owned ? qsTr("Delete"): qsTr("Add")
                imageSource: ListItemData.owned ? "asset:///images/action_delete.png" : "asset:///images/action_add.png"
                onTriggered: {
                    if (ListItemData.owned)
                    {
                        item.ListItem.view.itemDeleted(ListItemData.vid);
                    }
                    else
                    {
                        item.ListItem.view.itemAdded(ListItemData.vid, ListItemData.owner_id);
                    }
                }
            }
        }
    ]
    
    attachedObjects: [
        // The NetImageTracker is a custom ImageTracker with the ability to 
        // load images from the internet (see netimagetracker.h/.cpp). 
        NetImageTracker {
            id: tracker
            
            // The image source is set 
            source: ListItemData.image
            
            // In this sample the same manager is used for all list item trackers, 
            // it handles the networked requests and contain the image cache (see netimagetracker.cpp/.h).
            // Note that if managers with many different cacheIds are used you have to be 
            // careful not to use up to much disk space for the different caches.
            manager: item.ListItem.view.listImageManager
        }
    ]
}
