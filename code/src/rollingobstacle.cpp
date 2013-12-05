#include "rollingobstacle.h"

#include <iostream>

using std::cout;
using std::endl;

RollingObstacle::RollingObstacle() {
    color_ = Vector4f(0.8, 0.6, 0.6, 1.0);
    paint_color_ = Vector4f(0.5, 0.5, 0.7, 1.0);
};

void RollingObstacle::getCollisionsForTriangle(Vector3f& p1,
                                           Vector3f& p2,
                                           Vector3f& p3,
                                           RollingBall* ball,
                                           vector<Vector3f>* collision_points) {
    //    cout << "Tri"<< endl;
    //    p1.print();
    //    p2.print();
    //    p3.print();
        
    Vector3f cp = closestPtOnTriangle(ball->center_, p1, p2, p3);
    float dist = (cp - ball->center_).abs();
    if (dist < ball->radius() * 1.05) {
        cout << "Collision with box" << endl;
        color_ = (color_ + paint_color_) / 2.0;        
        collision_points->push_back(cp);
    }

    Vector3f normal_v = Vector3f::cross(p3 - p1, p2 - p1);//.normalized();

    glBegin(GL_TRIANGLES);
    glNormal(normal_v);
    glVertex(p1);
    glVertex(p2);
    glVertex(p3);

    glEnd();
};

BoxObstacle::BoxObstacle(Vector3f center_vec, float w_x, float w_y, float w_z) {
    center = center_vec;
    w_x_ = w_x;
    w_y_ = w_y;
    w_z_ = w_z;
};

void BoxObstacle::getCollisionsForFace(RollingBall* ball, vector<Vector3f>* collision_points,
                                       Vector3f& center,
                                       Vector3f& u_dir, float u,
                                       Vector3f& v_dir, float v) {
    Vector3f du = u_dir * u / 2.0;
    Vector3f dv = v_dir * v / 2.0;    
    Vector3f p1 = center + du + dv;
    Vector3f p2 = center + du - dv;
    Vector3f p3 = center - du - dv;
    Vector3f p4 = center - du + dv;        

    getCollisionsForTriangle(p1, p2, p3, ball, collision_points);
    getCollisionsForTriangle(p3, p4, p1, ball, collision_points);    
};

void BoxObstacle::getCollisionsForObstacle(RollingBall* ball, vector<Vector3f>* collision_points) {
    Vector3f x = Vector3f(1, 0, 0);
    Vector3f y = Vector3f(0, 1, 0);
    Vector3f z = Vector3f(0, 0, 1);

    Vector3f c_pos_x = center + x * w_x_ / 2.0;
    Vector3f c_neg_x = center - x * w_x_ / 2.0;    
    getCollisionsForFace(ball, collision_points,
                         c_pos_x,
                         y, w_y_,
                         z, w_z_);
    getCollisionsForFace(ball, collision_points,
                         c_neg_x,
                         y, -w_y_,
                         z, -w_z_);
    
    Vector3f c_pos_y = center + y * w_y_ / 2.0;
    Vector3f c_neg_y = center - y * w_y_ / 2.0;    
    getCollisionsForFace(ball, collision_points,
                         c_pos_y,
                         x, w_x_,
                         z, w_z_);
    getCollisionsForFace(ball, collision_points,
                         c_neg_y,
                         x, -w_x_,
                         z, -w_z_);

    Vector3f c_pos_z = center + z * w_z_ / 2.0;
    Vector3f c_neg_z = center - z * w_z_ / 2.0;    
    getCollisionsForFace(ball, collision_points,
                         c_pos_z,
                         x, w_x_,
                         y, w_y_);
    getCollisionsForFace(ball, collision_points,
                         c_neg_z,
                         x, -w_x_,
                         y, -w_y_);
};

void BoxObstacle::draw() {
    cout << "drawing box!" << endl;
    glPushMatrix();
    glTranslatef(center[0], center[1], center[2]);
    glScaled(w_x_, w_y_, w_z_);
    glutSolidCube(1.0);
    glPopMatrix();
};


// Based on calculations from "Real Time Collision Detectino Book"
Vector3f RollingObstacle::closestPtOnTriangle(Vector3f& p, Vector3f& a, Vector3f& b, Vector3f& c) {
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
