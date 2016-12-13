#pragma once

#include "ofMain.h"
#include "Settings.h"
#include "ofxPiMapper.h"
#include "CustomSource.h"
#include "CrossSource.h"
#include "VideoSource.h"
#include "ofxOscParameterSync.h"
#include "ofxXmlSettings.h"

class ofApp : public ofBaseApp {
	public:
		void setup();
		void draw();
		void update();
		void setupButtons();
        bool checkPadPressed();
        void playVideos();
        void stopVideos();
        void keyPressed(int key);
        void TogglePlaying(bool & val);
        void loadSettings();

		ofxPiMapper piMapper;

		// By using a custom source that is derived from FboSource
		// you will be able to see the source listed in sources editor
		CustomSource * customSource;
		CrossSource * crossSource;

        bool bPlaying;
        bool bIsPressed;

        ofParameterGroup parameters;
        ofParameter<bool>   bTogglePlaying;
        ofxOscParameterSync sync;

        ofxXmlSettings  xml;
        int gServerPort;
        int gClientPort;
};
