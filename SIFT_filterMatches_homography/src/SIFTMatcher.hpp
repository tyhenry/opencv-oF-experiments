//
//  SIFTMatcher.hpp
//  SIFT_filterMatches_homography
//
//  Created by Tyler on 3/24/16.
//
//

#pragma once 
#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"

// include non-free OpenCV modules
#include "opencv2/nonfree/nonfree.hpp"

using namespace cv;
using namespace ofxCv;

class SIFTMatcher {
    
public:
    
    SIFTMatcher();
    
    SIFTMatcher(ofImage& _findImg, ofImage& _fieldImg);
    
    void match();
    // returns vector of keypoint matches between "query" image (_findImg) and "train" image (_fieldImg)
    
    void filterMatches();
    // filters outliers in matches vector based on distance
    
    void drawMatchesCv(ofImage& _matchImg, bool bUseGoodMatches = false);
    // draws match visualization into _matchImg
    // bUseGoodMatches draws matches vector if false or goodMatches vector if true
    
    void getHomography(bool bUseGoodMatches = false);
    // calculates homography between matched keypoints
    // and transforms corners of findImg to match coordinates in fieldImg
    
    void drawHomography(float xOffset = 0, float yOffset = 0, ofColor color = ofColor::cyan, float lineWidth = 3);
    // draws warped box in fieldImg coordinates of where findImg was found
    // x and yOffset draw
    
    ofImage* findImg;
    ofImage* fieldImg;
    
    vector<KeyPoint> findKeypoints, fieldKeypoints;
    vector<DMatch> matches;
    vector<DMatch> goodMatches;
    
    vector<ofVec2f> fieldCorners; // stores corners of findImg transformed into fieldImg space
    
    
};
