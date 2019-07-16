#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxGui.h"

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
        vector<string> scriptSceneNames;
        map<string, float> sceneDurations;
        vector<float> scriptDurations;
    
        string currentScene;
        
        list<string> playList;
    
        int currentSceneIndex;
        
        int screenWidth, screenHeight, videoWidth, videoHeight;
        float videoX, videoY, resizedVideoWidth, resizedVideoHeight;
        
        bool changeVideoFlag, pleaseCloseA;
        char currentVideoPlayer;
        
        void changeVideo (string newVideo);
        
        ofxXmlSettings xmlSettings;
        ofTrueTypeFont messageFont;
    
//      TO DO
//        ofParameter<void> buttonParams{"here"};
//
//        ofParameterGroup parameters{
//            "settings",
//            buttonParams,
//        };
//        ofxPanel gui{parameters};
        ofxPanel gui;
        ofEventListener buttonListener;
    
    
        bool hideGui = false;
        vector<ofxButton> buttons;
        void buttonPressed(const void * sender);
        string statusMessage;
        
        ofURLFileLoader http;
        ofJson responseJson, scriptJson;
        
        int numScripts = 0;
		
};
