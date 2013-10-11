#include "BridgeApp.h"

//--------------------------------------------------------------
BridgeApp::BridgeApp(Document * document, cv::VideoCapture *capture)
{
    this->document = document;
    this->capture = capture;
    this->match = NULL;
    this->bridgeIMG = NULL;
    this->matchesImage = NULL;
    
    this->parametersChanged = true;
}

//--------------------------------------------------------------
BridgeApp::~BridgeApp()
{
    if(this->matchesImage == NULL)
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
}

//--------------------------------------------------------------
void BridgeApp::update()
{
    if(this->parametersChanged)
    {
        //Computes the document features
        creator = new FeatureCreator();
        creator->ComputeDocument(document);
        
        //Trains the matcher for the features of the document
        matcher = new BridgeMatcher();
        matcher->Train(document);
        this->parametersChanged = false;
    }
    
    /* get a frame */
    *capture >> frame;
    /* always check */
    if(frame.empty())
        return;
    
    /* reduce in size */
    cv::resize(frame, halfframe, halfSize);
    //halfframe = frame;
    cv::transpose(halfframe, halfframe);
    cv::flip(halfframe, halfframe, 1);
    cv::cvtColor(halfframe, halfframe, CV_BGR2GRAY);
    
    if(bridgeIMG != NULL)
        delete bridgeIMG;
    bridgeIMG = new Image(&halfframe);
    creator->ComputeImage(bridgeIMG);
    match = matcher->Match(bridgeIMG);
}

//--------------------------------------------------------------
void BridgeApp::draw()
{
    if(this->match == NULL)
        return;
    
    char * pageImagePath = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
    
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
                    std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    
    if(this->matchesImage == NULL)
        this->matchesImage = new ofxCvColorImage();
    this->matchesImage->setFromPixels(matchesImg.data, matchesImg.cols, matchesImg.rows);
    this->matchesImage->draw(0, 0,
                             (int)(this->matchesImage->getWidth() * 0.5),
                             (int)(this->matchesImage->getHeight() * 0.5));
}

//--------------------------------------------------------------
void BridgeApp::keyPressed(int key){

}

//--------------------------------------------------------------
void BridgeApp::keyReleased(int key){

}

//--------------------------------------------------------------
void BridgeApp::mouseMoved(int x, int y ){

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
