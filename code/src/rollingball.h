#ifndef ROLLING_BALL_H
#define ROLLING_BALL_H

#include "Vector3f.h"
#include "Matrix4f.h"
#include "extra.h"
#include <iostream>
#include <vector>
using namespace std;

class RollingBall {
  public:
    RollingBall(Vector3f center, float r);
    ~RollingBall();
    bool debug_sphere;

    void draw();

    float radius() { return radius_; };

    Matrix4f rotation_;
    float axis_rotation_;
    Vector3f velocity_;
    Vector3f center_;

    Vector3f n_vel_;
    Vector3f t_vel_;

    vector<Vector3f> collision_points;
    Vector3f avg_collision;

    GLuint loadBMP_custom(const char * imagepath);
    GLuint texture;
    GLuint getTextureName();

    Matrix4f last_rotation_delta;

  private:
    float radius_;
    GLuint textureName;
    char* textureFileName;
    bool textureActive;
    unsigned char * m_data; // Actual RGB data.
    unsigned int m_width;
    unsigned int m_height;
    void drawSphere();
    void drawDebugSphere();

    void drawSphereFace(int p_recurse, double p_radius, Vector3f a, Vector3f b, Vector3f c);


  
    static const int RENDER_SLICES;
    static const int RENDER_STACKS;   
};

#endif // ROLLING_BALL_H
