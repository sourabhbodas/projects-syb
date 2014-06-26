#ifndef PROCESSINGCLASS_H
#define PROCESSINGCLASS_H

#include "all_includes.h"

class Processing
{

public:
    bool ellipse;
    int expectedCircles;

    int lowerThreshBound;
    int upperThreshBound;

    int threshChangeRange;

    std::vector<cv::RotatedRect> minRect;
    std::vector<cv::RotatedRect> minEllipse;

    std::vector<cv::RotatedRect> retEllipse;
    std::vector<cv::RotatedRect> tempEllipse;

    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    std::string num;


    // Constructor
    Processing();

    // Destructor
    ~Processing();

    // Capture Image using OpenCV

    cv::Mat captureFrame ();
    cv::Mat loadFrame ();

    // Preproecssing of the images
    cv::Mat preprocessImage(cv::Mat inImage);

    /*
     * denoise the captured image
     */
    void denoiseImage(cv::Mat inImage, cv::Mat *outImage);

    /*
     * Threshold and find contours in the input image
     */
    void threshContImage(cv::Mat inImage,
                         cv::Mat *outImage,
                         std::vector<std::vector<cv::Point> > *contours,
                         std::vector<cv::Vec4i> *hierarchy);

    /*
     * Threshold and find contours in the input image
     */
    void threshContImage(cv::Mat inImage, cv::Mat *outImage);

    /*
     * Threshold and detect Ellipse in the input image
     */
    void threshElliImage(cv::Mat inImage,
                         cv::Mat *outImage,
                         std::vector<cv::RotatedRect> *minRect,
                         std::vector<cv::RotatedRect> *minEllipse);

    /*
     * Detect Ellipse - internal method
     */
    void detectEllipses(std::vector<std::vector<cv::Point> > contours,
                        cv::Mat ellipseImg,
                        std::vector<cv::RotatedRect> *minRect,
                        std::vector<cv::RotatedRect> *minEllipse);


};

#endif
