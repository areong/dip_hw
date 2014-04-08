#include "ImageProcessing.h"
#include <cmath>
#include <iostream>
#include "Histogram.h"

ImageProcessing::ImageProcessing() {

}

ImageProcessing::~ImageProcessing() {

}

void ImageProcessing::histogramEqualization(const cv::Mat &src, cv::Mat &dst) {
    int w = src.cols;
    int h = src.rows;
    int pixels = w * h;
    int channels = src.channels();

    // Calculate histogram.
    Histogram* hist = new Histogram(src);
    hist->calcCumHist();

    // Create destination image.
    dst.create(h, w, src.type());

    // Fill the destination image.
    for (int p = 0; p < pixels; p++) {
        for (int ch = 0; ch < channels; ch++) {
            int value = src.data[channels * p + ch];
            dst.data[channels * p + ch] = (int)hist->valueCumHist(ch, value);
        }
    }
}

void ImageProcessing::histogramSpecification(const cv::Mat &src, const cv::Mat &src2, cv::Mat &dst) {
    int w = src.cols;
    int h = src.rows;
    int pixels = w * h;
    int channels = src.channels();

    // Calculate histogram.
    Histogram* hist = new Histogram(src);
    hist->calcCumHist();
    Histogram* hist2 = new Histogram(src2);
    hist2->calcInvCumHist();

    // Create destination image.
    dst.create(h, w, src.type());

    // Fill the destination image.
    for (int p = 0; p < pixels; p++) {
        for (int ch = 0; ch < channels; ch++) {
            int value = src.data[channels * p + ch];
            int interValue = (int)hist->valueCumHist(ch, value);
            dst.data[channels * p + ch] = (int)hist2->valueInvCumHist(ch, interValue);
        }
    }

}

void ImageProcessing::resize(const cv::Mat &src, cv::Mat &dst, cv::Size &dsize,
                        double fx, double fy, ImageProcessing::Interpolation interpolation) {

    int w_s = src.cols;
    int h_s = src.rows;
    int w_d = dsize.width;
    int h_d = dsize.height;
    int type = src.type();
    int channels = src.channels();

    // Size or scaling of dst is specified.
    if (fx == 0) {
        fx = (double)dsize.width/src.cols;        
    } else if (fy == 0) {
        fy = (double)dsize.height/src.rows;
    }

    // Create dst.
    dst.create(dsize, type);

    // Assure dst is not empty.
    if (!dst.empty()) {

        // Each interpolation method.
        switch(interpolation) {
        case INTER_NEAREST:

            for (int y = 0; y < h_d; y++) {
                for (int x = 0; x < w_d; x++) {
                    for (int ch = 0; ch < channels; ch++) {
                        dst.data[channels * (w_d * y + x) + ch] = 
                            src.data[channels * (w_s * (int)(y/fy + 0.5)
                                                     + (int)(x/fx + 0.5)) + ch];
                    }
                }
            }

            break;
        case INTER_LINEAR:

            for (int y = 0; y < h_d; y++) {
                for (int x = 0; x < w_d; x++) {
                    
                    // Ratios and weights.
                    int x_fx = (int)(x/fx);
                    int y_fy = (int)(y/fy);
                    double a = x_fx + 1 - x/fx;
                    double b = y_fy + 1 - y/fy;

                    // Naming: [x],[y], or in this order:
                    //          00    01
                    //          10    11
                    int x00, y00, x10, y10, x01, y01, x11, y11;
                    double x_mod_fx = fmod(x, fx);
                    double y_mod_fy = fmod(y, fy);
                    if (x_mod_fx != 0) {
                        if (y_mod_fy != 0) {
                            // Between four different pixels.
                            x00 = x_fx;
                            x01 = x_fx + 1;
                            x10 = x_fx;
                            x11 = x_fx + 1;
                            y00 = y_fy;
                            y01 = y_fy;
                            y10 = y_fy + 1;
                            y11 = y_fy + 1;
                        } else {
                            // Between two horizontal neighboring pixels.
                            x00 = x_fx;
                            x01 = x_fx + 1;
                            x10 = x_fx;
                            x11 = x_fx + 1;
                            y00 = y_fy;
                            y01 = y_fy;
                            y10 = y_fy;
                            y11 = y_fy;
                        }
                    } else {
                        if (y_mod_fy != 0) {
                            // Between two vertical neighboring pixels.
                            x00 = x_fx;
                            x01 = x_fx;
                            x10 = x_fx;
                            x11 = x_fx;
                            y00 = y_fy;
                            y01 = y_fy;
                            y10 = y_fy + 1;
                            y11 = y_fy + 1;
                        } else {
                            // On an existing pixel.
                            x00 = x_fx;
                            x01 = x_fx;
                            x10 = x_fx;
                            x11 = x_fx;
                            y00 = y_fy;
                            y01 = y_fy;
                            y10 = y_fy;
                            y11 = y_fy;
                        }
                    }
                    
                    // Assign colors.
                    for (int ch = 0; ch < channels; ch++) {
                        dst.data[channels * (w_d * y + x) + ch] = 
                            a     * b     * src.data[channels * (w_s * y00 + x00) + ch] +
                            (1-a) * b     * src.data[channels * (w_s * y01 + x01) + ch] +
                            a     * (1-b) * src.data[channels * (w_s * y10 + x10) + ch] +
                            (1-a) * (1-b) * src.data[channels * (w_s * y11 + x11) + ch] ;
                    }
                }
            }

            break;
        }
    }
}