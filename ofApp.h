#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{
    
public:
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofVideoPlayer         videoA, videoB;

    ofDirectory dir;
    vector<string> sceneNames;
    string currentScene;
    
    list<string> playList;
    
    int currentSceneIndex;
    
    int screenWidth, screenHeight, videoWidth, videoHeight;
    float videoX, videoY, resizedVideoWidth, resizedVideoHeight;
    
    bool changeVideoFlag, pleaseCloseA;
    char currentVideoPlayer;
    
    void changeVideo (string newVideo);
};

