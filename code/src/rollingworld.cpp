#include "rollingworld.h"

#include "Vector3f.h"
#include "extra.h"

#include <cstdlib>
#include <iostream>


using std::vector;
using std::cout;
using std::endl;

int s = 8;

float rand_height() {
    return  rand() / (float) RAND_MAX;
};

RollingWorld::RollingWorld() {
    landscape_data_ = vector<vector<float> >();
    num_rows = 24;
    num_cols = 24;
    x_extent = 15;
    z_extent = 15;
    cout << "Creeating rolling world" << endl;
    for (int r = 0; r < num_rows; r++) {
        landscape_data_.push_back(vector<float>());
        for (int c = 0; c < num_cols; c++) {
            landscape_data_[r].push_back(rand_height());
            cout << landscape_data_[r][c];
        }
    }
};

float RollingWorld::r_to_x(int r) {
    return (-0.5 + r/((float) num_rows)) * x_extent;
};

float RollingWorld::c_to_z(int c) {
    return (-0.5 + c/((float) num_cols)) * z_extent;    
};

Vector3f RollingWorld::point(int r, int c) {
    return Vector3f(r_to_x(r),
                    landscape_data_[r][c],
                    c_to_z(c));
};

Vector3f RollingWorld::normal(int r, int c) {
    return Vector3f(0, 1, 0);
};

void RollingWorld::drawTriangle(int r1, int c1, int r2, int c2, int r3, int c3) {
    Vector3f p1 = point(r1, c1);
    Vector3f p2 = point(r2, c2);
    Vector3f p3 = point(r3, c3);    

    Vector3f normal_v = Vector3f::cross(p3 - p1, p2 - p1).normalized();
    
    glNormal(normal_v);
    glVertex(p1);
    
    glNormal(normal_v);
    glVertex(p2);

    glNormal(normal_v);
    glVertex(p3);
};

// Draw the square [r, c] -> [r + 1, c + 1]
void RollingWorld::drawSquare(int r, int c) {
    if ((r + c) % 2 == 0) {
        drawTriangle(r, c,
                     r + 1, c,
                     r + 1 , c+ 1);
        drawTriangle(r + 1, c + 1,
                     r, c + 1,
                     r, c);
                     
    } else {
        drawTriangle(r + 1, c,
                     r + 1, c + 1,
                     r , c+ 1);
        drawTriangle(r, c + 1,
                     r, c,
                     r + 1, c);
    }
};

void RollingWorld::draw() {
    // Save State
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    // Plane is offset by -1
    glTranslatef(0, -1.0, 0);

    glBegin(GL_TRIANGLES);


    for (int r = 0; r < num_rows - 1; r++) {
        for (int c = 0; c < num_cols - 1; c++) {
            drawSquare(r, c);
        }
    }

    glEnd();

    // Revert state, etc.
    glPopMatrix();
    glPopAttrib();
};
