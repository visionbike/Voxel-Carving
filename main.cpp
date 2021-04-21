#include "voxel_grid.hpp"
#include "camera.hpp"

void doCarving(const char *_fn_params, const char *_fd_sils) {

    // Setup cameras
    std::vector<Camera> cameras;
    loadCameras(_fn_params, _fd_sils, cameras);
    std::cout << cameras.size() << std::endl;

    // Workspace initiation
    int xmin = -50, ymin = -50, zmin = 0;
    int xmax = 50, ymax = 50, zmax = 100;

    // Voxel origin
    cv::Point3f origin(-50, -50, 0);
    int xdims = xmax - xmin;
    int ydims = ymax - ymin;
    int zdims = zmax - zmin;

    // Initiate voxel grid
    VoxelGrid grid(xdims, ydims, zdims, origin);

    // Voxel carving
    grid.carve(cameras);

    // Save xyz file
    std::string fnSave = std::string(_fd_sils) + ".xyz";
    grid.saveAsXYZ(fnSave);
}

int main() {
    doCarving("Camera Parameter.txt", "Teapot");
    doCarving("Camera Parameter.txt", "Last");
    doCarving("Camera Parameter.txt", "Monkey");
    doCarving("Camera Parameter.txt", "Bird");
    return 0;
}
