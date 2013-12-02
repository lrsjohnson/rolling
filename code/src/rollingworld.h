#ifndef ROLLING_WORLD_H
#define ROLLING_WORLD_H

#include <vector>
#include "Vector3f.h"

#include "rollingball.h"

using std::vector;

class RollingWorld {
 public:
    RollingWorld();
    void draw();

    void getCollisions(RollingBall* ball, vector<Vector3f>* collision_points);
    
    Vector3f sphere_c;

    void computeNormals();
 private:
    Vector3f closestPtOnTriangle(Vector3f p, Vector3f a, Vector3f b, Vector3f c);
    
    Vector3f point(int r, int c);
    float r_to_x(int r);
    float c_to_z(int z);

    Vector3f normal(int r, int c);
    
    void drawTriangle(int r1, int c1,
                      int r2, int c2,
                      int r3, int c3);
    void drawSquare(int r, int c);

    void getCollisionsForTriangle(int r1, int c1,
                                  int r2, int c2,
                                  int r3, int c3,
                                  RollingBall* ball,
                                  vector<Vector3f>* collision_points);
    void getCollisionsForSquare(int r, int c,
                                RollingBall* ball,
                                vector<Vector3f>* collision_points);

    
    float x_extent;
    float z_extent;
    int num_rows;
    int num_cols;
    vector<vector<float> > landscape_data_;

    vector<vector<Vector3f> > normals_;
};

#endif // ifndef ROLLING_WORLD_H
