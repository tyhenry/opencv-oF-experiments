#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    // save start time (in ms) for testing speed
    uint64_t startTime = ofGetElapsedTimeMillis();
    
    
    //---------------------//
    //---- LOAD IMAGES ----//
    //---------------------//
    
    // load the two images as ofImage
    
    findImg.load("peeping_tom_1.jpg"); // image we're looking for (called "query" in OpenCV)
    fieldImg.load("peeping_tom_2.jpg"); // image to search in (called "train" in OpenCV)
    
    // convert to grayscale (SIFT works in grayscale)
    
    findImg.setImageType(OF_IMAGE_GRAYSCALE);
    fieldImg.setImageType(OF_IMAGE_GRAYSCALE);

    
    
    // wrap images with Mat class for use with SIFT
    
    Mat findMat = toCv(findImg);
    Mat fieldMat = toCv(fieldImg);
    
    

    uint64_t loadTime = ofGetElapsedTimeMillis() - startTime; // calc image load time
    ofLogNotice("Image Load") << "took " << loadTime << " ms" << endl; // print load time to console
    startTime = ofGetElapsedTimeMillis();   // reset startTime

    
    
    //------------------------------------//
    //---- SIFT DETECTION/DESCRIPTION ----//
    //------------------------------------//
    
    
    // ---------------------------------------
    // 1. DETECT SIFT keypoints in both images
    // ---------------------------------------

    
    SiftFeatureDetector detector(2000); // SIFT detector object
    
    // 400 = max number of keypoints to find
            //  all optional constructor args, with default values:
            //      (int nfeatures=0, int nOctaveLayers=3, double contrastThreshold=0.04,
            //       double edgeThreshold=10, double sigma=1.6);
    
    
    vector<KeyPoint> findKeypoints, fieldKeypoints; // KeyPoint vectors to store keypoints detected per image
    
    // run the detector on each image
    
    detector.detect(findMat, findKeypoints);
    detector.detect(fieldMat, fieldKeypoints);
    
    
    // print results
    // --------------
    uint64_t detectTime = ofGetElapsedTimeMillis() - startTime; // calculate detection time
    
    // print # keypoints found to console
    
    ofLogNotice("SIFT Detector") << "took " << detectTime << " ms" << endl << endl
        << "# keypoints found" << endl
        << "-----------------" << endl
        << "  findImg: " << findKeypoints.size() << endl
        << "  fieldImg: " << fieldKeypoints.size() << endl;
    
    startTime = ofGetElapsedTimeMillis();   // reset startTime
    // --------------

    
    // --------------------------------------
    // 2. DESCRIBE SIFT features of keypoints
    // --------------------------------------
    
    
    SiftDescriptorExtractor extractor; // SIFT descriptor object
    
    
    Mat findDescriptors, fieldDescriptors; // matrices to hold all features per keypoint in image
    // i.e. in each matrix, row 'i' is the list of features for keypoint 'i'
    
    // run the feature description extractor
    
    extractor.compute(findMat, findKeypoints, findDescriptors);
    extractor.compute(fieldMat, fieldKeypoints, fieldDescriptors);
    
    
    // print results
    // --------------
    uint64_t describeTime = ofGetElapsedTimeMillis() - startTime; // calculate description time
    
    // print some statistics on the matrices
    
    cv::Size findSize = findDescriptors.size(); // size of matrix
    cv::Size fieldSize = fieldDescriptors.size();
    
    ofLogNotice("SIFT Descriptor") << "took " << describeTime << " ms" << endl << endl
        << "findImg feature matrix" << endl
        << "--------------------" << endl
        << "  height: " << findSize.height << ", width: " << findSize.width << endl
        << "  area: " << findSize.area() << ", non-zero: " << countNonZero(findDescriptors) << endl
        << endl
        << "fieldImg feature matrix" << endl
        << "--------------------" << endl
        << "  height: " << fieldSize.height << ", width: " << fieldSize.width << endl
        << "  area: " << fieldSize.area() << ", non-zero: " << countNonZero(fieldDescriptors) << endl;
    
    startTime = ofGetElapsedTimeMillis();   // reset startTime
    // --------------
    
    
    
    //---------------------------------//
    //---- FIND MATCHING KEYPOINTS ----//
    //---------------------------------//

    
    BFMatcher matcher(NORM_L1, true); // Brute-force Matcher object
    // loops through every feature in matrix 1, comparing it to every feature in matrix 2
    // to find best match in matrix 2
    
    // NORM_L2 is "normType" - use NORM_L1 or NORM_L2 for SIFT
    // false is "crossCheck" boolean - true would return more strict matches?
    // see here: http://docs.opencv.org/3.0-last-rst/modules/features2d/doc/common_interfaces_of_descriptor_matchers.html?highlight=bfmatcher#bfmatcher
    
    vector<DMatch> matches; // vector to store matches
    
    // run the matcher
    
    matcher.match(findDescriptors, fieldDescriptors, matches);
    
    
    // print results
    // --------------
    
    uint64_t matchTime = ofGetElapsedTimeMillis() - startTime; // calculate match time
    
    ofLogNotice("Matcher") << "took " << matchTime << " ms" << endl
        << "Found " <<  matches.size() << " matching keypoints" << endl;
    // --------------
    
    
    //----------------------//
    //---- DRAW RESULTS ----//
    //----------------------//
    
    
    // cv::drawMatches() displays the images side by side,
    // with colored circles at each keypoint,
    // and lines connecting the matched keypoints.
    // It draws into a Mat.
    
    Mat matchMat; // image-matrix to temp store the results visualization
    
    // draw results visualization into matchMat image-matrix
    
    drawMatches(findMat, findKeypoints, fieldMat, fieldKeypoints, matches, matchMat);

    // convert matchMat to matchImg
    
    toOf(matchMat,matchImg); // wraps matchMat in ofImage matchImg, allocating as needed
    matchImg.update(); // updates the matchImg texture on the GPU
    
    // now we can draw matchImg in ofApp::draw()
    

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0);
    
    // draw matchImg
    matchImg.draw(0,0);

    
    // sidenote on drawing Mats:
    // -------------------------
    // if we had declared matchMat (from setup()) as a public variable in ofApp.h,
    // we could draw it in ofApp::draw() using ofxCv's drawMat():
    // drawMat(matchMat,0,0);
    // but this is expensive since drawMat() allocates a new GPU texture every time it's called.
    // it's best to convert to ofImage, so the texture is just allocated once on the GPU

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
