//
// Created by user on 2021-03-18.
//
#pragma once
#ifndef _VOXEL_GRID_H_
#define _VOXEL_GRID_H_

#include "voxel.hpp"
#include "camera.hpp"

class VoxelGrid
{
public:
    VoxelGrid() = default;
    VoxelGrid(int _x_dims, int _y_dims, int _z_dims, const cv::Point3f &_origin);
    ~VoxelGrid();
    void carve(const std::vector<Camera> &_cams);
    void saveAsXYZ(const std::string &_filename);
private:
    void _initVoxelGrid(int _x_dims, int _y_dims, int _z_dims, const cv::Point3f &_origin);
private:
    std::vector<Voxel> m_grid;
};

#endif //_VOXEL_GRID_H_
