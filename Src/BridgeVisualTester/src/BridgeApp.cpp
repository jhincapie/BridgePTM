#include "BridgeApp.h"

//--------------------------------------------------------------
BridgeApp::BridgeApp(Document * document, cv::VideoCapture *capture)
{
    this->document = document;
    this->capture = capture;
    this->match = NULL;
    this->bridgeIMG = NULL;
    this->matchesImage = NULL;
    this->creatorDoc = NULL;
    this->creatorCap = NULL;
    
    this->parametersChanged = true;
    this->isPaused = true;
}

//--------------------------------------------------------------
BridgeApp::~BridgeApp()
{
    delete this->creatorDoc;
    delete this->creatorCap;
    delete this->matchesImage;
}

//--------------------------------------------------------------
void BridgeApp::setup()
{
    *capture >> frame;
    /* calculates the actual frame size */
    halfSize = cv::Size((int)(frame.cols/4), (int)(frame.rows/4));
    /* get fps, needed to set the delay */
    fps = (int)capture->get(CV_CAP_PROP_FPS);
    
    //Set-up event handlers
    bPlay.addListener(this, &BridgeApp::playButtonPressed);
    bPause.addListener(this, &BridgeApp::pausedButtonPressed);
    sHessianDFC.addListener(this, &BridgeApp::sHessianDFCSliderChanged);
    
    //Putting up the UI elements together
    pGuid.setup();
    gPlayback.setup("Playback");
    gPlayback.add(bPlay.setup("Play"));
    gPlayback.add(bPause.setup("Pause"));
    pGuid.add(&gPlayback);
    
    gDocumentFC.setup("Document - Feature Creator");
    gDocumentFC.add(sHessianDFC.setup("Hessian", 350, 200, 600));
//    ofxSlider<int> sOctavesDFC;
//    ofxSlider<int> sOctaveLayerDFC;
//    ofxToggle tgUprightDFC;
//    ofxToggle tgOrientationNormalizedDFC;
//    ofxToggle tgScaleNormalizedDFC;
//    ofxSlider<double> sPatternScaleDFC;
//    ofxSlider<int> sNOctavesDFC;
    pGuid.add(&gDocumentFC);
    
//    ofxGuiGroup gCaptureFC;
//    ofxSlider<double> sHessianCFC;
//    ofxSlider<int> sOctavesCFC;
//    ofxSlider<int> sOctaveLayerCFC;
//    ofxToggle tgUprightCFC;
//    ofxToggle tgOrientationNormalizedCFC;
//    ofxToggle tgScaleNormalizedCFC;
//    ofxSlider<double> sPatternScaleCFC;
//    ofxSlider<int> sNOctavesCFC;
//    
//    ofxGuiGroup gMatcher;
//    ofxSlider<int> sMinDistanceFactor;
//    ofxSlider<int> sLIPTableNumber; //4
//    ofxSlider<int> sLIPKeySize; //25
//    ofxSlider<int> sLIPMultiProbeLevel; //0
//    
//    ofxGuiGroup gCaptureImage;
//    ofxSlider<double> sReductionFactor; //0.1 ... 1.0
//    ofxSlider<int> sBlurFactor;
    
    
}

//--------------------------------------------------------------
void BridgeApp::update()
{
    bool updateUI = false;
    if(this->parametersChanged)
    {
        this->parametersChanged = false;
        updateUI = true;
        
        //Creates feature creator for the document and the capture
        creatorDoc = new FeatureCreator(sHessianDFC);
        creatorCap = new FeatureCreator();
        
        //Computes features for the target document
        creatorDoc->ComputeDocument(document);
        
        //Trains the matcher for the features of the document
        matcher = new BridgeMatcher();
        matcher->Train(document);
    }
    
    if(!this->isPaused || updateUI)
    {
        /* get a frame */
        if(!this->isPaused)
            *capture >> frame;
        
        /* always check */
        if(!frame.empty())
        {
            /* reduce in size */
            cv::resize(frame, halfframe, halfSize);
            //halfframe = frame;
            cv::transpose(halfframe, halfframe);
            cv::flip(halfframe, halfframe, 1);
            cv::cvtColor(halfframe, halfframe, CV_BGR2GRAY);
            
            if(bridgeIMG == NULL)
                bridgeIMG = new Image(&halfframe);
            else
                bridgeIMG->UpdateData(&halfframe);
            creatorCap->ComputeImage(bridgeIMG);
            
            Match* newMatch = matcher->Match(bridgeIMG);
            Match* tmpMatch = this->match;
            this->match = newMatch;
            delete tmpMatch;
        }
    }
}

//--------------------------------------------------------------
void BridgeApp::draw()
{
    this->drawMatches(250, 0, 0.45, 0.45);
    this->pGuid.draw();
}

void BridgeApp::drawMatches(float x, float y, float factorX, float factorY)
{
    if(this->match == NULL)
        return;
    
    char pageImagePath[MAX_STRING_SIZE * sizeof(char)];
    
    pageImagePath[0] = 0; //clears the string
    strlcat(pageImagePath, match->Document->Root, MAX_STRING_SIZE);
    strlcat(pageImagePath, "/", MAX_STRING_SIZE);
    strlcat(pageImagePath, match->Page->FileName, MAX_STRING_SIZE);
    
    cv::Mat pageImage = cv::imread(pageImagePath, CV_LOAD_IMAGE_GRAYSCALE);
    
    std::vector<cv::Point2f> device_corners(5);
	device_corners[0] = cvPoint(0,0);
	device_corners[1] = cvPoint(bridgeIMG->Capture->cols, 0 );
	device_corners[2] = cvPoint(bridgeIMG->Capture->cols, bridgeIMG->Capture->rows );
	device_corners[3] = cvPoint(0, bridgeIMG->Capture->rows );
	device_corners[4] = cvPoint(bridgeIMG->Capture->cols/2.0f, bridgeIMG->Capture->rows/2.0f);
    
	if (!match->Homography.empty())
	{
		cv::perspectiveTransform(device_corners, device_corners, match->Homography);
        
		//-- Draw lines between the corners (the mapped object in the scene - image_2 )
		cv::line( pageImage, device_corners[0], device_corners[1] , cv::Scalar( 0, 255, 0), 4 );
		cv::line( pageImage, device_corners[1], device_corners[2] , cv::Scalar( 0, 255, 0), 4 );
		cv::line( pageImage, device_corners[2], device_corners[3] , cv::Scalar( 0, 255, 0), 4 );
		cv::line( pageImage, device_corners[3] , device_corners[0] , cv::Scalar( 0, 255, 0), 4 );
	}
    
    //-- Draw only "good" matches
    cv::Mat matchesImg;
    cv::drawMatches(*bridgeIMG->Capture, *bridgeIMG->Features->keypoints,
                    pageImage, *match->Page->Features->keypoints,
                    *match->MatcherMatches, matchesImg, cv::Scalar::all(-1), cv::Scalar::all(-1),
                    std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    
    if(this->matchesImage == NULL)
        this->matchesImage = new ofxCvColorImage();
    this->matchesImage->setFromPixels(matchesImg.data, matchesImg.cols, matchesImg.rows);
    this->matchesImage->draw(x, y,
                             (int)(this->matchesImage->getWidth() * factorX),
                             (int)(this->matchesImage->getHeight() * factorY));
}

void BridgeApp::playButtonPressed()
{
    this->isPaused = false;
}

void BridgeApp::pausedButtonPressed()
{
    this->isPaused = true;
}

void BridgeApp::sHessianDFCSliderChanged(double & value)
{    
    if(this->creatorDoc == NULL)
        return;
    
    if(this->creatorDoc->dHessianThreshold != sHessianDFC)
        this->parametersChanged = true;
}

//--------------------------------------------------------------
void BridgeApp::keyPressed(int key)
{

}

//--------------------------------------------------------------
void BridgeApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void BridgeApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void BridgeApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void BridgeApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void BridgeApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void BridgeApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void BridgeApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void BridgeApp::dragEvent(ofDragInfo dragInfo){ 

}
