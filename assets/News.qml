import bb.cascades 1.0

NavigationPane {
    property alias dataModel: messageListModel
        
    Page {    
        Container {
            ListView {
                dataModel: GroupDataModel {
                    id: messageListModel
                    sortingKeys: ["id"]
                    sortedAscending: false
                }
                                        
                listItemComponents: [
                    ListItemComponent {
                        type: "item"
                        StandardListItem {
                            title: ListItemData.text
                            description: ListItemData.date
                        }
                    }
                 ]
            }
        }
    }
    onPopTransitionEnded: { page.destroy(); }
}