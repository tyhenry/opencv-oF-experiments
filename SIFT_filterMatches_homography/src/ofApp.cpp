#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // load the two images as ofImage
    
    findImg.load("peeping_tom_crop2-3d.jpg"); // image we're looking for (called "query" in OpenCV)
    // for findImg here:
    // I'm using an image I cropped as a trapezoid from the fieldImg,
    // and then warped back to be a square in Photoshop
    // (the original crop is "peeping_tom_crop2.jpg")
    
    fieldImg.load("peeping_tom_scene.jpg"); // image to search in (called "train" in OpenCV)
    
    // run sift matcher
    
    siftMatcher = SIFTMatcher(findImg, fieldImg); // construct SIFTMatcher object
    
    siftMatcher.match(); // find matches
    
    siftMatcher.filterMatches(); // filter matches
    
    siftMatcher.drawMatchesCv(matchImg, true); // draw good matches into matchImg
    
    siftMatcher.getHomography(true); // calculate homography between (good) matched keypoints
                                 // and calc corresponding transformation on findImg
    
    
    // load the pre-warped cropped image for reference in ofApp::draw()
    refImg.load("peeping_tom_crop2.jpg");
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    // draw matchImg
    matchImg.draw(0,0);
    
    /*
    // sidenote on drawing Mats:
    // -------------------------
    // if we had declared matchMat (from setup()) as a public variable in ofApp.h,
    // we could draw it in ofApp::draw() using ofxCv's drawMat():
    //   drawMat(matchMat,0,0);
    // but this is expensive since drawMat() allocates a new GPU texture every time it's called.
    // it's best to convert to ofImage, so the texture is just allocated once on the GPU
    */
    
    // draw box around findImg inside fieldImg
    siftMatcher.drawHomography(findImg.getWidth());
        //offset by findImg width to correspond to matchImg
    
    
    // draw the original crop from the image, before I warped it in photoshop
    refImg.draw(0,ofGetHeight()-refImg.getHeight());
    string refImgLbl = "Original Crop for Reference";
    ofDrawBitmapString(refImgLbl, 10, ofGetHeight()-refImg.getHeight()-20);
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
