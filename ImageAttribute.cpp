//
//  ICAOphotoquality
//
//  Copyright © 2017 Marcelo Cobias. All rights reserved.
//
#include "ImageAttribute.hpp"

ImageAttribute::ImageAttribute(Mat image)
{
  this->brightness = this->calcBrightness(image);
  this->contrast = this->calcContrast(image);
  this->blur = this->calcBlur(image);
}

float ImageAttribute::calcBrightness(Mat image)
{
    Mat imageGray = Util::coloredToGray(image);
    multiply(imageGray, Scalar(0.2126, 0.7152, 0.0722), imageGray);
    Scalar scalar = sum(imageGray);
    return (scalar.val[0] + scalar.val[1] + scalar.val[2]) / imageGray.size().area();
}

float ImageAttribute::calcContrast(Mat image)
{
    float contrast = 0;
    float intensity = 0;
    float brightness = ImageAttribute::calcBrightness(image);

    for(int i = 0; i < image.rows; i++ )
    {
        for( int j = 0; j < image.cols; j++ )
        {
            intensity += (image.at<cv::Vec3b>(i,j)[0] * 0.0722) + (image.at<cv::Vec3b>(i,j)[1] * 0.7152) + (image.at<cv::Vec3b>(i,j)[2] * 0.2126); // Blue, green and red
            intensity /= 255.0;
            contrast += pow((brightness - intensity), 2);
        }
    }
    contrast /= image.size().area();
    return contrast;
}

float ImageAttribute::calcBlur(Mat image)
{
    short max = -32767;
    Mat imageGray = Util::coloredToGray(image);
    Mat in(imageGray.rows, imageGray.cols, CV_8U);
    Mat out(imageGray.rows, imageGray.cols, CV_16S);
    in = imageGray.clone();

    out = cv::Scalar::all(0);
    cv::Laplacian(in, out, CV_16S, 1);
    for(int i = (int)(out.rows * 0.15); i < (int)(out.rows * 0.85); i++ )
    {
        for (int j = (int)(out.cols * 0.15); j < (int)(out.cols * 0.85); j++ )
        {
            if (out.at<short int>(i, j) > max)
            {
                max = out.at<short int>(i, j);
            }
        }
    }
    return max;
}

float ImageAttribute::getBrightness()
{
    return this->brightness;
}

float ImageAttribute::getContrast()
{
    return this->contrast;
}
float ImageAttribute::getBlur()
{
    return this->blur;
}

ImageAttribute::~ImageAttribute(){}

// add func calcule nivel noise (Histogram)
