#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
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
        
    ofBackground(0,0,0);
    ofSetVerticalSync(true);
    
    dir.listDir("scenes");
    dir.allowExt("mov");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order
    
    //allocate the vector to have as many ofImages as files
    if( dir.size() ){
        sceneNames.assign(dir.size(), (""));
    }
    
    // you can now iterate through the files and load them into the ofImage vector
    for(int i = 0; i < (int)dir.size(); i++){
        sceneNames[i] = (dir.getPath(i));
        playList.push_back((string)dir.getPath(i));
        ofLog(OF_LOG_NOTICE, "scene name " + sceneNames[i]);
    }
    currentScene = "";
    currentSceneIndex = 0;
    
    
    videoA.load(sceneNames[currentSceneIndex]);
    videoA.setLoopState(OF_LOOP_NORMAL);
    videoA.play();
    
    changeVideoFlag = 0;
    currentVideoPlayer = 'A';

}

//--------------------------------------------------------------
void ofApp::update(){
    
    
    
    if (videoA.isLoaded()) {
        ofLog(OF_LOG_NOTICE, "A frame time duration %d %d %f %f", videoA.getCurrentFrame(), videoA.getTotalNumFrames(), videoA.getPosition(), videoA.getDuration());
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
        ofLog(OF_LOG_NOTICE, "B frame time duration %d %d %f %f", videoB.getCurrentFrame(), videoB.getTotalNumFrames(), videoB.getPosition(), videoB.getDuration());
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
    
    
    ofLog(OF_LOG_NOTICE, "player name %c", currentVideoPlayer);
    
    //    if (currentVideoPlayer == 'A') {
    //        if (videoA.isLoaded()) {
    //            videoA.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
    //        }
    //    }
    //    else {
    //        if (videoB.isLoaded()) {
    //            videoB.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
    //        }
    //    }
    //
    //    if (changeVideoFlag) {
    //        if (currentVideoPlayer == 'A') {
    //            if (videoB.isLoaded()) {
    //                videoB.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
    //                changeVideoFlag = 0;
    //                videoA.close();
    //                currentVideoPlayer = 'B';
    //            }
    ////            else
    ////                videoA.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
    //        } else {
    //            if (videoA.isLoaded()) {
    //                videoA.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
    //                changeVideoFlag = 0;
    //                videoB.close();
    //                currentVideoPlayer = 'A';
    //            }
    ////            else
    ////                videoB.draw(videoX,videoY,resizedVideoWidth, resizedVideoHeight);
    //        }
    //    }
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
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

//--------------------------------------------------------------
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
