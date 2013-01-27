import bb.cascades 1.0

Sheet {
    Page {
        content: Container {
            background: Color.LightGray
            layout: DockLayout {
            }
            
            //TODO: logo
            
            // Loging controls
            Container {
                id: buttonContainer
                layout: StackLayout {
                }
                
                topPadding: 15
                bottomPadding: 15
                leftPadding: 30
                rightPadding: 30
                
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                
                TextField {
                    id: username 
                    hintText: "Username"
                    inputMode: TextFieldInputMode.EmailAddress                
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                    }   
                }
                
                TextField {
                    id: password
                    hintText: "Password"
                    inputMode: TextFieldInputMode.Password
                    textStyle {
                        base: SystemDefaults.TextStyles.BodyText
                        color: Color.Black
                    }
                }
                
                Button {
                    id: login                
                    text: "Login"
                    horizontalAlignment: HorizontalAlignment.Fill
                    onClicked: {              
                    }
                }
            }
        }
    }
}