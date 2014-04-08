#ifndef IMAGEPROCESSING_IMAGEPROCESSING_H
#define IMAGEPROCESSING_IMAGEPROCESSING_H
#include <opencv2/core/core.hpp>

class ImageProcessing {
public:
    ImageProcessing();
    ~ImageProcessing();

    static void histogramEqualization(const cv::Mat &src, cv::Mat &dst);
    static void histogramSpecification(const cv::Mat &src, const cv::Mat &src2, cv::Mat &dst);

    enum Interpolation {
        INTER_NEAREST,
        INTER_LINEAR
    };
    static void resize(const cv::Mat &src, cv::Mat &dst, cv::Size &dsize,
                       double fx = 1, double fy = 1,
                       Interpolation interpolation = INTER_LINEAR);
};

#endif