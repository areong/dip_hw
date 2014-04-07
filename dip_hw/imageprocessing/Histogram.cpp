#include "Histogram.h"
//#include <iostream>

Histogram::Histogram(const cv::Mat &src) {
    hasCalcCumHist = false;
    hasCalcInvCumHist = false;

    channels = src.channels();
    depth = 256;    // TODO
    pixels = src.cols * src.rows;
    
    // Create histogram arrays.
    // Initialize with zeros.
    hist = new double* [channels];
    for (int ch = 0; ch < channels; ch++) {
        hist[ch] = new double[depth];
        for (int i = 0; i < depth; i++) {
            hist[ch][i] = 0;
        }
    }
    
    // Calculate histogram.
    for (int p = 0; p < pixels; p++) {
        for (int ch = 0; ch < channels; ch++) {
            int index = src.data[channels * p + ch];
            hist[ch][index] += 1;
        }
    }

    // Normalize.    
    for (int i = 0; i < depth; i++) {
        for (int ch = 0; ch < channels; ch++) {
            hist[ch][i] /= pixels;
            //if (ch == 0)
            //    std::cout << i << "\t" << hist[ch][i] << std::endl;
        }
    }
}

Histogram::~Histogram() {
    for (int i = 0; i < channels; i++) {
        delete hist[i];
    }
    delete hist;

    if (hasCalcCumHist) {
        for (int i = 0; i < channels; i++) {
            delete cumHist[i];
        }
        delete cumHist;
    }

    if (hasCalcInvCumHist) {
        for (int i = 0; i < channels; i++) {
            delete invCumHist[i];
        }
        delete invCumHist;
    }
}

double Histogram::valueHist(int channel, int index) {
    if (!isChannelAndIndexValid(channel, index)) {
        return -1;
    } else {
        return hist[channel][index];
    }
}

void Histogram::calcCumHist() {
    hasCalcCumHist = true;

    // Create cummulative histogram arrays.
    // It is okey not to initialize the values.
    cumHist = new double* [channels];
    for (int ch = 0; ch < channels; ch++) {
        cumHist[ch] = new double[depth];
    }

    // Calculate.
    // First element.
    for (int ch = 0; ch < channels; ch++) {
        cumHist[ch][0] = hist[ch][0];        
    }
    // Later elements.
    for (int i = 1; i < depth; i++) {
        for (int ch = 0; ch < channels; ch++) {
            cumHist[ch][i] = cumHist[ch][i - 1] + hist[ch][i];
        }
    }

    // Normalize.
    for (int i = 0; i < depth; i++) {
        for (int ch = 0; ch < channels; ch++) {
            cumHist[ch][i] *= depth - 1;
            //if (ch == 0)
            //    std::cout << i << "\t" << cumHist[ch][i] << std::endl;
        }
    }
}

double Histogram::valueCumHist(int channel, int index) {
    if (!isChannelAndIndexValid(channel, index)) {
        return -1;
    } else {
        return cumHist[channel][index];
    }
}

void Histogram::calcInvCumHist() {
    hasCalcInvCumHist = true;

    // Create cummulative histogram arrays.
    // It is okey not to initialize the values.
    invCumHist = new int* [channels];
    for (int ch = 0; ch < channels; ch++) {
        invCumHist[ch] = new int[depth];
    }

    // First element.
    for (int ch = 0; ch < channels; ch++) {
        // Fill the invCumHist from 0 to cumHist[ch][0] with 0s.
        for (int ii = 0; ii <= cumHist[ch][0]; ii++) {
            invCumHist[ch][ii] = 0;
        }     
    }

    int iiLast[] = {0, 0, 0};

    // Later element.
    for (int ic = 1; ic < depth; ic++) {
        for (int ch = 0; ch < channels; ch++) {
            if ((int)cumHist[ch][ic] > iiLast[ch]) {
                // Fill the invCumHist from icLast + 1 to cumHist[ch][ic] with ic.
                for (int ii = iiLast[ch] + 1; ii <= cumHist[ch][ic]; ii++) {
                    invCumHist[ch][ii] = (int)ic;
                    //if (ch == 0)
                    //    std::cout << ii << "\t" << invCumHist[ch][ii] << std::endl;
                }   
                iiLast[ch] = (int)cumHist[ch][ic];
            } else {
                continue;
            }
        }
    }

    //for (int ii = 0; ii < depth; ii++) {
    //    std::cout << ii << "\t" << invCumHist[0][ii] << std::endl;
    //}
}

// Private:

bool Histogram::isChannelAndIndexValid(int channel, int index) {
    return (channel >= 0) && (channel < channels) && (index >= 0) && (index < depth);
}