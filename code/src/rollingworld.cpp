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
    num_rows = 40;
    num_cols = 20;
    x_extent = 30;
    z_extent = 15;
    cout << "Creeating rolling world" << endl;
    for (int r = 0; r < num_rows; r++) {
        landscape_data_.push_back(vector<float>());
        for (int c = 0; c < num_cols; c++) {
            landscape_data_[r].push_back(rand_height() - 0.3 * abs(r % (num_rows / 4) - num_rows / 8));
        }
    }
};

// Based on calculations from "Real Time Collision Detectino Book"
Vector3f RollingWorld::closestPtOnTriangle(Vector3f p, Vector3f a, Vector3f b, Vector3f c) {
    Vector3f ab = b - a;
    Vector3f ac = c - a;

    // Check if P in vertex region outside A
    Vector3f ap = p - a;
    float d1 = Vector3f::dot(ab, ap);
    float d2 = Vector3f::dot(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return a; // barycentric coordinates (1,0,0)
    
    // Check if P in vertex region outside B
    Vector3f bp = p - b;
    float d3 = Vector3f::dot(ab, bp);
    float d4 = Vector3f::dot(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return b; // barycentric coordinates (0,1,0)
    
    // Check if P in edge region of AB, if so return projection of P onto AB
    float vc = d1*d4 - d3*d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v = d1 / (d1 - d3);
        return a + v * ab; // barycentric coordinates (1-v,v,0) 
    }
    
    // Check if P in vertex region outside C
    Vector3f cp = p - c;
    float d5 = Vector3f::dot(ab, cp);
    float d6 = Vector3f::dot(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return c; // barycentric coordinates (0,0,1)
    
    // Check if P in edge region of AC, if so return projection of P onto AC
    float vb = d5*d2 - d1*d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w = d2 / (d2 - d6);
        return a + w * ac; // barycentric coordinates (1-w,0,w)
    }
    
    // Check if P in edge region of BC, if so return projection of P onto BC
    float va = d3*d6 - d5*d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return b + w * (c - b); // barycentric coordinates (0,1-w,w)
    }
    
    // P inside face region. Compute Q through its barycentric coordinates (u,v,w)
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    return a + ab * v + ac * w; //=u*a+v*b+w*c,u=va*denom=1.0f-v-w
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
    
    glBegin(GL_TRIANGLES);
    glNormal(normal_v);
    glVertex(p1);
    
    glNormal(normal_v);
    glVertex(p2);

    glNormal(normal_v);
    glVertex(p3);

    glEnd();
    /*
    Vector3f cp = closestPtOnTriangle(sphere_c, p1, p2, p3);
    float dist = (cp - sphere_c).abs();
    cout << dist << endl;
    if (dist < 1.0) {
        glDisable(GL_LIGHTING);
        glBegin(GL_LINES);
        glColor3f(1.0, 0, 0);
        glVertex(cp);
        glColor3f(1.0, 0, 0);
        glVertex(sphere_c);
        glEnd();
        glEnable(GL_LIGHTING);        
    }
        
    glPushMatrix();
    glTranslatef(cp[0], cp[1], cp[2]);
    glutSolidCube(0.1);
    glPopMatrix();
    */
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

void RollingWorld::getCollisionsForTriangle(int r1, int c1,
                                            int r2, int c2,
                                            int r3, int c3,
                                            RollingBall* ball,
                                            vector<Vector3f>* collision_points) {
    Vector3f p1 = point(r1, c1);
    Vector3f p2 = point(r2, c2);
    Vector3f p3 = point(r3, c3);    

    Vector3f cp = closestPtOnTriangle(ball->center_, p1, p2, p3);
    float dist = (cp - sphere_c).abs();
    if (dist < ball->radius()) {
        collision_points->push_back(cp);
    }
};



void RollingWorld::getCollisionsForSquare(int r, int c,
                                          RollingBall* ball,
                                          vector<Vector3f>* collision_points) {
    if ((r + c) % 2 == 0) {
        getCollisionsForTriangle(r, c,
                                 r + 1, c,
                                 r + 1 , c+ 1,
                                 ball,
                                 collision_points);
        getCollisionsForTriangle(r + 1 , c + 1,
                                 r, c + 1,
                                 r, c,
                                 ball,
                                 collision_points);
    } else {
        getCollisionsForTriangle(r + 1, c,
                                 r + 1, c + 1,
                                 r , c+ 1,
                                 ball,
                                 collision_points);
        getCollisionsForTriangle(r, c + 1,
                                 r, c,
                                 r + 1, c,
                                 ball,
                                 collision_points);
    }
};

void RollingWorld::getCollisions(RollingBall* ball, vector<Vector3f>* collision_points) {
    for (int r = 0; r < num_rows - 1; r++) {
        for (int c = 0; c < num_cols - 1; c++) {
            getCollisionsForSquare(r, c, ball, collision_points);
        }
    }
}

void RollingWorld::draw() {
    // Save State
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    // Plane is offset by -1
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
