
#include "ofMain.h"

#include "BridgeApp.h"

#include "BridgePTM/BridgePTM.h"

//========================================================================
void printUsage();
void printFolderNotExist();

//========================================================================
int main(int argc, const char * argv[])
{
    //Checks for the right number of parameters
    if(argc < 2)
    {
        printUsage();
        return 1;
    }
    
    //Checks that the provided folder path actually exists
    if(!exists(argv[1]))
    {
        printFolderNotExist();
        printUsage();
        return 1;
    }
    
    //Creates the document object - loads the pages and content on the constructor
    Document * document = new Document(argv[1]);
    
    //Creates video capture object - in order to feed it to the matcher
    char* testvideo = (char*)malloc(MAX_STRING_SIZE * sizeof(char));
    testvideo[0] = 0;
    strlcat(testvideo, argv[1], MAX_STRING_SIZE);
#ifdef WIN32
    strlcat(testvideo, "\\", MAX_STRING_SIZE);
#else
	strlcat(testvideo, "//", MAX_STRING_SIZE);
#endif
    strlcat(testvideo, "test.MOV", MAX_STRING_SIZE);
	if(!exists(testvideo))
	{
		std::cerr << "Video does not exist [" << testvideo << "]" <<  std::endl;
		return 1;
	}
    
    cv::VideoCapture *capture = new cv::VideoCapture(testvideo);
	if (!capture->isOpened())
	{
		std::cerr << "Unable to load the video [" << testvideo << "]" <<  std::endl;
		return 1;
	}
    
	ofSetupOpenGL(1024,768,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new BridgeApp(document, capture));
}

void printUsage()
{
    std::cout << "Usage: BridgeVisualTester \"path to document folder\"" << std::endl;
}

void printFolderNotExist()
{
    std::cout << "Given folder does not exist, please use full path." << std::endl;
}

