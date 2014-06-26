#include "processingClass.h"

cv::RNG rng(12345);

Processing::Processing()
{
    ellipse = true;
    expectedCircles = 10;

    lowerThreshBound = 120;
    upperThreshBound = 200;

    threshChangeRange = 10;

    num = "_dt";
}

Processing::~Processing()
{

}

void Processing::denoiseImage(cv::Mat inImage, cv::Mat *outImage)
{

}

cv::Mat Processing::preprocessImage(cv::Mat inImage)
{
    cv::Mat processedImage, tempImage;

	/// this is shown in the main so commented here
    //cv::imshow("input Image", inImage);

    try
    {
        cv::cvtColor(inImage, tempImage, CV_RGB2GRAY);
        cv::equalizeHist(tempImage, tempImage);
        cv::imshow("Histogram eqalized image", tempImage);

        cv::waitKey(0);
    }
    catch (cv::Exception)
    {
        std::cerr << "The equalize histogram failed" << std::endl;
    }

    // Sobel or other processing
    // Clean the image - denoising
    // Converting to RGB ?

    tempImage.copyTo(processedImage);

    return processedImage;
}

cv::Mat Processing::captureFrame()
{
    cv::Mat frame0, frameFF;
    cv::VideoCapture capture(-1);
    // Legacy C usage
    //CvCapture capture = cvCaptureFromCAM(0);

    std::cout << "Capture started" << std::endl;
    std::cout << frame0.size() << std::endl;

    if(capture.isOpened())
    {
        capture >> frame0;
        // Legacy C usage
        //capture.read(frame0);

        std::cout << frame0.size() << std::endl;

        cv::imwrite("test.png", frame0);
        cv::imshow("InputFrame", frame0);
    }

    return frame0;
}

cv::Mat Processing::loadFrame()
{
    cv::Mat frame0;
    cv::VideoCapture capture(-1);
    // Legacy C usage
    //CvCapture capture = cvCaptureFromCAM(0);

    std::cout << "Capture started" << std::endl;

    capture.open("data/Data_2305/");

    if(capture.isOpened())
    {
        capture >> frame0;
        // Legacy C usage
        //capture.read(frame0);

        std::cout << frame0.size() << std::endl;

        cv::imwrite("test.png", frame0);
        cv::imshow("InputFrame", frame0);
    }

    return frame0;
}


void Processing::threshContImage(cv::Mat inImage,
                     cv::Mat *outImage,
                     std::vector<std::vector<cv::Point> > *contours,
                     std::vector<cv::Vec4i> *hierarchy)
{
    if(expectedCircles >= contours->size())
    {
        lowerThreshBound+=threshChangeRange;
        
        // Temporary image for processing
        cv::Mat tempImg, blurImg, edgeImg;
        std::vector<std::vector<cv::Point> > _contours;
        std::vector<cv::Vec4i> _hierarchy;

        /// Thresholding
        cv::threshold(inImage, tempImg, lowerThreshBound, upperThreshBound, 0);
        std::cout << "Operation: Thresholding Image" << std::endl;
        cv::imshow("Threshold Image", tempImg);

        /// Edge detection
        //cv::Canny(tempImg, tempImg, 100, 150);
        //cv::imshow("Edge Image", tempImg);

        /// Contour detection
        // Convert color image to grayscale image
        cv::cvtColor(tempImg, tempImg, CV_BGR2GRAY);
        cv::blur(tempImg, blurImg, cv::Size(3, 3));
        
        std::cout << "Operation: Edge detection" << std::endl;
        cv::Canny(blurImg, blurImg, 100, 150);

        blurImg.copyTo(edgeImg);
        cv::imshow("Edge Image", edgeImg);
        cv::findContours(blurImg, _contours, _hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

        cv::Mat drawing = cv::Mat::zeros(blurImg.size(), CV_8UC3);
        
        cv::Mat ellipseImg = cv::Mat::zeros(blurImg.size(), CV_8UC3);

        for(int i = 0; i < _contours.size(); i++)
        {
            cv::Scalar color = Scalar( 255, 255, 0 );
            cv::drawContours( drawing, _contours, i, color, 2, 8, _hierarchy, 0, cv::Point() );
        }


        minRect.resize(_contours.size());
        minEllipse.resize(_contours.size());

        ellipse = true;
        if(ellipse)
        {
            detectEllipses(_contours, ellipseImg, &minRect, &minEllipse);
        }

        std::cout << "Contours Size: " << _contours.size() << std::endl;
        cv::imshow("Contour Image", drawing);
        cv::imwrite("ContoursImage" + num + ".png", drawing);
        cv::waitKey(1);

        *contours =  _contours;
        *hierarchy =  _hierarchy;
    }

}


void Processing::threshContImage(cv::Mat inImage, cv::Mat *outImage)
{
    // Temporary image for processing
    cv::Mat tempImg, blurImg;
    std::vector<std::vector<cv::Point> > _contours;
    std::vector<cv::Vec4i> _hierarchy;

    /// Thresholding
    cv::threshold(inImage, tempImg, 150, 180, 0);
    std::cout << "Thresholding Image" << std::endl;
    cv::imshow("Threshold Image", tempImg);

    /// Edge detection
    //cv::Canny(tempImg, tempImg, 100, 150);
    //cv::imshow("Edge Image", tempImg);

    /// Contour detection
    // Convert color image to grayscale image
    cv::cvtColor(tempImg, tempImg, CV_BGR2GRAY);
    cv::blur(tempImg, blurImg, cv::Size(3, 3));
    cv::Canny(blurImg, blurImg, 100, 150);
    cv::findContours(blurImg, _contours, _hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

    cv::Mat drawing = cv::Mat::zeros(blurImg.size(), CV_8UC3);
    for(int i = 0; i < _contours.size(); i++)
    {
        cv::Scalar color = Scalar( 255, 255, 0 );
        cv::drawContours( drawing, _contours, i, color, 2, 8, _hierarchy, 0, cv::Point() );
    }

    try
    {
        cv::RotatedRect roRect = cv::fitEllipse(drawing);
        std::cout << "rotated rectangle size" << roRect.size << std::endl;
    }
    catch(cv::Exception)
    {
        std::cerr << "The Fit Ellipse method failed" << std::endl;
    }


    std::cout << "Contours Size: " << _contours.size() << std::endl;
    cv::imshow("Contour Image", drawing);
    cv::imwrite("ContoursImage" + num + ".png", drawing);
    cv::waitKey(1);

}

void Processing::threshElliImage(cv::Mat inImage,
                     cv::Mat *outImage,
                     std::vector<cv::RotatedRect> *minRect,
                     std::vector<cv::RotatedRect> *minEllipse)
{


}

void Processing::detectEllipses(std::vector<std::vector<cv::Point> > _contours,
                    cv::Mat ellipseImg,
                    std::vector<cv::RotatedRect> *minRect,
                    std::vector<cv::RotatedRect> *minEllipse)
{

    //std::vector<cv::RotatedRect> retEllipse;
        if(ellipse)
        {
            try{
                for( int i = 0; i < _contours.size(); i++ )
                {
                    minRect->at(i) = cv::minAreaRect( Mat(_contours[i]) );
                        if( _contours[i].size() > 5 )
                        {
                            minEllipse->at(i) = cv::fitEllipse( Mat(_contours[i]) );
                    }
                }

                std::cout << minEllipse->size() << std::endl;
                string Result;          // string which will contain the result
                ostringstream convert;   // stream used for the conversion

                // This is the point of modification

                //cleanEllipse(*minEllipse, &retEllipse);

                for(int i = 0; i < minEllipse->size(); i++)
                {
                    convert << i;      // insert the textual representation of 'Number' in the characters in the stream
                    Result = convert.str(); // set 'Result' to the contents of the stream

                    cv::Scalar color = Scalar( 255, 255, 0 );
                    cv::ellipse(ellipseImg, minEllipse->at(i), color, 2, 7);

                    cv::Rect rRect = minEllipse->at(i).boundingRect();
                    cv::rectangle(ellipseImg, rRect, cv::Scalar(255, 0, 255));

                    //cv::putText(ellipseImg, Result, rRect.br(), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250) );

                    Result.clear();

                    std::cout << i << std::endl;
                }

                cv::imshow("Ellipse Image", ellipseImg);
                cv::imwrite("EllipseWithBounding" + num + ".png", ellipseImg);


            }
            catch(cv::Exception){
                std::cerr << "Ellipse detection is not working" << std::endl;
            }
        }

}
