//
// Created by user on 2021-03-16.
//
#include "camera.hpp"

Camera::Camera(const cv::Mat &_K, const cv::Mat &_T, const cv::Mat &_sil)
{
    assert(!_K.empty() && !_T.empty() && !_sil.empty());
    _K.copyTo(m_K);
    _T.copyTo(m_T);
    _sil.copyTo(m_silhouette);
    m_P = m_K * m_T;
}

Camera::~Camera()
{
    m_K.release();
    m_T.release();
    m_P.release();
    m_silhouette.release();
}

cv::Point Camera::project(const Voxel &_v) const {
    cv::Mat projected(3, 1, CV_32FC1);
    cv::Mat voxel(4, 1, CV_32FC1);

    auto projectedPtr = projected.ptr<float>();
    auto voxelPtr     = voxel.ptr<float>();

    voxelPtr[0] = _v.x;
    voxelPtr[1] = _v.y;
    voxelPtr[2] = _v.z;
    voxelPtr[3] = 1.f;

    // Project the voxel coord back using corresponding projection matrix
    projected = m_P * voxel;

    // The divide by the w component
    cv::Point prj;
    prj.x = (int)(projectedPtr[0] / projectedPtr[2]);
    prj.y = (int)(projectedPtr[1] / projectedPtr[2]);

    return prj;
}

int Camera::getImageWidth() const
{
    return m_silhouette.cols;
}

int Camera::getImageHeight() const
{
    return m_silhouette.rows;
}

uchar Camera::getImageValue(int _x, int _y) const
{
    return m_silhouette.at<uchar>(_y, _x);
}

void loadCameras(const char *_fn_params, const char *_fd_sils, std::vector<Camera>& _cams)
{
    // ******************************************************************* //
    // Load camera' s parameters
    std::fstream f(_fn_params, std::ios::in);
    assert(f.is_open());

    std::vector<cv::Mat> Ts;    // Extrinsics vector
    cv::Mat K;                  // Intrinsic matrix
    int sizeK[] = { 3, 3 };      // size of K
    int sizeT[] = { 3, 4 };      // size of P

    int count = 0;
    std::string ln;
    std::vector<float> floats;
    while (std::getline(f, ln)) {
        ++count;
        if (count % 5 == 0 || count % 5 == 1) { continue; }
        splitStringToFloat(ln, " ", floats);    // Get float value vector from string

        if (count == 4) {
            // Get K
            K.push_back(floats);
            K = K.reshape(1, 2, sizeK);
            floats.clear();
        }
        else if (count % 5 == 4) {
            // Get T
            cv::Mat T;
            T.push_back(floats);
            Ts.push_back(T.reshape(1, 2, sizeT));
            floats.clear();
        }
    }
    f.close();

    // ******************************************************************* //
    // Load silhouette image filenames
    std::vector<std::string> fns;
    cv::glob(std::string(_fd_sils) + "/*.bmp", fns, false);

    assert(fns.size() == Ts.size());

    // ******************************************************************* //
    // Get list of Cameras
    if (!_cams.empty()) { _cams.clear(); }
    for (int i = 0; i < Ts.size(); ++i) {
        cv::Mat img = cv::imread(fns[i], cv::IMREAD_ANYDEPTH);
        _cams.push_back(Camera(K, Ts[i], img));
    }
}

void splitStringToFloat(const std::string &_s, const char *_delim, std::vector<float> &_o)
{
    size_t start, end = 0;
    while ((start = _s.find_first_not_of(_delim, end)) != std::string::npos) {
        end = _s.find(_delim, start);
        _o.push_back(std::stof(_s.substr(start, end - start)));
    }
}

