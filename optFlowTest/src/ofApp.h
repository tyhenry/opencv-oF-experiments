#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxCv.h"

#include "ofxGui.h"

using namespace cv;
using namespace ofxCv;

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    
    void reset();

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
    
    ofVideoPlayer vid;
    float vidW, vidH;
    
    FlowFarneback flow; // ofxCv's dense optical flow analyzer (for whole image)
    // alternative would be FlowPyrLK which detects/analyzes a sparse feature set
    
    vector<ofVec2f> frameFlows; // store average flow frame-by-frame
    ofVec2f vidFlowTotal = ofVec2f(0,0), vidFlowAvg = ofVec2f(0,0); // store average flow of video
    
    // moving dot
    ofVec2f dotPos;
    ofPolyline dotPath; // polyline tracks dot movement
    
    
    // gui options - stolen from ofxCv example-flow
    ofxPanel gui;
    ofParameter<float> flowPyrScale, flowPolySigma, vidScale, lineWidth;
    ofParameter<int> flowLevels, flowIterations, flowPolyN, flowWinSize;
    ofParameter<bool> flowUseGaussian, loopVid;
		
};
