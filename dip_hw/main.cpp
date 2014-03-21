#include <cstdlib>  // atof
#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "imageprocessing/ImageProcessing.h"

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "Command wrong." << std::endl;
        return -1;
    }

    cv::Mat image;
    image = cv::imread(argv[1], cv::IMREAD_COLOR);

    if (!image.data) {
        std::cout << "Could not open or find the image." << std::endl;
        return -1;
    }

    cv::Mat imageOut;

    double magnify = atof(argv[2]);
    if (magnify <= 0) {
        std::cout << "Zooming factor should be greater than zero." << std::endl;
        return -1;
    }
    double fx = magnify;
    double fy = magnify;
    cv::Size dsize(image.size().width * fx, image.size().height * fy);

    std::string interpolationType = argv[3];
    if (interpolationType == "nearest") {
        ImageProcessing::resize(image, imageOut, dsize, fx, fy, ImageProcessing::INTER_NEAREST);
    } else if (interpolationType == "bilinear") {
        ImageProcessing::resize(image, imageOut, dsize, fx, fy, ImageProcessing::INTER_LINEAR);
    } else {
        std::cout << "Command for interpolation is wrong." << std::endl;
        return -1;
    }

    cv::namedWindow("Original picture", cv::WINDOW_AUTOSIZE);
    cv::imshow("Original picture", image);

    cv::namedWindow("Enlarged picture", cv::WINDOW_AUTOSIZE);
    cv::imshow("Enlarged picture", imageOut);

    cv::waitKey(0);
    return 0;
}