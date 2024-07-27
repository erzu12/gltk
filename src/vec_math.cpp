#include "vec_math.h"


void Vec2::asArray(float *dest) {
    dest[0] = this->x;
    dest[1] = this->y;
}

Vec2::Vec2(float x, float y) {
    Vec2 a = {x, y};
}

Vec2::Vec2() {
    Vec2 a = {0.0f, 0.0f};
}

Vec2 Vec2::operator+(const Vec2 b) const {
    Vec2 c;
    c.x = this->x + b.x;
    c.y = this->y + b.y;
    return c;
}

Vec2 Vec2::operator+=(const Vec2 b) {
    this->x += b.x;
    this->y += b.y;
    return *this;
}

Vec2 Vec2::operator-(const Vec2 b) const {
    Vec2 c;
    c.x = this->x - b.x;
    c.y = this->y - b.y;
    return c;
}

Vec2 Vec2::operator-=(const Vec2 b) {
    this->x -= b.x;
    this->y -= b.y;
    return *this;
}

float Vec2::Magnitude() const {
    return sqrt(this->x*this->x + this->y*this->y);
}

Vec2 Vec2::Noramlize() {
    float mag = Magnitude();
    if(mag == 0) return *this;
    this->x /= mag;
    this->y /= mag;
    return *this;
}

Vec2 Vec2::operator* (const float b) const {
    Vec2 c;
    c.x = this->x * b;
    c.y = this->y * b;
    return c;
}

Vec2 Vec2::operator*= (const float b) {
    this->x *= b;
    this->y *= b;
    return *this;
}


float Vec2::Dot(Vec2 b) const {
    return this->x * b.x + this->y * b.y;
}

float Vec2::Angle(Vec2 b) const {
    return  atan2(this->x * b.y - this->y * b.x, this->x * b.x + this->y * b.y);
}

Vec2 Vec2::Rot(float rot) const {
    Vec2 b;
    b.x = this->x * cos(rot) - this->y * sin(rot);
    b.y = this->x * sin(rot) + this->y * cos(rot);
    return b;
}

Vec2 Vec2::Lerp(Vec2 b, float t) const {
    Vec2 c;
    c.x = this->x + t * (b.x - this->x);
    c.y = this->y + t * (b.y - this->y);
    return c;
}


Mat3::Mat3() {
    mat = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};
}

Mat3 Mat3::operator*(const Mat3 &b) const {
    Mat3 c;
    c.mat[0] = this->mat[0] * b.mat[0] + this->mat[1] * b.mat[3] + this->mat[2] * b.mat[6];
    c.mat[1] = this->mat[0] * b.mat[1] + this->mat[1] * b.mat[4] + this->mat[2] * b.mat[7];
    c.mat[2] = this->mat[0] * b.mat[2] + this->mat[1] * b.mat[5] + this->mat[2] * b.mat[8];
    c.mat[3] = this->mat[3] * b.mat[0] + this->mat[4] * b.mat[3] + this->mat[5] * b.mat[6]; 
    c.mat[4] = this->mat[3] * b.mat[1] + this->mat[4] * b.mat[4] + this->mat[5] * b.mat[7]; 
    c.mat[5] = this->mat[3] * b.mat[2] + this->mat[4] * b.mat[5] + this->mat[5] * b.mat[8]; 
    c.mat[6] = this->mat[6] * b.mat[0] + this->mat[7] * b.mat[3] + this->mat[8] * b.mat[6]; 
    c.mat[7] = this->mat[6] * b.mat[1] + this->mat[7] * b.mat[4] + this->mat[8] * b.mat[7]; 
    c.mat[8] = this->mat[6] * b.mat[2] + this->mat[7] * b.mat[5] + this->mat[8] * b.mat[8]; 
    return c;
}

Mat3 Mat3::rotationMat(float angle) {
    Mat3 rotMat;
    rotMat.mat[0] = cos(angle);
    rotMat.mat[1] = -sin(angle);
    rotMat.mat[2] = 0.0f;
    rotMat.mat[3] = sin(angle);
    rotMat.mat[4] = cos(angle);
    rotMat.mat[5] = 0.0f;
    rotMat.mat[6] = 0.0f;
    rotMat.mat[7] = 0.0f;
    rotMat.mat[8] = 1.0f;
    return rotMat;
}

Mat3 Mat3::scalingMatrix(Vec2 scale) {
    Mat3 scaleMat;
    scaleMat.mat[0] = scale.x; 
    scaleMat.mat[1] = 0.0f; 
    scaleMat.mat[2] = 0.0f; 
    scaleMat.mat[3] = 0.0f;
    scaleMat.mat[4] = scale.y; 
    scaleMat.mat[5] = 0.0f;
    scaleMat.mat[6] = 0.0f;
    scaleMat.mat[7] = 0.0f;
    scaleMat.mat[8] = 1.0f;
    return scaleMat;
}

Mat3 Mat3::translationMatrix(Vec2 translate) {
    Mat3 translateionMat;
    translateionMat.mat[0] = 1.0f; 
    translateionMat.mat[1] = 0.0f; 
    translateionMat.mat[2] = translate.x; 
    translateionMat.mat[3] = 0.0f; 
    translateionMat.mat[4] = 1.0f;
    translateionMat.mat[5] = translate.y;
    translateionMat.mat[6] = 0.0f;
    translateionMat.mat[7] = 0.0f;
    translateionMat.mat[8] = 1.0f;
    return translateionMat;
}

void Mat3::rotateMat(float angle) {
    Mat3 rotMat = rotationMat(angle);
    *this = *this * rotMat;
}

void Mat3::scaleMat(Vec2 scale) {
    Mat3 scaleMat = scalingMatrix(scale);
    *this = *this * scaleMat;
}

void Mat3::translateMat(Vec2 translate) {
    Mat3 translateionMat = translationMatrix(translate);
    *this = *this * translateionMat;
}

