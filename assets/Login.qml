import bb.cascades 1.0
    
Page {
    id: loginPage
    
    signal loginRequested()
    signal signupRequested()
    
    content: Container {
        layout: DockLayout {
        }
        
        ImageView {
            imageSource: "asset:///images/vk-splash-media.jpg"
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
        }
                    
        // Loging controls
        Container {
            id: buttonContainer
            layout: StackLayout {
            }
            
            topPadding: 15
            bottomPadding: 100
            leftPadding: 70
            rightPadding: 70
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Bottom
             
            Button {
                id: login                
                text: "Login"
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: { 
                    loginPage.loginRequested();    
                }
            }
                            
            Button {
                id: signup                
                text: "Sign up"
                horizontalAlignment: HorizontalAlignment.Fill
                onClicked: {
                    loginPage.signupRequested();              
                }
            }
        }
    }
}