#ifndef IMAGEPROCESSING_HISTOGRAM_H
#define IMAGEPROCESSING_HISTOGRAM_H
#include <opencv2/core/core.hpp>

class Histogram {
public:
    Histogram(const cv::Mat &src);
    ~Histogram();
    double valueHist(int channel, int index);
    void calcCumHist();
    double valueCumHist(int channel, int index);
    void calcInvCumHist();
    int valueInvCumHist(int channel, int index);

private:
    double** hist;
    double** cumHist;
    int** invCumHist;

    bool hasCalcCumHist;
    bool hasCalcInvCumHist;

    int channels;
    int depth;
    int pixels;

    bool isChannelAndIndexValid(int channel, int index);
};

#endif