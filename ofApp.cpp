#include "ofApp.h"
#include "ofxButton.h"




void ofApp::changeVideo(string newVideo) {
    if (currentSceneIndex == 0) {
        currentSceneIndex = (int)sceneNames.size() - 1;
    } else {
        currentSceneIndex --;
    }
    //            videoB.load(sceneNames[currentSceneIndex]);
    //            videoB.play();
    if (currentVideoPlayer=='A') {
        videoB.load(sceneNames[currentSceneIndex]);
        videoB.play();
    } else {
        videoA.load(sceneNames[currentSceneIndex]);
        videoA.play();
    }
    changeVideoFlag = 1;
}

//--------------------------------------------------------------
void ofApp::setup(){

    
//    buttonListener = buttonParams.newListener([this](const void * sender){
//        ofParameter<void> * p = (ofParameter<void> *)sender;
//        string name = p->getName();
//        cout << name << endl;
//    });
    
    
    //local video stuff
    screenWidth = ofGetScreenWidth();
    screenHeight = ofGetScreenHeight();
    ofSetWindowShape(screenWidth, screenHeight);
    ofSetFrameRate(23.976);
    
    videoWidth = 1280;
    videoHeight = 720;
    
    //define the video position relative to the screen + resize in a way it won't get distorted
    if (((float)screenWidth / (float)videoWidth * videoHeight) > screenHeight) {
        
        resizedVideoWidth = ((float)screenHeight / (float)videoHeight) * videoWidth;
        resizedVideoHeight = screenHeight;
        videoY = 0;
        videoX = (screenWidth - resizedVideoWidth) / 2;
        
        
    } else {
        
        resizedVideoHeight = ((float)screenWidth / (float)videoWidth)  * videoHeight;
        resizedVideoWidth = screenWidth;
        videoX = 0;
        videoY = ( screenHeight - resizedVideoHeight ) / 2 ;
    }
    
    
    ofSetVerticalSync(true);
    
    if (!dir.listDir("scenes")) {
        statusMessage = "Scenes folder not found. Create a folder named scenes in the data folder and restart";
    } else {
        dir.allowExt("mov");
        dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
        
        //allocate the vector to have as many ofImages as files
        if( dir.size() ){
            sceneNames.assign(dir.size(), (""));
            // you can now iterate through the files and load them into the ofImage vector
            for(int i = 0; i < (int)dir.size(); i++){
                sceneNames[i] = (dir.getPath(i));
                ofVideoPlayer temp;
                temp.load(sceneNames[i]);
                string sceneName = sceneNames[i].substr(7);
                int pos = sceneName.find('.');
                
                sceneDurations.insert ( pair<string, float> (sceneName.substr(0,pos), temp.getDuration()));
//                cout << sceneName.substr(0,pos) << " " << temp.getDuration() << endl;
                playList.push_back((string)dir.getPath(i));
                
            }
        } else {
            statusMessage = "No video files found in the scenes folder. Please add .mov files and restart";
        }
        
        currentScene = "";
        currentSceneIndex = 0;
        
    
        changeVideoFlag = 0;
        currentVideoPlayer = 'A';
    }
    
    
    ofSetBackgroundColor(200);

    //online scripts stuff
    messageFont.load("mono.ttf", 10);
    
    //the string is printed at the top of the app
    //to give the user some feedback
    statusMessage = "loading mySettings.xml";
    
    if( xmlSettings.loadFile("mySettings.xml") ){
        statusMessage = "mySettings.xml loaded!";
    }else{
        statusMessage = "unable to load mySettings.xml check data/ folder";
    }
    
    string token  = xmlSettings.getValue("TOKEN", "");
    
    
    ofHttpRequest request;
    
    
    if (token == "") { // use password to get new token
        string password = xmlSettings.getValue("PASSWORD", "");
        string user = xmlSettings.getValue("USER", "");
        
        if ((user =="")||(password=="")) {
            statusMessage = "couldn't find token or password on settings. \nplease create <TOKEN>token</TOKEN><USER>user</USER><PASSWORD>password</PASSWORD> and restart";
        } else {
            request.method = ofHttpRequest::POST;
            request.contentType = "application/json";
            request.url = "dispersao-scripts-api.herokuapp.com/authentication";
            request.body = "\n{\n\t\"email\":\"";
            request.body.append(user);
            request.body.append("\",\n\t\"password\":\"");
            request.body.append(password);
            request.body.append("\",\n\t\"strategy\":\"local\"}");
            
            auto response = http.handleRequest(request);
            if ((response.status == 200)||(response.status == 201)||(response.status == 204)) {
                responseJson = nlohmann::json::parse(response.data.getText())  ;
                auto responseToken = responseJson["accessToken"];
                if (responseToken == nullptr) {
                    statusMessage = "Invalid credentials (password or user). Update settings and restart. ";
                } else {
                    token = responseToken.dump();
                    token.erase(remove( token.begin(), token.end(), '\"' ),token.end());
                }
            } else {
                statusMessage = "Error :";
                statusMessage.append(to_string(response.status));
                statusMessage.append("\n");
                statusMessage.append(response.error);
            }
        }
        
    }
    
    if (token != "") {
        request.method = ofHttpRequest::GET;
        request.contentType = "application/json";
        request.headers["Authorization"] = "Bearer ";
        request.headers["Authorization"].append(token);
        request.body = "";
        
        request.url = "dispersao-scripts-api.herokuapp.com/scripts";
        
        auto response = http.handleRequest(request);
        
        if ((response.status == 200)||(response.status == 201)||(response.status == 204)) {

            
            responseJson = nlohmann::json::parse(response.data.getText())  ;
            
            //    for(auto & j: responseJson){
            //        cout << j << endl;
            //    }
            
            //scriptJson = nlohmann::json::parse(responseJson["data"].dump());
            scriptJson = responseJson["data"];
            
            statusMessage = "Loaded " ;
            statusMessage.append(to_string(scriptJson.size()));
            statusMessage.append(" scripts");
            
            numScripts = (int)scriptJson.size();
            buttons.resize(numScripts);
            gui.setup("load film", "guisettings.xml",550,30);
            for (int i=0;i<numScripts;i++) {
                // TO DO!
//                buttons[i].addListener(this, &ofApp::buttonPressed);
                gui.add(buttons[i].setup(to_string(i+1),40,20));
            }
            
            //now we get the list of file names
            request.method = ofHttpRequest::GET;
            request.contentType = "application/json";
            request.headers["Authorization"] = "Bearer ";
            request.headers["Authorization"].append(token);

            request.body = "";
            
            request.url = "dispersao-scripts-api.herokuapp.com/sequences";
            
            auto response = http.handleRequest(request);
            
            if ((response.status == 200)||(response.status == 201)||(response.status == 204)) {

                ofJson sequencesJson = nlohmann::json::parse(response.data.getText())  ;
                
                scriptSceneNames.assign(sequencesJson["data"].size(),"");
                for (auto& element : sequencesJson["data"]) {
                    int id = element["id"];
                    string s = element["sceneNumber"];
                    if (s.size()==1) {
                        s.insert(0,"00");
                    } else
                        if (s.size()==2) {
                            if (isalpha(s[1])) {
                                s.insert(0,"00");
                            } else {
                                s.insert(0,"0");
                            }
                        } else
                            if (s.size()>2) {
                                s.insert(0,"0");
                            }
                    scriptSceneNames[id] = s;
                }
                
                
                
                //create playlists from scripts
                for (int i=0;i<numScripts;i++) {
                    ofJson sequences = scriptJson[i]["sequences"];
                    
                    float duration = 0.0;
                    for (int j=0;j<sequences.size();j++) {
                        string sceneName;
                        sceneName = scriptSceneNames[sequences[j]];
                        duration += sceneDurations[sceneName];
                    }
                    
                    scriptDurations.push_back(duration);
                    
                }

                
            } else {
                statusMessage = "Error :";
                statusMessage.append(to_string(response.status));
                statusMessage.append("\n");
                statusMessage.append(response.error);
            }
            
            
            
            
        } else {
            statusMessage = "Error :";
            statusMessage.append(to_string(response.status));
            statusMessage.append("\n");
            statusMessage.append(response.error);
        }
    }
    
  
}

//--------------------------------------------------------------
void ofApp::update(){
    if (videoA.isLoaded()) {
        
        if (videoA.getCurrentFrame()>(videoA.getTotalNumFrames()-2)) {
            if (playList.size()>0) {
                changeVideo(playList.front());
                playList.pop_front();
            } else {
                ofLog(OF_LOG_ERROR, "Empty playlist");
            }
        }
        videoA.update();
    }
    
    if (videoB.isLoaded()) {
        
        videoB.update();
        
        if (videoB.getCurrentFrame()>(videoB.getTotalNumFrames()-2)) {
            if (playList.size()>0) {
                
                changeVideo(playList.front());
                playList.pop_front();
            } else {
                ofLog(OF_LOG_ERROR, "Empty playlist");
            }
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if (currentVideoPlayer=='A') {
        videoA.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
        if (videoB.isLoaded()&&(videoB.isPlaying())) {
            videoB.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
            //            changeVideoFlag = 0;
            videoA.stop();
            currentVideoPlayer = 'B';
        }
    } else {
        videoB.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
        if (videoA.isLoaded()&&(videoA.isPlaying())) {
            videoA.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
            //            changeVideoFlag = 0;
            videoB.stop();
            currentVideoPlayer = 'A';
        }
    }
 
    if (!hideGui) {

        ofSetColor(24, 24, 24);
        messageFont.drawString("status: "+statusMessage, 10, 30);


        if (numScripts > 0) {

            messageFont.drawString("ID", 30, 50);
            messageFont.drawString("Duration", 50, 50);
            messageFont.drawString("N. seq", 150, 50);
            messageFont.drawString("Author", 250, 50);
            messageFont.drawString("Name", 350, 50);

            for (int i=0;i<numScripts;i++) {
                int gap = 20;
                messageFont.drawString(to_string(i+1), 30, (65+(i*gap)));
                int minutes = (int)(scriptDurations[i]/60.0f);
                string durationText = to_string(minutes);
                durationText.append(":");
                durationText.append(to_string((int)(scriptDurations[i]-(60.0f*minutes))));
                messageFont.drawString(durationText, 50, (65+(i*gap)));
                int numSequences = scriptJson[i]["sequences"].size();
                messageFont.drawString(to_string(numSequences), 150, (65+(i*gap)));
                string author = scriptJson[i]["author"];
                author.erase(remove( author.begin(), author.end(), '\"' ),author.end());
                messageFont.drawString(author, 250, (65+(i*gap)));
                string name = scriptJson[i]["name"];
                name.erase(remove( name.begin(), name.end(), '\"' ),name.end());
                messageFont.drawString(name, 350, (65+(i*gap)));

            }
        }
        
        gui.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case OF_KEY_RIGHT:
            //fingerMovie.previousFrame();
            if (currentSceneIndex == (int)sceneNames.size()-1) {
                currentSceneIndex =0;
            } else {
                currentSceneIndex++;
                
            }
            //            videoB.load(sceneNames[currentSceneIndex]);
            //            videoB.play();
            if (currentVideoPlayer=='A') {
                videoB.load(sceneNames[currentSceneIndex]);
                videoB.play();
            } else {
                videoA.load(sceneNames[currentSceneIndex]);
                videoA.play();
            }
            changeVideoFlag = 1;
            hideGui = true;
            break;
        case OF_KEY_LEFT:
            //fingerMovie.nextFrame();
            if (currentSceneIndex == 0) {
                currentSceneIndex = (int)sceneNames.size() - 1;
            } else {
                currentSceneIndex --;
            }
            //            videoB.load(sceneNames[currentSceneIndex]);
            //            videoB.play();
            if (currentVideoPlayer=='A') {
                videoB.load(sceneNames[currentSceneIndex]);
                videoB.play();
            } else {
                videoA.load(sceneNames[currentSceneIndex]);
                videoA.play();
            }
            changeVideoFlag = 1;
            hideGui = true;
            break;
        case OF_KEY_RETURN:
            videoA.load(sceneNames[currentSceneIndex]);
            videoA.setLoopState(OF_LOOP_NORMAL);
            videoA.play();
            hideGui = true;
            break;
        case '0':
            //fingerMovie.firstFrame();
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//TO DO !
//void ofApp::buttonPressed(const void * sender) {
//    string name = (*(ofxButton*) sender).getName();
//    // ofxButton button = *(ofxButton*) sender;
//    //    cout << button.getName() << endl;
//    // string name = button.getName();
//
//}
