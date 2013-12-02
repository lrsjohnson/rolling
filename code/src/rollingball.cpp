#include "rollingball.h"
#include "extra.h"

#include <cassert>

const int RollingBall::RENDER_SLICES = 20;
const int RollingBall::RENDER_STACKS = 15;

const double PI = 3.1415926535897932384626433832795;

RollingBall::RollingBall(Vector3f center, float r) {
    assert(center_);
    assert(r > 0);
    center_ = center;
    radius_ = r;

    rotation_ = Matrix4f::identity();
    velocity_ = Vector3f(1, 0, 1);

    axis_rotation_ = 0;

    textureFileName = "worldmap.bmp";
    textureActive = false;
};

RollingBall::~RollingBall() {
};

void RollingBall::draw() {
    // Save State
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glPushMatrix();

    // Adjust sphere rotation

    glTranslatef(center_[0], center_[1], center_[2]);


    glPushMatrix();
  
    glMultMatrixf(rotation_);
    // Draw Sphere
    //  glShadeMode(SMOOTH);
    //  glutWireSphere(radius_, RENDER_SLICES, RENDER_STACKS);
    drawSphere();


    glPopMatrix();

    // Attempt at shadow
    glTranslatef(0, -radius_, 0);
    glScaled(0.9, 0.02, 0.9);

    GLfloat diffColor[] = {0.0f, 0.0f, 0.0f, 1.f};
    GLfloat specColor[] = {0.1f, 0.1f, 0.1f, 1.f};
    GLfloat shininess[] = {5.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);    

    glutSolidSphere(radius_, RENDER_SLICES, RENDER_STACKS);

    // revert colors
    GLfloat diffColor_d[] = {0.4f, 0.4f, 0.4f, 1.f};
    GLfloat specColor_d[] = {0.6f, 0.6f, 0.6f, 1.f};
    GLfloat shininess_d[] = {50.0f};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColor_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor_d);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess_d);    
  
    // Revert state, etc.
    glPopMatrix();
    glPopAttrib();
};

void RollingBall::drawSphere() {
    //glutSolidSphere(radius_, RENDER_SLICES, RENDER_STACKS);
    Vector3f a = Vector3f(1, 0, 0);
    Vector3f b = Vector3f(0, 0, -1);
    Vector3f c = Vector3f(-1, 0, 0);
    Vector3f d = Vector3f(0, 0, 1);
    Vector3f e = Vector3f(0, 1, 0);
    Vector3f f = Vector3f(0, -1, 0);

    int recurse = 5;

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, getTextureName());

    drawSphereFace(recurse, radius_, d, a, e);
    drawSphereFace(recurse, radius_, a, b, e);
    drawSphereFace(recurse, radius_, b, c, e);
    drawSphereFace(recurse, radius_, c, d, e);
    drawSphereFace(recurse, radius_, a, d, f);
    drawSphereFace(recurse, radius_, b, a, f);
    drawSphereFace(recurse, radius_, c, b, f);
    drawSphereFace(recurse, radius_, d, c, f);

    glDisable(GL_TEXTURE_2D);
};

void RollingBall::drawSphereFace(int p_recurse, double p_radius, Vector3f a, Vector3f b, Vector3f c) {
    if (p_recurse > 1) {
        // Compute vectors halfway between the passed vectors
        Vector3f d = (a + b).normalized();
        Vector3f e = (b + c).normalized();
        Vector3f f = (a + c).normalized();

        drawSphereFace(p_recurse-1, p_radius, a, d, f);
        drawSphereFace(p_recurse-1, p_radius, d, b, e);
        drawSphereFace(p_recurse-1, p_radius, f, e, c);
        drawSphereFace(p_recurse-1, p_radius, f, d, e);
    }

    glBegin(GL_TRIANGLES);
    float tx1 = atan2(a[0], a[2]) / (2. * PI) + 0.5;
    float ty1 = asin(a[1]) / PI + .5;
    glTexCoord2f(tx1, ty1);

    glNormal(a);
    glVertex(a * p_radius);

    float tx2 = atan2(b[0], b[2]) / (2. * PI) + 0.5;
    float ty2 = asin(b[1]) / PI + .5;
    if (tx2 < 0.75 && tx1 > 0.75) {
        tx2 += 1.0;
    } else if(tx2 > 0.75 && tx1 < 0.75) {
        tx2 -= 1.0;
    }
    glTexCoord2f(tx2, ty2);
    glNormal(b);
    glVertex(b * p_radius);


    float tx3 = atan2(c[0], c[2]) / (2. * PI) + 0.5;
    float ty3 = asin(c[1]) / PI + .5;
    if (tx3 < 0.75 && tx1 > 0.75) {
        tx3 += 1.0;
    } else if(tx3 > 0.75 && tx1 < 0.75) {
        tx3 -= 1.0;
    }
    glTexCoord2f(tx3, ty3);
    glNormal(c);
    glVertex(c * p_radius);
    glEnd();
};

GLuint RollingBall::getTextureName() {
    if (textureActive) {
        return textureName;
    }
    texture = loadBMP_custom(textureFileName);
    textureActive = true;
    return textureName;
};

GLuint RollingBall::loadBMP_custom(const char * imagepath) {
    // Data read from the header of the BMP file
    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
    unsigned int dataPos;     // Position in the file where the actual data begins
    unsigned int imageSize;   // = width*height*3

    // Open the file
    FILE * file = fopen(imagepath,"rb");
    if (!file) {
        cout << "Image could not be opened" << endl;
        return 0;
    }

    if (fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
        cout << "Not a correct BMP file" << endl;
        return false;
    }

    if (header[0]!='B' || header[1]!='M') {
        cout << "Not a correct BMP file" << endl;
        return 0;
    }

    // Read ints from the byte array
    dataPos    = *(int*)&(header[0x0A]);
    imageSize  = *(int*)&(header[0x22]);
    m_width      = *(int*)&(header[0x12]);
    m_height     = *(int*)&(header[0x16]);

    // Some BMP files are misformatted, guess missing information
    if (imageSize == 0) {
        imageSize=m_width*m_height*3; // 3 : one byte for each Red, Green and Blue component
    }
    if (dataPos == 0) {
        dataPos=54; // The BMP header is done that way
    }

    // Create a buffer
    m_data = new unsigned char [imageSize];

    // Read the actual data from the file into the buffer
    fread(m_data,1,imageSize,file);

    //Everything is in memory now, the file can be closed
    fclose(file);

    cout << "Read file " << imagepath << endl;

    // Create one OpenGL texture
    glGenTextures(1, &textureName);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureName);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, m_data);
};
