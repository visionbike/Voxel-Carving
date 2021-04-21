//
// Created by user on 2021-03-18.
//
#include <iostream>
#include"voxel_grid.hpp"

VoxelGrid::VoxelGrid(int _x_dims, int _y_dims, int _z_dims, const cv::Point3f &_origin) :
    m_grid(_x_dims * _y_dims * _z_dims)
{
    _initVoxelGrid(_x_dims, _y_dims, _z_dims, _origin);
}

VoxelGrid::~VoxelGrid()
{
    if (!m_grid.empty()) { m_grid.clear(); }
}

void VoxelGrid::carve(const std::vector<Camera> &_cams)
{
    for (int i = 0; i < _cams.size(); ++i) {
        std::vector<Voxel> new_grid;
        for (int j = 0; j < m_grid.size(); ++j) {
            cv::Point coord = _cams[i].project(m_grid[j]);

            // Skip scan voxels that are out of range of camera's FOV
            if (coord.x < 0 || coord.x >= _cams[i].getImageWidth() || coord.y < 0 || coord.y >= _cams[i].getImageHeight()) { continue; }

            // Only consider foreground
            if (_cams[i].getImageValue(coord.x, coord.y)) { new_grid.push_back(m_grid[j]); }
        }
        m_grid.clear();
        m_grid = new_grid;
    }
    std::cout << m_grid.size() << std::endl;
}

void VoxelGrid::saveAsXYZ(const std::string &_filename)
{
    std::fstream f(_filename, std::ios::out);
    assert(f.is_open());

    for (int i = 0; i < m_grid.size(); ++i) {
        f << m_grid[i].x << " " << m_grid[i].y << " " << m_grid[i].z << std::endl;
    }
    f.close();
}

void VoxelGrid::_initVoxelGrid(int _x_dims, int _y_dims, int _z_dims, const cv::Point3f &_origin)
{
    int k = 0;
    for (int x = 0; x < _x_dims; ++x) {
        for (int y = 0; y < _y_dims; ++y) {
            for (int z = 0; z < _z_dims; ++z) {
                m_grid[k].x = _origin.x + float(x);
                m_grid[k].y = _origin.y + float(y);
                m_grid[k].z = _origin.z + float(z);
                ++k;
            }
        }
    }
 }


