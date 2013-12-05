#include "rollingworld.h"

#include "Vector3f.h"
#include "Vector4f.h"
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
    num_rows = 180;
    num_cols = 180;
    x_extent = 120;
    z_extent = 120;
    cout << "Creating rolling world" << endl;
    for (int r = 0; r < num_rows; r++) {
        landscape_data_.push_back(vector<float>());
        for (int c = 0; c < num_cols; c++) {
            landscape_data_[r].push_back(0.4 * rand_height() -
                                         0.6 * (sin(r /16.0 * 2 * M_PI)) -
                                         0.8 * (sin((r + c) / 40.0 * 2 * M_PI))
                                         - 0.25 * (r - 0.6 * abs(c - num_cols / 2.0)));
        }
    }
    computePoints();
    computeNormals();
    computeColors();

    obstacles_.push_back(new BoxObstacle(Vector3f(8, 0, 0), 10.0, 100.0, 10.0));

    paint_color_ = Vector4f(0.5, 0.5, 0.7, 1.0);
};

void RollingWorld::computePoints() {
    points_.clear();
    for (int ri = 0; ri < num_rows; ri++) {
        vector<Vector3f> row_vec;
	for (int ci = 0; ci < num_cols; ci++) {
	    row_vec.push_back(Vector3f(r_to_x(ri),
                                       height(ri, ci),
                                       c_to_z(ci)));
	}
        points_.push_back(row_vec);
    }
};

void RollingWorld::computeColors() {
    colors_.clear();
    for (int ri = 0; ri < num_rows; ri++) {
        vector<Vector4f> row_vec;
	for (int ci = 0; ci < num_cols; ci++) {
	    float ht = height(ri, ci);
            Vector4f v_color = (ht - -50)/50.0 * Vector4f(0, 1, 0, 1)  + -ht/50.0 * Vector4f(0, 0, 1, 1);
            v_color = Vector4f(0.7, 0.8, 0.72, 1.0) + rand_height() * Vector4f(-0.2, -0.2, -0.2, 0);
	    row_vec.push_back(v_color);
	}
        colors_.push_back(row_vec);
    }
};

// Based on calculations from "Real Time Collision Detectino Book"
Vector3f RollingWorld::closestPtOnTriangle(Vector3f& p, Vector3f& a, Vector3f& b, Vector3f& c) {
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

void RollingWorld::handleClick(Vector3f clickedPoint) {
    int r = x_to_r(clickedPoint[0]);
    int c = z_to_c(clickedPoint[2]);
    if (rc_in_bounds(r, c)) {
        makeLandRise(r, c, 1.0);
    }
};

// Assumes r and c are in bounds.
void RollingWorld::makeLandRise(int r, int c, float amount) {
    landscape_data_[r][c] += amount;
    int h = height(r, c);
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            if (rc_in_bounds(r+i, c+j) && h > height(r+i, c+j) + 1) {
                makeLandRise(r+i, c+j, amount * 0.9);
            }
        }
    }
};

bool RollingWorld::rc_in_bounds(int r, int c) {
    return 0 <= r && r < num_rows && 0 <= c && c < num_cols;
};

int RollingWorld::x_to_r(float x) {
    return (int) num_rows * (x / x_extent + 0.5);
};

int RollingWorld::z_to_c(float z) {
    return (int) num_cols * (z / z_extent + 0.5);
};



float RollingWorld::r_to_x(int r) {
    return (-0.5 + r/((float) num_rows)) * x_extent;
};

float RollingWorld::c_to_z(int c) {
    return (-0.5 + c/((float) num_cols)) * z_extent;    
};

float RollingWorld::height(int r, int c) {
    return landscape_data_[r][c];
};

// returns -1 if not in bounds.
float RollingWorld::height_at_xz(float x, float z) {
    int r = x_to_r(x);
    int c = z_to_c(z);
    if (!rc_in_bounds(r, c)) {
        return -1;
    }
    return height(r, c);
};

Vector4f& RollingWorld::color(int r, int c) {
    return colors_[r][c];
};


Vector3f& RollingWorld::point(int r, int c) {
    return points_[r][c];
};

void RollingWorld::computeNormals() {
    normals_.clear();
    for (int ri = 0; ri < num_rows; ri++) {
        vector<Vector3f> row_vec;
	for (int ci = 0; ci < num_cols; ci++) {
	    int num_neighbors = 0;
	    Vector3f sum(0, 0, 0);
	    Vector3f pt = point(ri, ci);
	    if (ri > 0 && ci > 0) {
		sum += Vector3f::cross(point(ri - 1, ci) - pt,
                                       point(ri, ci - 1) - pt);
		num_neighbors++;
	    }
	    if (ri > 0 && ci < num_cols -1) {
		sum += Vector3f::cross(point(ri, ci + 1) - pt,
                                       point(ri - 1, ci) - pt);
		num_neighbors++;
	    }
	    if (ri < num_rows - 1 && ci > 0) {
		sum += Vector3f::cross(point(ri, ci - 1) - pt,
                                       point(ri + 1, ci) - pt);
		num_neighbors++;
	    }
	    if (ri < num_rows - 1 &&  ci < num_cols -1) {
		sum += Vector3f::cross(point(ri + 1, ci) - pt,
                                       point(ri, ci + 1) - pt);
		num_neighbors++;
	    }
	    row_vec.push_back(-sum / ((float) num_neighbors));
	}
        normals_.push_back(row_vec);
    }
};

Vector3f RollingWorld::normal(int r, int c) {
    return normals_[r][c];
};

void RollingWorld::drawTriangle(int r1, int c1, int r2, int c2, int r3, int c3) {
    Vector3f& p1 = point(r1, c1);
    Vector3f& p2 = point(r2, c2);
    Vector3f& p3 = point(r3, c3);

    Vector3f normal_v = Vector3f::cross(p3 - p1, p2 - p1);//.normalized();
    
    glBegin(GL_TRIANGLES);
    //    glShadeModel(GL_SMOOTH);
    Vector4f& color_1 = color(r1, c1);
    Vector4f& color_2 = color(r2, c2);
    Vector4f& color_3 = color(r3, c3);
    
    glNormal(normal_v);
    glColor3f(color_1[0], color_1[1], color_1[2]);
    glVertex(p1);

    glColor3f(color_2[0], color_2[1], color_2[2]);    
    glVertex(p2);

    glColor3f(color_3[0], color_3[1], color_3[2]);        
    glVertex(p3);

    glEnd();
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
    Vector3f cp = closestPtOnTriangle(ball->center_,
                                      point(r1, c1),
                                      point(r2, c2),
                                      point(r3, c3));
    float dist = (cp - sphere_c).abs();
    if (dist < ball->radius() * 1.05) {
        colors_[r1][c1] = (colors_[r1][c1] + paint_color_) / 2.0;
        colors_[r2][c2] = (colors_[r1][c1] + paint_color_) / 2.0;
        colors_[r3][c3] = (colors_[r1][c1] + paint_color_) / 2.0;        
        collision_points->push_back(cp);
    }
};



void RollingWorld::getCollisionsForSquare(int r, int c,
                                          RollingBall* ball,
                                          vector<Vector3f>* collision_points) {
    float ball_y = ball->center_.y();
    if (!((height(r,c) - ball_y < 0) ||
          (height(r + 1, c + 1) - ball_y < 0) ||
          (height(r + 1, c) - ball_y < 0) ||
          (height(r, c + 1) - ball_y < 0))) {
        // square can't possibly intersect..
        return;
    }
            

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

const int RollingWorld::COLLISION_CELL_PADDING = 0;

void RollingWorld::getCollisions(RollingBall* ball, vector<Vector3f>* collision_points) {
    float radius = ball->radius();
    float center_x = ball->center_.x();
    float center_z = ball->center_.z();

    // Get collisions with surface
    int min_r = max(x_to_r(center_x - radius) - COLLISION_CELL_PADDING, 0);
    int max_r = min(x_to_r(center_x + radius) + COLLISION_CELL_PADDING, num_rows - 1);

    int min_c = max(z_to_c(center_z - radius) - COLLISION_CELL_PADDING, 0);
    int max_c = min(z_to_c(center_z + radius) + COLLISION_CELL_PADDING, num_cols - 1);    

    
    for (int r = min_r; r < max_r; r++) {
        for (int c = min_c; c < max_c; c++) {
            getCollisionsForSquare(r, c, ball, collision_points);
        }
    }

    // Get collisions with obstacles
    int num_obstacles = obstacles_.size();
    for (int i = 0; i < num_obstacles; i++) {
        obstacles_[i]->getCollisionsForObstacle(ball, collision_points);
    }
}

void RollingWorld::draw() {
    // Save State
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    glBegin(GL_TRIANGLES);

    // Draw surface
    for (int r = 0; r < num_rows - 1; r++) {
        for (int c = 0; c < num_cols - 1; c++) {
            drawSquare(r, c);
        }
    }

    // Draw obstacles
    int num_obstacles = obstacles_.size();
    for (int i = 0; i < num_obstacles; i++) {
        obstacles_[i]->draw();
    }

    // Draw pillars
    for (int r = 0; r < num_rows; r += num_rows/5) {
        for (int c = 0; c < num_cols; c +=num_cols/5) {
            int x = r_to_x(r);
            int z = c_to_z(c);
            glPushMatrix();
            glTranslatef(x, 0, z);
            glScaled(1, 100, 1);
            glutSolidCube(1);
            glPopMatrix();
        }
    }
    
    

    glEnd();

    // Revert state, etc.
    glPopMatrix();
    glPopAttrib();
};
