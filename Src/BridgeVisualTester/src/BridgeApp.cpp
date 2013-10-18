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
    this->matcher = NULL;
    
    this->docParametersChanged = true;
    this->capParametersChanged = true;
    this->matchParametersChanged = true;
    this->preCapParametersChanged = true;
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
    /* get fps, needed to set the delay */
    fps = (int)capture->get(CV_CAP_PROP_FPS);
    
    //Set-up event handlers
    //-- Playback
    bPlay.addListener(this, &BridgeApp::playButtonPressed);
    bPause.addListener(this, &BridgeApp::pausedButtonPressed);
    //-- Document Feature Creator
    sHessianDFC.addListener(this, &BridgeApp::DoubleChanged);
    sOctavesDFC.addListener(this, &BridgeApp::IntChanged);
    sOctaveLayerDFC.addListener(this, &BridgeApp::IntChanged);
    tgUprightDFC.addListener(this, &BridgeApp::BoolChanged);
    tgOrientationNormalizedDFC.addListener(this, &BridgeApp::BoolChanged);
    tgScaleNormalizedDFC.addListener(this, &BridgeApp::BoolChanged);
    sPatternScaleDFC.addListener(this, &BridgeApp::DoubleChanged);
    sNOctavesDFC.addListener(this, &BridgeApp::IntChanged);
    //-- Capture Feature Creator
    sHessianCFC.addListener(this, &BridgeApp::DoubleChanged);
    sOctavesCFC.addListener(this, &BridgeApp::IntChanged);
    sOctaveLayerCFC.addListener(this, &BridgeApp::IntChanged);
    tgUprightCFC.addListener(this, &BridgeApp::BoolChanged);
    tgOrientationNormalizedCFC.addListener(this, &BridgeApp::BoolChanged);
    tgScaleNormalizedCFC.addListener(this, &BridgeApp::BoolChanged);
    sPatternScaleCFC.addListener(this, &BridgeApp::DoubleChanged);
    sNOctavesCFC.addListener(this, &BridgeApp::IntChanged);
    //-- Matcher Settings
    sMinDistanceFactor.addListener(this, &BridgeApp::DoubleChanged);
    sLIPTableNumber.addListener(this, &BridgeApp::IntChanged);
    sLIPKeySize.addListener(this, &BridgeApp::IntChanged);
    sLIPMultiProbeLevel.addListener(this, &BridgeApp::IntChanged);
    sLSPCheckes.addListener(this, &BridgeApp::IntChanged);
    sLSPeps.addListener(this, &BridgeApp::DoubleChanged);
    tgLSPSorted.addListener(this, &BridgeApp::BoolChanged);
    //-- Pre Capture Settings
    sReductionFactor.addListener(this, &BridgeApp::DoubleChanged);
    sBlurKernelSize.addListener(this, &BridgeApp::IntChanged);
    
    //Putting up the UI elements together
    pGuid.setup();
    gPlayback.setup("Playback");
    gPlayback.add(bPlay.setup("Play"));
    gPlayback.add(bPause.setup("Pause"));
    pGuid.add(&gPlayback);
    
    gDocumentFC.setup("Document - Feature Creator");
    gDocumentFC.add(sHessianDFC.setup("Hessian", 350, 300, 450));
    gDocumentFC.add(sOctavesDFC.setup("Octaves", 3, 1, 10));
    gDocumentFC.add(sOctaveLayerDFC.setup("Octave Layers", 4, 1, 10));
    gDocumentFC.add(tgUprightDFC.setup("Uprigth", false));
    gDocumentFC.add(tgOrientationNormalizedDFC.setup("Orient. Norm.", true));
    gDocumentFC.add(tgScaleNormalizedDFC.setup("Scale Norm.", true));
    gDocumentFC.add(sPatternScaleDFC.setup("Pattern Scale", 22, 15, 30));
    gDocumentFC.add(sNOctavesDFC.setup("Nro Octaves", 4, 1, 10));
    pGuid.add(&gDocumentFC);
    
    gCaptureImage.setup("Capture Preprocessing");
    gCaptureImage.add(sReductionFactor.setup("Reduction", 0.5, 0.1, 1));
    gCaptureImage.add(sBlurKernelSize.setup("Blur Kernel", 0, 0, 5));
    pGuid.add(&gCaptureImage);
    
    gCaptureFC.setup("Capture - Feature Creator");
    gCaptureFC.add(sHessianCFC.setup("Hessian", 350, 300, 450));
    gCaptureFC.add(sOctavesCFC.setup("Octaves", 3, 1, 10));
    gCaptureFC.add(sOctaveLayerCFC.setup("Octave Layers", 4, 1, 10));
    gCaptureFC.add(tgUprightCFC.setup("Uprigth", false));
    gCaptureFC.add(tgOrientationNormalizedCFC.setup("Orient. Norm.", true));
    gCaptureFC.add(tgScaleNormalizedCFC.setup("Scale Norm.", true));
    gCaptureFC.add(sPatternScaleCFC.setup("Pattern Scale", 22, 15, 30));
    gCaptureFC.add(sNOctavesCFC.setup("Nro Octaves", 4, 1, 10));
    pGuid.add(&gCaptureFC);

    gMatcher.setup("Matcher");
    gMatcher.add(sMinDistanceFactor.setup("Min Distance Factor", 2, 1, 3));
    gMatcher.add(sLIPTableNumber.setup("Table Number", 4, 1, 10)); //4
    gMatcher.add(sLIPKeySize.setup("Key Size", 25, 20, 30)); //25
    gMatcher.add(sLIPMultiProbeLevel.setup("MultiProbe Level", 0, 0, 5)); //0
    gMatcher.add(sLSPCheckes.setup("Checks", 32, 20, 50));
    gMatcher.add(sLSPeps.setup("EPS", 0, 0, 5));
    gMatcher.add(tgLSPSorted.setup("Sorted", true));
    pGuid.add(&gMatcher);
}

//--------------------------------------------------------------
void BridgeApp::update()
{
    if(this->docParametersChanged)
    {
        //Creates feature creator for the document
        creatorDoc = new FeatureCreator(sHessianDFC, sOctavesDFC, sOctaveLayerDFC, tgUprightDFC,
                                        tgOrientationNormalizedDFC, tgScaleNormalizedDFC, sPatternScaleDFC, sNOctavesDFC);
        //Computes features for the target document
        creatorDoc->ComputeDocument(document);
    }
    
    if(this->capParametersChanged)
    {
        //Creates feature creator for the document
        creatorCap = new FeatureCreator(sHessianCFC, sOctavesCFC, sOctaveLayerCFC, tgUprightCFC,
                                        tgOrientationNormalizedCFC, tgScaleNormalizedCFC, sPatternScaleCFC, sNOctavesCFC);
    }
    
    if(this->matchParametersChanged)
    {
        //Trains the matcher for the features of the document
        matcher = new BridgeMatcher(sLIPTableNumber, sLIPKeySize, sLIPMultiProbeLevel,
                                    sLSPCheckes, sLSPeps, tgLSPSorted);
        matcher->Train(document);
    }
    
    if(this->docParametersChanged || this->capParametersChanged ||
       this->matchParametersChanged || this->preCapParametersChanged || !this->isPaused)
    {
        /* get a frame */
        if(!this->isPaused)
            *capture >> frame;
        
        /* always check */
        if(!frame.empty())
        {
            /* calculates the actual frame size */
            processingSize = cv::Size((int)(frame.cols * sReductionFactor), (int)(frame.rows * sReductionFactor));
            
            /* reduce in size */
            cv::resize(frame, sizedframe, processingSize);
            cv::transpose(sizedframe, sizedframe);
            cv::flip(sizedframe, sizedframe, 1);
            cv::cvtColor(sizedframe, sizedframe, CV_BGR2GRAY);
            
            /* blurs the image with a gaussian blur */
            if(sBlurKernelSize > 0)
            {
                if(sBlurKernelSize % 2 == 0)
                    sBlurKernelSize = sBlurKernelSize + 1;
                cv::GaussianBlur(sizedframe, sizedframe, cv::Size(sBlurKernelSize, sBlurKernelSize), 0, 0);
            }
            
            if(bridgeIMG == NULL)
                bridgeIMG = new Image(&sizedframe);
            else
                bridgeIMG->UpdateData(&sizedframe);
            creatorCap->ComputeImage(bridgeIMG);
            
            Match* newMatch = matcher->Match(bridgeIMG, sMinDistanceFactor);
            Match* tmpMatch = this->match;
            this->match = newMatch;
            delete tmpMatch;
        }
        else
        {
            //restarts the video capture
            capture->set(CV_CAP_PROP_POS_FRAMES, 0);
        }
        
        this->docParametersChanged = false;
        this->capParametersChanged = false;
        this->matchParametersChanged = false;
        this->preCapParametersChanged = false;
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
    
    cv::Mat pageClone = match->Page->Capture->clone();
    
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
		cv::line(pageClone, device_corners[0], device_corners[1] , cv::Scalar( 0, 255, 0), 4 );
		cv::line(pageClone, device_corners[1], device_corners[2] , cv::Scalar( 0, 255, 0), 4 );
		cv::line(pageClone, device_corners[2], device_corners[3] , cv::Scalar( 0, 255, 0), 4 );
		cv::line(pageClone, device_corners[3] , device_corners[0] , cv::Scalar( 0, 255, 0), 4 );
	}

    //-- Draw only "good" matches
    cv::Mat matchesImg;
    cv::drawMatches(*bridgeIMG->Capture, *bridgeIMG->Features->keypoints,
                    pageClone, *match->Page->Features->keypoints,
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

void BridgeApp::DoubleChanged(double & value)
{
    this->CheckParametersChanged();
}

void BridgeApp::IntChanged(int & value)
{
    this->CheckParametersChanged();
}

void BridgeApp::BoolChanged(bool & value)
{
    this->CheckParametersChanged();
}

void BridgeApp::CheckParametersChanged()
{
    this->preCapParametersChanged = true;
    if(this->creatorDoc != NULL)
    {
        if(this->creatorDoc->dHessianThreshold != sHessianDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->dOctaves != sOctavesDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->dOctaveLayers != sOctaveLayerDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->dUpright != tgUprightDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->eOrientationNormalized != tgOrientationNormalizedDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->eScaleNormalized != tgScaleNormalizedDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->ePatternScale != sPatternScaleDFC)
            this->docParametersChanged = true;
        if(this->creatorDoc->eNOctaves != sNOctavesDFC)
            this->docParametersChanged = true;
    }
    
    if(this->creatorCap != NULL)
    {
        if(this->creatorCap->dHessianThreshold != sHessianCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->dOctaves != sOctavesCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->dOctaveLayers != sOctaveLayerCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->dUpright != tgUprightCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->eOrientationNormalized != tgOrientationNormalizedCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->eScaleNormalized != tgScaleNormalizedCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->ePatternScale != sPatternScaleCFC)
            this->capParametersChanged = true;
        if(this->creatorCap->eNOctaves != sNOctavesCFC)
            this->capParametersChanged = true;
    }
    
    if(this->matcher != NULL)
    {
        if(this->matcher->ipTableNumber != sLIPTableNumber)
            this->matchParametersChanged = true;
        if(this->matcher->ipKeySize != sLIPKeySize)
            this->matchParametersChanged = true;
        if(this->matcher->ipMultiProbeLevel != sLIPMultiProbeLevel)
            this->matchParametersChanged = true;
        if(this->matcher->spChecks != sLSPCheckes)
            this->matchParametersChanged = true;
        if(this->matcher->spEPS != sLSPeps)
            this->matchParametersChanged = true;
        if(this->matcher->spSorted != tgLSPSorted)
            this->matchParametersChanged = true;
    }
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
