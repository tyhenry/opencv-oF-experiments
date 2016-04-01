# OpenCV oF Experiments

Code experiments related to computer vision & cinema posts on my [blog](http://www.tylerhenry.com/category/computervision/) at [tylerhenry.com](http://tylerhenry.com).  
Mostly using OpenCV and openFrameworks.

---

### griddedHistogram

Contains Histogrid class: uses OpenCV calcHist to generate histograms for subsections of an ofImage.
Written in openFrameworks v.9.0 / XCode 7

*uses addons:* [`ofxCv`](https://github.com/kylemcdonald/ofxCv/) and `ofxOpenCv`

![Hisogrid sample output](http://i1.wp.com/www.tylerhenry.com/site/wp-content/uploads/2016/03/histogrid_sample.jpg)


### SIFT_filterMatches_homography

Updated, more complete SIFT implementation than SIFTtest (see below).  
Written in openFrameworks v.9.0 / XCode 7 

*uses addons:* [`ofxCv`](https://github.com/kylemcdonald/ofxCv/) and `ofxOpenCv`

**Related tutorial**

[SIFT Implementation in OpenFrameworks, Part 3](http://www.tylerhenry.com/sift-implementation-in-openframeworks-part-3/)

Updates over SIFTtest:

*  SIFT implementation placed inside SIFTMatcher class
*  added keypoint filter to sort out good keypoint matches
*  added homography transformation to  
   calculate 3D warp of query image to train img

![SIFT_filterMatches_homography sample output](http://i1.wp.com/www.tylerhenry.com/site/wp-content/uploads/2016/03/SIFT_v2_test1.jpg?resize=1024%2C501)  

---

### SIFTtest
First test of SIFT algorithm to match features between two images.  
Written in openFrameworks v.9.0 / XCode 7 

*uses addons:* [`ofxCv`](https://github.com/kylemcdonald/ofxCv/) and `ofxOpenCv`

**Related tutorial**

[SIFT Implementation in OpenFrameworks, Part 2](http://www.tylerhenry.com/sift-implementation-in-openframeworks-part-2/)

**Intro tutorials**

1.  [Getting Started](http://www.tylerhenry.com/getting-started/)
2.  [Finding Features](http://www.tylerhenry.com/finding-features/)
3.  [SIFT Implementation in OpenFrameworks, Part 1](http://www.tylerhenry.com/sift-implementation-in-openframeworks-part-1/)

![SIFTtest sample output](http://i2.wp.com/www.tylerhenry.com/site/wp-content/uploads/2016/03/SIFT_test5.jpg?resize=1024%2C490)  

---

### optFlowTest
Test of optical flow analysis using Farneback algorithm from OpenCV (through ofxCv)
Written in openFrameworks v.9.0 / XCode 7 

*uses addons:* [`ofxCv`](https://github.com/kylemcdonald/ofxCv/), `ofxOpenCv` and `ofxGui`

Display local motion in video as a flow field, and also:

* average motion per frame (red line in video center)
* average motion per clip (red line on left)
* track of average motion per clip (blue dot and path)

** Related tutorial**

[Optical Flow, Part 1](http://www.tylerhenry.com/optical-flow-part-1/)  

[![optFlowtest sample output](http://i1.wp.com/www.tylerhenry.com/site/wp-content/uploads/2016/03/optFlowtest1.jpg)](https://www.youtube.com/watch?v=09hFXvW_NMs)  
[YouTube sample](https://www.youtube.com/watch?v=09hFXvW_NMs)