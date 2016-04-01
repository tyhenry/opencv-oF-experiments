#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    
    img.load("img.jpg");
    
    imgXY = ofVec2f(0,0); // placement of image for draw
    imgWH = ofVec2f(640,480); // scale of image for draw
    
    nDivsX = 2; // # of grid divisions on x-axis
    nDivsY = 2; // # of divs on y-axis
    
    hist = Histogrid(img,nDivsX,nDivsY,256); //img, nDivs x,y, nBins
    
    hist.run(); // calculate the histogram
    
    rects = hist.getRectangles(); // get the grid as a vector of rectangles
    
    drawGrid = ofRectangle(0,0,0,0); // rectangle overlay for selected subsection

}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    img.draw(imgXY.x,imgXY.y, imgWH.x,imgWH.y); // draw image
    
    hist.draw(gridNum, imgWH.x,imgXY.y, imgWH.x,imgWH.y); // draw histogram
    // gridNum = which section to draw histogram for
    // other options: x,y, w,h for draw (here, draw next to image)
    
    
    // draw selected grid chunk
    
    ofPushMatrix();
    ofTranslate(imgXY);
    ofScale(imgWH.x/img.getWidth(),imgWH.y/img.getHeight());
    ofPushStyle();
    ofFill();
    ofSetColor(255,255,0,100);
    ofDrawRectangle(drawGrid); // draw selected rect
    ofPopStyle();
    ofPopMatrix();

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    // if mouse is over image
    ofRectangle rect(imgXY,imgWH);
    
    if (rect.inside(ofVec2f(x,y))){
        
        // figure out which rectangle the mouse is in
        
        float mX = ofMap(x, imgXY.x, imgWH.x, 0, img.getWidth());
        float mY = ofMap(y, imgXY.y, imgWH.y, 0, img.getHeight());
        
        for (int i=0; i<rects.size(); i++){
            
            if (rects[i].inside(mX,mY)){
                gridNum = i;
                drawGrid = rects[i];

                break;
            }
        }
    }

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
