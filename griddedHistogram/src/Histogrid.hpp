//
//  Histogrid.hpp
//  griddedHistogram
//
//  Created by Tyler on 3/31/16.
//
//

#pragma once
#include "ofMain.h"

#include "ofxOpenCv.h"
#include "ofxCv.h"

using namespace cv;
using namespace ofxCv;

class Histogrid {
    
public:
    
    Histogrid();
    Histogrid(ofImage& _img, int _nDivsX = 10, int _nDivsY = 10, int _nBins = 256);
    
    void run();
    void draw(int n, ofColor color = ofColor::white);
    void draw(int n, float x, float y, float w, float h, ofColor color = ofColor::white);
    void drawMat();
    
    const vector<vector<float>>& getHistograms() const;
    const vector<float>& getHistogram(int n) const;
    int getNDivsX();
    int getNDivsY();
    int getNBins();
    const vector<ofRectangle>& getRectangles() const;
    
private:
    
    int nDivsX, nDivsY, nBins;
    ofImage* img;
    Mat imgMat;
    
    vector<vector<float>> histograms;
    vector<ofRectangle> rects;
    
};
