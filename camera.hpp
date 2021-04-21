//
// Created by user on 2021-03-16.
//
#pragma once
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <iostream>
#include <fstream>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "voxel.hpp"

class Camera
{
public:
    Camera() = default;
    Camera(const cv::Mat &_K, const cv::Mat &_T, const cv::Mat &_sil);
    cv::Point project(const Voxel &_v) const;
    int getImageWidth() const;
    int getImageHeight() const;
    uchar getImageValue(int _x, int _y) const;
    ~Camera();

public:
    cv::Mat m_K;            // Intrinsic matrix (3x3)
    cv::Mat m_T;            // Extrinsic matrix (3x4)
    cv::Mat m_silhouette;   // Silhouette image
    cv::Mat m_P;            // Projection matrix (3x4)
};

void splitStringToFloat(const std::string &_s, const char *_delim, std::vector<float> &_o);
void loadCameras(const char *_fn_params, const char *_fd_sils, std::vector<Camera>& _cams);

#endif //_CAMERA_H_
