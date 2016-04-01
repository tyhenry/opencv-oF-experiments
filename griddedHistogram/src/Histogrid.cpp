//
//  Histogrid.cpp
//  griddedHistogram
//
//  Created by Tyler on 3/31/16.
//
//

#include "Histogrid.hpp"


Histogrid::Histogrid(){
    
}


Histogrid::Histogrid(ofImage& _img, int _nDivsX, int _nDivsY, int _nBins){
    
    img = &_img;
    
    nDivsX = _nDivsX;
    nDivsY = _nDivsY;
    nBins = _nBins;
    
    imgMat = toCv(*img);
    cvtColor(imgMat,imgMat, CV_BGR2GRAY);
    // convert to grayscale for intensity
    
}


void Histogrid::run(){
    
    // calculate histograms
    
    histograms.clear();
    rects.clear();
    
    Mat hist;
    float range[] = { 0, 256 }; // histogram range, upper bound exclusive
    const float* histRange = { range }; // another range? cv weirdness
    
    for (int r=0; r<nDivsY; r++){ // x
        for (int c=0; c<nDivsX; c++){ // y
        
            //calc masked histogram
            
            // generate mask
            int x = imgMat.cols / nDivsX * c; // x
            int y = imgMat.rows / nDivsY * r; // y
            int w = imgMat.cols / nDivsX; // width
            int h = imgMat.rows / nDivsY; // height
            
            cout << "mask " << r*nDivsX+c << ": " << x << "," << y << " : " << w << "," << h << endl;
            
            // save to rects for future reference
            ofRectangle tmpRect = ofRectangle( x,y, w,h );
            rects.push_back(tmpRect);
            
            Mat mask = Mat::zeros(imgMat.rows, imgMat.cols, CV_8U); // full black at dims of img
            cv::Rect const roi( x,y, w,h ); // selected rectangle for mask (region of interest)
            mask(roi) = 1; // set roi to white
            
            // get histogram for image section
            calcHist(&imgMat, 1, 0, mask, hist, 1, &nBins, &histRange, true, false);
            normalize(hist, hist, 0, 255, NORM_MINMAX, -1, Mat()); // convert vals to 0-255
        
            // save histogram values to vector
            vector<float> tmpHist;
            for (int i=0; i<nBins; i++){
                
                tmpHist.push_back(hist.at<float>(i));
                
            }
            // save to histogram vector of histograms
            histograms.push_back(tmpHist);
        }
    }
    

    
}

void Histogrid::draw(int n, ofColor color){
    draw(n,0,0,img->getWidth(),img->getHeight(), color);
    // draw at (0,0) and image size
}

void Histogrid::draw(int n, float x, float y, float w, float h, ofColor color){
    
    ofPushMatrix();
    ofTranslate(x,y);
    
    ofPushStyle();
    ofSetLineWidth(1);
    ofFill();
    ofSetColor(color);
    
    for (int i=0; i<nBins-1; i++){
        
        ofVec2f pt1;
        pt1.x= ofMap(i, 0,nBins, 0,w);
        pt1.y= ofMap(histograms[n][i], 0,255, h,0);
        
        ofVec2f pt2;
        pt2.x= ofMap(i+1, 0,nBins, 0,w);
        pt2.y= ofMap(histograms[n][i+1], 0,255, h, 0);
        
        ofDrawLine(pt1,pt2); // line chart histogram
        
    }
                   
    ofPopStyle();
    ofPopMatrix();
    
}


const vector<vector<float>>& Histogrid::getHistograms() const{
    
    return histograms; // return all histograms
}

const vector<float>& Histogrid::getHistogram(int n) const{
    
    return histograms[n]; // return specific histogram
}

int Histogrid::getNDivsX(){
    
    return nDivsX;
}

int Histogrid::getNDivsY(){
    
    return nDivsY;
}

int Histogrid::getNBins(){
    
    return nBins;
}

const vector<ofRectangle>& Histogrid::getRectangles() const{
    return rects;
}