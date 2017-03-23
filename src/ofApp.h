#pragma once

#include "ofMain.h"
#include "ofxGUI.h"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void close();

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
    
    //State Control
    //0 is default
    //1 is blur & white
    int state;
    
    // Video
    ofVideoPlayer video;
    bool record;
    
    // Shaders
    ofShader blur;
    ofShader noise;
    ofShader mask;
    
    // GUI
    ofxPanel gui;
    ofParameterGroup blurGroup;
    ofParameter<int> iterations;
    ofParameter<int> distance;
    
    ofParameterGroup videoControls;
    ofParameter<float> videoSpeed;
    ofParameter<float> videoBrightness;
    ofParameter<float> noiseAmount;
    ofParameter<float> noiseSpeed;
    ofParameter<bool> flicker;
    
    ofParameterGroup noiseControls;
    ofParameter<float> noiseMaskSpeed;
    
    bool drawGUI;
    
    // Fbo's
    ofFbo blurredFbo;
    ofFbo maskFbo;
    ofFbo finalFbo;
};
