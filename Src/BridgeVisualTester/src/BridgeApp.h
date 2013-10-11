#pragma once

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "ofMain.h"
#include "ofxCvMain.h"

#include "BridgePTM/BridgePTM.h"

class BridgeApp : public ofBaseApp
{
private:
    cv::VideoCapture *capture;
    
    Document* document;
    Image* bridgeIMG;
    Match* match;
    
    FeatureCreator* creator;
    BridgeMatcher* matcher;
    
    int fps;
    cv::Size halfSize;
    cv::Mat frame, halfframe;
    ofxCvColorImage* matchesImage;
    
    bool parametersChanged;
    
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
};
