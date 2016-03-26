#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0);
    ofSetFrameRate(60);
    ofFill();
    
    vid.load("rearWindow_clip_1-240p.mp4"); // a single shot from Rear Window
    vidW = vid.getWidth();
    vidH = vid.getHeight();
    
    // setup gui
    gui.setup();
    gui.add(flowPyrScale.set("Pyramid Scale", .5, 0, .99));
    gui.add(flowLevels.set("Pyramid Levels", 4, 1, 8));
    gui.add(flowIterations.set("Iterations", 2, 1, 8));
    gui.add(flowPolyN.set("Polynomial size", 7, 5, 10));
    gui.add(flowPolySigma.set("PolySigma", 1.5, 1.1, 2));
    gui.add(flowUseGaussian.set("Use Gaussian", true));
    gui.add(flowWinSize.set("Window size", 32, 4, 64));
    gui.add(loopVid.set("Loop video", false));
    gui.add(vidScale.set("Scale video", 2, 1, 3));
    gui.add(lineWidth.set("Draw line width", 4, 1, 10));
    
    // start video
    vid.play();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if (vid.getCurrentFrame() == 0) {
        reset();
        // will only do full reset if video is at beginning
        // doubles as an initialization
    }
    
    vid.update();
    
    if (loopVid){
        vid.setLoopState(OF_LOOP_NORMAL);
        
        if (!vid.isPlaying()){
            vid.play(); // restart vid if it ended
        }
        
        
    } else {
        vid.setLoopState(OF_LOOP_NONE);
        
        if (vid.getIsMovieDone()){
            flow.resetFlow(); // reset flow cache in ofxCv to clear flow draw
        }
    }

    
    if (vid.isFrameNew()){
        
        // Farneback flow parameters
        // more info at: http://docs.opencv.org/3.0-beta/modules/video/doc/motion_analysis_and_object_tracking.html#calcopticalflowfarneback
        
        flow.setPyramidScale(flowPyrScale);
            // 0.5 is classical image pyramid (scales image * 0.5 each layer)
        
        flow.setNumLevels(flowLevels); // number of levels of scaling pyramid
            // 1 means only original image is analyzed, no pyramid
        
        flow.setWindowSize(flowWinSize); // average window size
            // larger means robust to noise + better fast motion, but more blurred motion
        
        flow.setNumIterations(flowIterations); // # iterations of algorithm at each pyramid level
        
        flow.setPolyN(flowPolyN);
            // size of pixel neighborhood used for per-pixel polynomial expansion
            // larger means image will be approximated at smooth surfaces, more robust algorithm, more blurred motion
        
        flow.setPolySigma(flowPolySigma);
            // standard deviation of gaussian used to smooth derivates for polynomial expansion
            // for PolyN of 5, use PolySigma 1.1
            // for PolyN of 7, use PolySigma 1.5
        
        flow.setUseGaussian(flowUseGaussian);
            // use gaussian filter instead of box filter: slower but more accurate
            // normally use a larger window with this option
        
        
        
        // calculate flow
        
        flow.calcOpticalFlow(vid);
        
        
        
        // save average flows per frame + per vid
        
        frameFlows.push_back(flow.getAverageFlow());
        
        vidFlowTotal += frameFlows.back();
        vidFlowAvg = vidFlowTotal / frameFlows.size();
        
        // calculate dot position to show average movement
        
        dotPos += frameFlows.back();
        dotPath.addVertex(dotPos);
        
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // draw video and flow field
    
    ofSetColor(255);
    
    ofPushMatrix();
    ofTranslate(250,0); // make room for gui at left
    ofScale(vidScale, vidScale); // scale to vidScale gui choice
    
    
        vid.draw(0,0, vidW, vidH); // draw video at scale
    
        ofSetLineWidth(1); // draw thin flow field
        // (i.e. don't change flow field's line width with gui lineWidth choice)
    
        flow.draw(0,0, vidW, vidH); // draw flow field at scale
    
        ofSetLineWidth(lineWidth); // draw other lines based on gui lineWidth
    
    
        // draw dot to track average flow
    
        ofSetColor(0,255,255);
        
        ofDrawCircle(dotPos, lineWidth*0.75); // draw dot
    
        dotPath.draw(); // draw dot's path
    
        if (frameFlows.size() > 0){
        // draw average magnitude and direction of this frame, if we have it
    
            ofSetColor(255,0,0);
        
            ofDrawLine(vidW*0.5, vidH*0.5, vidW*0.5+ frameFlows.back().x * 50, vidH*0.5+ frameFlows.back().y * 50);
            // scale motion average * 50 for visibility
        }
    
    
    ofPopMatrix();
    
    // draw framerate at bottom of video
    ofDrawBitmapStringHighlight(ofToString((int) ofGetFrameRate()) + "fps", 250, vidH*vidScale);
    
    
    
    // draw gui
    gui.draw();
    

    
    
    // draw average flow per frame and per video as lines
    
    if (frameFlows.size() > 0){ // only draw if there's been some flow
        
        ofPushMatrix();
        ofTranslate(125,300); // move below gui
        
            // draw avg magnitude + direction
        
            ofSetColor(255,0,0);
            ofDrawLine(0, 0, vidFlowAvg.x * 50, vidFlowAvg.y * 50); // scale up * 50 for visibility
        
            ofSetColor(0,255,0);
        
            // draw normalized direction for magnitude reference
        
            ofDrawCircle(vidFlowAvg.getNormalized().x * 50, vidFlowAvg.getNormalized().y * 50, lineWidth);
        
            ofDrawBitmapStringHighlight("Avg magnitude + direction\nnorm'd to 50px",-115,-50);
        
        
        ofPopMatrix();
        
    }
    

}

//--------------------------------------------------------------
void ofApp::reset(){
    
    flow.resetFlow(); // clear ofxCv's flow cache
    
    // reset frameFlow tracking
    frameFlows.clear();
    vidFlowTotal.set(0,0);
    
    // moving dot
    dotPos = ofVec2f(vidW*0.5, vidH*0.5); // center of drawn video
    
    // polyline that tracks dot movement
    dotPath.clear();
    dotPath.addVertex(dotPos);
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
