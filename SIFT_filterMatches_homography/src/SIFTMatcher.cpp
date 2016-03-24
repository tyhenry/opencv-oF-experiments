//
//  SIFTMatcher.cpp
//  SIFT_filterMatches_homography
//
//  Created by Tyler on 3/24/16.
//
//

#include "SIFTMatcher.hpp"


//--------------------------------------------------------------
// Constructor
//--------------------------------------------------------------

// default constructor - blank
SIFTMatcher::SIFTMatcher(){
}


// actual constructor - always use this
SIFTMatcher::SIFTMatcher(ofImage& _findImg, ofImage& _fieldImg){
    
    findImg = &_findImg;
    fieldImg = &_fieldImg;
    
}

//-------------------------------------------------------------------------
// MATCH
// runs SIFT keypoint detection and feature descriptor on findImg, fieldImg
// then uses cv::BFMatcher to find matches
//-------------------------------------------------------------------------

void SIFTMatcher::match(){
    
    // takes in a query image (findImg) and "train" image (fieldImg)
    // returns the matches vector (unfiltered)
    
    // also references findImg, fieldImg as public findImg, fieldImg
    // and saves resulting KeyPoints vectors as public variables
    
    
    uint64_t startTime = ofGetElapsedTimeMillis(); // save start time (in ms) for testing speed
    
    
    //---------------------//
    //---- LOAD IMAGES ----//
    //---------------------//

    
    
    // convert to grayscale (SIFT works in grayscale)
    
    findImg->setImageType(OF_IMAGE_GRAYSCALE);
    fieldImg->setImageType(OF_IMAGE_GRAYSCALE);
    
    
    // wrap images with Mat class for use with SIFT
    
    Mat findMat = toCv(*findImg);
    Mat fieldMat = toCv(*fieldImg);
    
    
    // print image load time
    // --------------
    uint64_t loadTime = ofGetElapsedTimeMillis() - startTime; // calc image load time
    ofLogNotice("siftMatch") << "took " << loadTime << " ms to prep images" << endl; // print load time to console
    startTime = ofGetElapsedTimeMillis();   // reset startTime
    // --------------
    
    
    
    //------------------------------------//
    //---- SIFT DETECTION/DESCRIPTION ----//
    //------------------------------------//
    
    
    // ---------------------------------------
    // 1. DETECT SIFT keypoints in both images
    // ---------------------------------------
    
    
    SiftFeatureDetector detector(2000); // SIFT detector object
    /*
     // 2000 = max number of keypoints to find
     //  all optional constructor args, with default values:
     //      (int nfeatures=0, int nOctaveLayers=3, double contrastThreshold=0.04,
     //       double edgeThreshold=10, double sigma=1.6);
     */
    
    
    // run the detector on each image
    
    detector.detect(findMat, findKeypoints);
    detector.detect(fieldMat, fieldKeypoints);
    
    
    // print results
    // --------------
    uint64_t detectTime = ofGetElapsedTimeMillis() - startTime; // calculate detection time
    
    // print # keypoints found to console
    
    ofLogNotice("SIFTMatcher") << "took " << detectTime << " ms to find keypoints" << endl << endl
    << "          # keypoints found" << endl
    << "          -----------------" << endl
    << "            findImg: " << findKeypoints.size() << endl
    << "            fieldImg: " << fieldKeypoints.size() << endl;
    
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
    
    ofLogNotice("SIFTMatcher") << "took " << describeTime << " ms to describe features" << endl << endl
    << "          findImg feature matrix" << endl
    << "          ----------------------" << endl
    << "            height: " << findSize.height << ", width: " << findSize.width << endl
    << "            area: " << findSize.area() << ", non-zero: " << countNonZero(findDescriptors) << endl
    << endl
    << "          fieldImg feature matrix" << endl
    << "          -----------------------" << endl
    << "            height: " << fieldSize.height << ", width: " << fieldSize.width << endl
    << "            area: " << fieldSize.area() << ", non-zero: " << countNonZero(fieldDescriptors) << endl;
    
    startTime = ofGetElapsedTimeMillis();   // reset startTime
    // --------------
    
    
    
    //---------------------------------//
    //---- FIND MATCHING KEYPOINTS ----//
    //---------------------------------//
    
    
    BFMatcher matcher(NORM_L1, true); // Brute-force Matcher object
    /*
     // loops through every feature in matrix 1, comparing it to every feature in matrix 2
     // to find best match in matrix 2
     
     // NORM_L1 is "normType" - use NORM_L1 or NORM_L2 for SIFT.  I think this determines the type of normalization done when determining "distance" (in n-dimensional space) between keypoints
     // true is crossCheck boolean - this means that BFMatcher will only return a match when both keypoints find each other as their closest match.  This should be set to true to produce more reliable matches, but only if you have a lot of keypoints.
     // a good visual example of cross-checking is on StackOverflow: http://stackoverflow.com/questions/11181823/why-we-need-crosscheckmatching-for-feature
     // see here for BFMatcher reference: http://docs.opencv.org/3.0-last-rst/modules/features2d/doc/common_interfaces_of_descriptor_matchers.html?highlight=bfmatcher#bfmatcher
     */
    
    // run the matcher
    
    matcher.match(findDescriptors, fieldDescriptors, matches);
    
    
    // print results
    // --------------
    uint64_t matchTime = ofGetElapsedTimeMillis() - startTime; // calculate match time
    
    ofLogNotice("SIFTMatcher") << "took " << matchTime << " ms to match keypoints" << endl << endl
    << "          Found " <<  matches.size() << " matching keypoints" << endl;
    // --------------

    
}


//--------------------------------------------------------------
// FILTER MATCHES
// using distance calculations
//--------------------------------------------------------------

void SIFTMatcher::filterMatches(){
    
    // code referenced from:
    // http://docs.opencv.org/3.1.0/d5/d6f/tutorial_feature_flann_matcher.html
    // -----------------------------------------------------------------------
    
    uint64_t startTime = ofGetElapsedTimeMillis(); // get start time for speed test

    
    // loop through matches to find min and max distances
    
    double minDist = 100, maxDist = 0;
    
    for( int i = 0; i < matches.size(); i++ ) {
        
        double dist = matches[i].distance; // calc distance in n-dim space
        
        if (dist < minDist) { minDist = dist; }
        if (dist > maxDist) { maxDist = dist; }
    }
    

    // save only "good" matches
    // here, threshold at 2 * minDist or 0.3 * maxDist, whichever is larger
    // this is fairly arbitrary
    
    if (goodMatches.size() > 0){
        goodMatches.clear(); // clear the goodMatches vector if it has anything in it
    }
    
    double threshold = max(2 * minDist, 0.3 * maxDist);
    
    for (int i = 0; i < matches.size(); i++) {
        
        if (matches[i].distance <= threshold) {
            
            goodMatches.push_back(matches[i]); // store as good match
        }
    }
    
    
    // print results
    // --------------
    uint64_t filterTime = ofGetElapsedTimeMillis() - startTime; // calculate filter time
    
    ofLogNotice("SIFTMatcher") << "took " << filterTime << " ms to filter matches" << endl << endl
    << "          calc\'ed minDist: " << minDist << ", maxDist: " << maxDist << endl
    << "          used treshold of: " << threshold << endl
    << "          saved " << goodMatches.size() << " out of " << matches.size() << " total matches" << endl;
    // --------------

    
}


//--------------------------------------------------------------------
// DRAW MATCHES CV
// uses cv::drawMatches() to draw matches visualization into _matchImg
//--------------------------------------------------------------------

void SIFTMatcher::drawMatchesCv(ofImage& _matchImg, bool bUseGoodMatches){
    
    //----------------------//
    //---- DRAW RESULTS ----//
    //----------------------//
    
    /*
     // cv::drawMatches() displays the images side by side,
     // with colored circles at each keypoint,
     // and lines connecting the matched keypoints.
     // It draws into a Mat.
     */
    
    Mat matchMat; // image-matrix to temp store the results visualization
    
    // draw results visualization into matchMat image-matrix
    
    // prep findImg and fieldImg as Mat
    Mat findMat, fieldMat;
    findMat = toCv(*findImg);
    fieldMat = toCv(*fieldImg);
    
    // pointer to matches vector
    vector<DMatch>* matchesPtr = &(matches);
    
    if (bUseGoodMatches){
        matchesPtr = &(goodMatches); // or point to goodMatches vector
    }
    
    drawMatches(findMat, findKeypoints, fieldMat, fieldKeypoints, *matchesPtr, matchMat,
                /* and optional parameters: */
                Scalar::all(-1), Scalar::all(170), vector<char>(), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    /*
     // The Scalar::all(-1) through DRAW_RICH_KEYPOINTS parameters are optional.
     // I'm copying the default arguments for them, except:
     //      Scalar(170) which draws the unmatched keypoints in gray
     //      DRAW_RICH_KEYPOINTS, which draws the size/rotation of the keypoints.
     // The default arguments for those would be:
     //      Scalar::all(-1) for random colors
     //      cv::DrawMatchesFlags::DEFAULT for drawing all keypoints as little circles.
     // see here for drawMatches() reference: http://docs.opencv.org/3.1.0/d4/d5d/group__features2d__draw.html#gsc.tab=0
     */
    
    // convert matchMat to matchImg
    
    toOf(matchMat,_matchImg); // wraps matchMat in ofImage matchImg, allocating as needed
    _matchImg.update(); // updates the matchImg texture on the GPU
    
    // now we can draw matchImg in ofApp::draw()
}


//----------------------------------------------------------------------------------
// GET HOMOGRAPHY
// uses cv::findHomography to get transformation values
// between findImg and fieldImg based on matched keypoints
// then transforms findImg corners to match where it is in fieldImg
//----------------------------------------------------------------------------------

void SIFTMatcher::getHomography(bool bUseGoodMatches){
    
    uint64_t startTime = ofGetElapsedTimeMillis(); // save start time (in ms) for testing speed
    
    
    vector<Point2f> findPts; // Point2f is cv's ofVec2f
    vector<Point2f> fieldPts;
    
    // pointer to matches vector
    vector<DMatch>* matchesPtr = &(matches);
    
    if (bUseGoodMatches){
        matchesPtr = &(goodMatches); // or point to goodMatches vector
    }
    
    for (int i=0; i<matchesPtr->size(); i++){
        
        // get original keypoints based on matches
        
        int findIndex = (*matchesPtr)[i].queryIdx; // get index in findKeypoints of matched keypoint
        int fieldIndex = (*matchesPtr)[i].trainIdx; // get index in fieldKeypoints of matched keypoint
        
        Point2f& findPt = findKeypoints[findIndex].pt; // get 2D location of findKeypoint
        Point2f& fieldPt = fieldKeypoints[fieldIndex].pt; // get 2D location of fieldKeypoint
        
        // save in vectors
        findPts.push_back(findPt);
        fieldPts.push_back(fieldPt);
        
    }
    
    // calculate homography matrix using RANSAC method
    
    Mat H = findHomography(findPts, fieldPts, RANSAC);
    
    // prep findImg and fieldImg as Mats
    
    Mat findMat = toCv(*findImg);
    Mat fieldMat = toCv(*fieldImg);
    
    // get the image corners of findMat
    
    vector<Point2f> findMatCorners(4);
    findMatCorners[0] = cvPoint(0,0);
    findMatCorners[1] = cvPoint( findMat.cols, 0 );
    findMatCorners[2] = cvPoint( findMat.cols, findMat.rows );
    findMatCorners[3] = cvPoint( 0, findMat.rows );
    
    // transform findMat corners to correspond with matched keypoints in fieldImg
    
    vector<Point2f> fieldMatCorners(4); // we'll save the transformed corners here
    
    perspectiveTransform(findMatCorners, fieldMatCorners, H); // perform transformation using homography matrix
    
    
    // now convert fieldMatCorners to an ofVec2f vector for use in openFrameworks
    
    fieldCorners.clear();
    for (int i=0; i<4; i++){
        fieldCorners.push_back(toOf(fieldMatCorners[i]));
    }
    
    
    
    // print results
    // --------------
    uint64_t hTime = ofGetElapsedTimeMillis() - startTime; // calculate homography transform time
    
    ofLogNotice("SIFTMatcher") << "took " << hTime << " ms to do homography transform" << endl << endl
    << "          x,y corners of findImg in fieldImg" << endl
    << "          ----------------------------------" << endl
    << "            " << fieldCorners[0].x << ", " << fieldCorners[0].y << endl
    << "            " << fieldCorners[1].x << ", " << fieldCorners[1].y << endl
    << "            " << fieldCorners[2].x << ", " << fieldCorners[2].y << endl
    << "            " << fieldCorners[3].x << ", " << fieldCorners[3].y << endl;
    // --------------
    
}


//----------------------------------------------------------------------------------
// DRAW HOMOGRAPHY
//----------------------------------------------------------------------------------

void SIFTMatcher::drawHomography(float xOffset, float yOffset, ofColor color, float lineWidth){
    
    ofPushStyle();
        ofSetColor(color);
        ofSetLineWidth(lineWidth);
    
    ofPushMatrix();
    ofTranslate(xOffset,yOffset);
    
        ofDrawLine(fieldCorners[0],fieldCorners[1]);
        ofDrawLine(fieldCorners[1],fieldCorners[2]);
        ofDrawLine(fieldCorners[2],fieldCorners[3]);
        ofDrawLine(fieldCorners[3],fieldCorners[0]);
    
    ofPopMatrix();
    ofPopStyle();
    
}

