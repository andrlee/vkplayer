import bb.cascades 1.0

Container {
    id: loader
    
    function stop() {
        activity.stop();
    }
    
    function isRunning() {
        return loading.visible || refresh.visible;
    }
    
    signal update();
    
    layout: DockLayout {
    }
    
    preferredHeight: 100
    preferredWidth: 768
    
    Container {        
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
            
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
            
        ActivityIndicator {
            id: activity
        }
            
        Label {
            id: loading
            text: "Loading..."
            visible: activity.running
        }
            
        Label {
            id: refresh
            text: "Pull down to refresh"
            visible: !activity.running
        }
    }
    
    attachedObjects: [            
        LayoutUpdateHandler {
            id: pos         
            onLayoutFrameChanged: {
                if (layoutFrame.y > 0 && !activity.running) {
                    loader.update();
                    activity.start();
                    updateDone.start(2000);
                }
            }
        },
                
        QTimer {
            id: updateDone
            singleShot: true
            onTimeout: {
                loader.preferredHeight = 0;
                resetUpdater.start(50);
            }
        },
                
        QTimer {
            id: resetUpdater
            singleShot: true
            onTimeout: {
                loader.preferredHeight = 100;
                activity.stop();
            }
        }
    ]
}