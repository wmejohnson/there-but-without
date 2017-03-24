#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(30);
    ofDisableDepthTest();
    ofEnableAlphaBlending();
    
    video1.load("videos/1.mp4");
    video1.setLoopState(OF_LOOP_NORMAL);
    video1.setVolume(1);
    
    video2.load("videos/2.mp4");
    video2.setLoopState(OF_LOOP_NORMAL);
    video2.setVolume(1);

    video3.load("videos/3.mp4");
    video3.setLoopState(OF_LOOP_NORMAL);
    video3.setVolume(1);
    
    //GUI
    gui.setup();
    gui.add(record.set("record", false));
    
    blurGroup.setName("blur");
    blurGroup.add(iterations.set("iterations", 6, 1, 100));
    blurGroup.add(distance.set("distance", 11, 2, 100));
    gui.add(blurGroup);
    
    noiseControls.setName("noise");
    noiseControls.add(noiseMaskSpeed.set("speed", 0.005, 0.001, 0.1));
    gui.add(noiseControls);
    
    videoControls.setName("video");
    videoControls.add(videoSpeed.set("speed", 0.6, 0.1, 2.0));
    videoControls.add(videoBrightness.set("brightness", 1.0, 0.0, 1.2));
    videoControls.add(noiseAmount.set("noiseAmt", 0.0, 0.0, 1.0));
    videoControls.add(noiseSpeed.set("noiseSpd", 8.0, 0.0, 30.));
    videoControls.add(flicker.set("flicker", false));
    gui.add(videoControls);
    drawGUI = true;

    //Shader Setup
    blur.load("shaders/blur");
    noise.load("shaders/noise");
    mask.load("shaders/mask");
    
    //Fbo Setup
    videoFrame.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    blurredFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    maskFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    finalFbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    
    videoFrame.begin();
    ofBackground(0);
    videoFrame.end();
    
    blurredFbo.begin();
    ofBackground(0);
    blurredFbo.end();
    
    maskFbo.begin();
    ofBackground(0);
    maskFbo.end();
    
    finalFbo.begin();
    ofBackground(0);
    finalFbo.end();
    
    //State
    state = 0;
    
    //play video
    video1.play();
    video2.play();
    video3.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    video1.setSpeed(videoSpeed);
    video1.update();
    video2.setSpeed(videoSpeed);
    video2.update();
    video3.setSpeed(videoSpeed);
    video3.update();
    
    videoFrame.begin();
    video1.draw(0, 0, ofGetWidth(), ofGetHeight()/3);
    video2.draw(0, ofGetHeight()/3, ofGetWidth(), (ofGetHeight()/3)*2);
    video3.draw(0, (ofGetHeight()/3)*2, ofGetWidth(), ofGetHeight());
    videoFrame.end();
    
    //blur the video
    blurredFbo.begin();
    ofClear(255, 255, 255, 0);
    blur.begin();
    blur.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    blur.setUniform1i("u_interations", iterations);
    blur.setUniform1i("u_dist", distance);
    videoFrame.draw(0, 0);
    blur.end();
    blurredFbo.end();
    
    
    //create the mask
    maskFbo.begin();
    ofClear(255, 255, 255, 0);
    noise.begin();
    noise.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
    noise.setUniform1f("u_time", ofGetElapsedTimef());
    noise.setUniform1f("u_speed", noiseMaskSpeed);
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    noise.end();
    maskFbo.end();
    
    
    //write the final fbo
    finalFbo.begin();
    ofClear(255, 255, 255, 0);
    ofBackground(255);
    
    mask.begin();
    
    video1.draw(0, 0, ofGetWidth(), ofGetHeight());
    if(flicker){
        mask.setUniform1f("u_bright", (videoBrightness-(ofNoise(noiseSpeed*ofGetElapsedTimef()))*noiseAmount)*((int)(ofRandom(1.0)>0.9)));
    } else {
        mask.setUniform1f("u_bright", (videoBrightness-(ofNoise(noiseSpeed*ofGetElapsedTimef()))*noiseAmount));
    }
    mask.setUniformTexture("video", videoFrame.getTexture(), 3);
    mask.setUniform1i("u_white", state);
    mask.setUniformTexture("blurred", blurredFbo.getTexture(), 1);
    mask.setUniformTexture("mask", maskFbo.getTexture(), 2);

    mask.end();
    finalFbo.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));

    finalFbo.draw(0 , 0, ofGetWidth(), ofGetHeight());
    
    if(ofGetFrameNum() < 300 && record) {
        //set the length of the recrording
        //before the gui draw so it doesn't matter
        //if i have it up or not :)
        ofSaveFrame();
    }
    
    if(drawGUI) gui.draw();
}

void ofApp::close(){
    //de allocate some memoryyy (do i need to do this in c++?)
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            windowResized(ofGetWidth(), ofGetHeight());
            break;
        case 'g':
            drawGUI = !drawGUI;
            break;
        case 'S':
            ofSaveScreen("memory"+ofGetTimestampString()+".png");
            break;
        case 'q':
            state = 0;
            break;
        case 'w':
            state =1;
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
