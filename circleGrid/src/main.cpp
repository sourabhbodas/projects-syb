#include "all_includes.h"
#include "processingClass.h"

/**********************************
 * This is a program for detecting circular patterns
 * Author: Sourabh Bodas
 *
 * 1. Image processing class
 **********************************/

using namespace std;
using namespace cv;

int main ()
{
	// Declared static for later references
    static Processing process;
    
    // Declare the variables
    cv::Mat inImage, outImage, inImage2;

    inImage2 = cv::imread("test2.png");

    std::cout << "Preprocessing Starts" << std::endl;

    inImage2.copyTo(inImage);

	// It will store all the Contours and Hierarchy
	process.threshContImage(inImage, &outImage, &process.contours, &process.hierarchy);
	
	// Commented out for testin purpose
	//process.threshElliImage(inImage, &outImage, &minRect, &minEllipse);
	
	imshow("Image: in", inImage);
	imshow("Image: final", outImage);

	cv::waitKey(0);
    
    return 0;
}
