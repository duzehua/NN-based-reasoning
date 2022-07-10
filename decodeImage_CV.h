#ifndef DECODEIMAGE_CV_H
#define DECODEIMAGE_CV_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Tensor.h"
#include "GINFO_VAR.h"

void getPixeldata_CV(const char* path, int flag, Tensor* pixel_ts);

#endif
