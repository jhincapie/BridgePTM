#pragma once

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ofMain.h"
#include "ofxCvMain.h"
#include "ofxGui.h"

#include "BridgePTM/BridgePTM.h"

class BridgeApp : public ofBaseApp
{
private:
    cv::VideoCapture *capture;
    
    Document* document;
    Image* bridgeIMG;
    Match* match;
    
    FeatureCreator* creatorDoc;
    FeatureCreator* creatorCap;
    BridgeMatcher* matcher;
    
    int fps;
    cv::Size processingSize;
    cv::Mat frame, sizedframe;
    ofxCvColorImage* matchesImage;
    
    //------ Runtime Variables -----
    bool parametersChanged;
    bool isPaused;
    
    //------ UI Elements -------
    ofxPanel pGuid;
    ofxGuiGroup gPlayback;
    ofxButton bPlay;
    ofxButton bPause;
    
    ofxGuiGroup gDocumentFC;
    ofxSlider<double> sHessianDFC;
    ofxSlider<int> sOctavesDFC;
    ofxSlider<int> sOctaveLayerDFC;
    ofxToggle tgUprightDFC;
    ofxToggle tgOrientationNormalizedDFC;
    ofxToggle tgScaleNormalizedDFC;
    ofxSlider<double> sPatternScaleDFC;
    ofxSlider<int> sNOctavesDFC;
    
    ofxGuiGroup gCaptureFC;
    ofxSlider<double> sHessianCFC;
    ofxSlider<int> sOctavesCFC;
    ofxSlider<int> sOctaveLayerCFC;
    ofxToggle tgUprightCFC;
    ofxToggle tgOrientationNormalizedCFC;
    ofxToggle tgScaleNormalizedCFC;
    ofxSlider<double> sPatternScaleCFC;
    ofxSlider<int> sNOctavesCFC;
    
    ofxGuiGroup gMatcher;
    ofxSlider<double> sMinDistanceFactor;
    ofxSlider<int> sLIPTableNumber; //4
    ofxSlider<int> sLIPKeySize; //25
    ofxSlider<int> sLIPMultiProbeLevel; //0
    ofxSlider<int> sLSPCheckes;
    ofxSlider<double> sLSPeps;
    ofxToggle tgLSPSorted;
    
    ofxGuiGroup gCaptureImage;
    ofxSlider<double> sReductionFactor; //0.1 ... 1.0
    ofxSlider<int> sBlurKernelSize;
    
public:
    BridgeApp(Document * document, cv::VideoCapture *capture);
    ~BridgeApp();
    
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //UI event handlers
    void playButtonPressed();
    void pausedButtonPressed();
    void DoubleChanged(double & value);
    void IntChanged(int & value);
    void BoolChanged(bool & value);
    void CheckParametersChanged();
    
    //Image drawing
    void drawMatches(float x, float y, float factorX, float factorY);
};
