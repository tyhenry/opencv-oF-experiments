#pragma once

#include "ofMain.h"

#include "Histogrid.hpp"

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

    ofImage img;
    Histogrid hist;
    int nDivsX;
    int nDivsY;
    ofVec2f imgXY;
    ofVec2f imgWH;
    
    int gridNum = 0;
    ofRectangle drawGrid;
    
    vector<ofRectangle> rects;
		
};
