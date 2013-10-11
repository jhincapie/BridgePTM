#include "ofMain.h"
#include "testApp.h"

#include "BridgePTM/BridgePTM.h"

//========================================================================
int main(int argc, const char * argv[])
{

    //Creates the document object - loads the pages and content on the constructor
    Document * document = new Document(argv[1]);
    
    //Computes the document features
    FeatureCreator* creator = new FeatureCreator();
    creator->ComputeDocument(document);
    
    //Trains the matcher for the features of the document
    BridgeMatcher* matcher = new BridgeMatcher();
    matcher->Train(document);
    
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new testApp());

}
