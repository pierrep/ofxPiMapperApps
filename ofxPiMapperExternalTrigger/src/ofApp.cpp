#include "ofApp.h"

#if defined(TARGET_RASPBERRY_PI)
#include "wiringPi.h"
#endif

#define PAD1 0

void ofApp::setup(){
    ofBackground(0);

    // Enable or disable audio for video sources globally
    // Set this to false to save resources on the Raspberry Pi
    ofx::piMapper::VideoSource::enableAudio = true;

    // Add our CustomSource to list of fbo sources of the piMapper
    // FBO sources should be added before piMapper.setup() so the
    // piMapper is able to load the source if it is assigned to
    // a surface in XML settings.
    crossSource = new CrossSource();
    customSource = new CustomSource();
    piMapper.registerFboSource(*crossSource);
    piMapper.registerFboSource(*customSource);
    piMapper.setup();

    ofSetFullscreen(Settings::instance()->getFullscreen());

    setupButtons();
    bPlaying = false;
    bIsPressed = false;

    loadSettings();

    parameters.setName("parameters");
    parameters.add(bTogglePlaying.set("Toggle Playing",true));
    bTogglePlaying.addListener(this,&ofApp::TogglePlaying);
    //sync.setup(parameters,6667,"localhost",6666);
    sync.setup(parameters,gServerPort,"localhost",gClientPort);

    stopVideos();
}

void ofApp::draw(){
    piMapper.draw();
}

void ofApp::update() {
    //    bIsPressed = checkPadPressed();

    sync.update();

    if(bIsPressed && !bPlaying) {
        bPlaying = true;
        bTogglePlaying = !bTogglePlaying;
    }

    if(bPlaying && !bIsPressed) {
        bTogglePlaying = !bTogglePlaying;
        bPlaying = false;
    }

}

void ofApp::playVideos() {
    piMapper.play();
}

void ofApp::stopVideos() {

    piMapper.stop();
}

void ofApp::keyPressed(int key)
{
    if(key == '0') {
        bTogglePlaying = !bTogglePlaying;
    }
    if(key == '/') {
        bIsPressed = !bIsPressed;
    }
}

void ofApp::TogglePlaying(bool & val)
{
    ofLogNotice() << "TogglePLaying() : val = " << val;
    if(val == 0) {
        playVideos();
    } else {
        stopVideos();
    }
}

void ofApp::setupButtons()
{
    #if defined(TARGET_RASPBERRY_PI)

    if(wiringPiSetup() < 0) {
        ofLogError() << "Failed to init WiringPi Lib";
    }

    pinMode(PAD1,INPUT); // pin 17
    pullUpDnControl(PAD1,PUD_UP);

    #endif
}

bool ofApp::checkPadPressed()
{
    #if defined(TARGET_RASPBERRY_PI)

    if(digitalRead(PAD1 == 0)) {
        cout << "Pad pressed!" << endl;
        return true;
    }
    #endif

    return false;
}

void ofApp::loadSettings()
{
    ofLogNotice() << "Loading Settings...";
    xml.load("Settings.xml");

    gServerPort = xml.getValue("Settings:ServerPort", 9999);
    gClientPort = xml.getValue("Settings:ClientPort", 9998);
    ofLogNotice() << "Server Port = " << gServerPort;
    ofLogNotice() << "Client Port = " << gClientPort;

}
