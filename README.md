# OpenCV oF Experiments

Code experiments related to computer vision & cinema posts on my [blog](http://www.tylerhenry.com/category/computervision/) at [tylerhenry.com](http://tylerhenry.com).  
Mostly using OpenCV and openFrameworks.

---

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

