#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(40);
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    video.load("videos/2.mp4");
    video.setLoopState(OF_LOOP_NORMAL);
    video.setVolume(0.0);
    video.play();

    
    //GUI
    gui.setup();
    drawGUI = false;

    //Shader Setup
    blur.load("shaders/blur");
    noise.load("shaders/noise");
    mask.load("shaders/mask");
    
    //Fbo Setup
    blurredFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    finalFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    blurredFbo.begin();
    ofBackground(0);
    blurredFbo.end();
    
    maskFbo.begin();
    ofBackground(0);
    maskFbo.end();
    
    finalFbo.begin();
    ofBackground(0);
    finalFbo.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    video.setSpeed(0.2);
    video.update();
    
    //blur the video
    blurredFbo.begin();
    ofClear(255, 255, 255, 0);
    blur.begin();
    blur.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    video.draw(0, 0, ofGetWidth(), ofGetHeight());
    blur.end();
    blurredFbo.end();
    
    
    //create the mask
    maskFbo.begin();
    ofClear(255, 255, 255, 0);
    noise.begin();
    noise.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    noise.setUniform1f("u_time", ofGetElapsedTimef());
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    noise.end();
    maskFbo.end();
    
    
    //write the final fbo
    finalFbo.begin();
    ofClear(255, 255, 255, 0);
    ofBackground(255);
    
    mask.begin();

    video.draw(0, 0, ofGetWidth(), ofGetHeight());
    mask.setUniformTexture("video", video.getTexture(), 3);
    mask.setUniformTexture("blurred", blurredFbo.getTexture(), 1);
    mask.setUniformTexture("mask", maskFbo.getTexture(), 2);

    mask.end();
    finalFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    finalFbo.draw(0, 0, ofGetWidth(), ofGetHeight());

    if (drawGUI) gui.draw();
}

void ofApp::close(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'g':
            drawGUI = !drawGUI;
            break;
        case 'S':
            ofSaveScreen("memory"+ofGetTimestampString()+".png");
            break;
        default:
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
    blurredFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    finalFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    blurredFbo.begin();
    ofBackground(0);
    blurredFbo.end();
    
    maskFbo.begin();
    ofBackground(0);
    maskFbo.end();
    
    finalFbo.begin();
    ofBackground(0);
    finalFbo.end();
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
