#include "vec_math.h"


void Vec2::asArray(float *dest) {
    dest[0] = this->x;
    dest[1] = this->y;
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

Vec2 Vec2::operator* (const Vec2 b) const {
    Vec2 c;
    c.x = this->x * b.x;
    c.y = this->y * b.y;
    return c;
}

Vec2 Vec2::operator/ (const float b) const {
    Vec2 c;
    c.x = this->x / b;
    c.y = this->y / b;
    return c;
}

float Vec2::Dot(const Vec2 b) const {
    return this->x * b.x + this->y * b.y;
}

float Vec2::Angle(const Vec2 b) const {
    return  atan2(this->x * b.y - this->y * b.x, this->x * b.x + this->y * b.y);
}

Vec2 Vec2::Rot(float rot) const {
    Vec2 b;
    b.x = this->x * cos(rot) - this->y * sin(rot);
    b.y = this->x * sin(rot) + this->y * cos(rot);
    return b;
}

Vec2 Vec2::max(const Vec2 b) {
    this->x = this->x > b.x ? this->x : b.x;
    this->y = this->y > b.y ? this->y : b.y;
    return *this;
}

Vec2 Vec2::min(const Vec2 b) {
    this->x = this->x < b.x ? this->x : b.x;
    this->y = this->y < b.y ? this->y : b.y;
    return *this;
}

Vec2 Vec2::Lerp(Vec2 b, float t) const {
    Vec2 c;
    c.x = this->x + t * (b.x - this->x);
    c.y = this->y + t * (b.y - this->y);
    return c;
}

bool Vec2::operator==(const Vec2 &b) const {
    return this->x == b.x && this->y == b.y;
}

std::ostream &operator<<(std::ostream &os, const Vec2 &f2) {
    os << "{" << f2.x << ", " << f2.y << "}";
    return os;
}

Vec3 Vec3::operator+(const Vec3 &b) const
{
    Vec3 c;
    c.x = this->x + b.x;
    c.y = this->y + b.y;
    c.z = this->z + b.z;
    return c;
}
Vec3 Vec3::operator+=(const Vec3 &b)
{
    this->x += b.x;
    this->y += b.y;
    this->z += b.z;
    return *this;
}

Vec3 Vec3::operator-(const Vec3 &b) const
{
    Vec3 c;
    c.x = this->x - b.x;
    c.y = this->y - b.y;
    c.z = this->z - b.z;
    return c;
}
Vec3 Vec3::operator-=(const Vec3 &b)
{
    this->x -= b.x;
    this->y -= b.y;
    this->z -= b.z;
    return *this;
}

Vec3 Vec3::operator-() const
{
    Vec3 c;
    c.x = -this->x;
    c.y = -this->y;
    c.z = -this->z;
    return c;
}

float Vec3::magnitude() const {
    return sqrt(this->x * this->x + this->y * this->y + this->z * this->z); 
}

Vec3 Vec3::normalize()
{
    float mag = magnitude();
    if (mag == 0)
        return *this;
    this->x /= mag;
    this->y /= mag;
    this->z /= mag;
    return *this;
}

Vec3 Vec3::operator*(float b) const
{
    Vec3 c;
    c.x = this->x * b;
    c.y = this->y * b;
    c.z = this->z * b;
    return c;
}

Vec3 Vec3::operator*=(float b)
{
    this->x *= b;
    this->y *= b;
    this->z *= b;
    return *this;
}

Vec3 Vec3::operator*(const Vec3 &b) const
{
    Vec3 c;
    c.x = this->x * b.x;
    c.y = this->y * b.y;
    c.z = this->z * b.z;
    return c;
}

Vec3 Vec3::operator/(float b) const
{
    Vec3 c;
    c.x = this->x / b;
    c.y = this->y / b;
    c.z = this->z / b;
    return c;
}

float Vec3::dot(const Vec3 &b) const { return this->x * b.x + this->y * b.y + this->z * b.z; }

Vec3 Vec3::cross(const Vec3 &b) const
{
    Vec3 c;
    c.x = this->y * b.z - this->z * b.y;
    c.y = this->z * b.x - this->x * b.z;
    c.z = this->x * b.y - this->y * b.x;
    return c;
}

Vec3 Vec3::reflect(const Vec3 &normal) const
{
    return *this - normal * 2 * this->dot(normal);
}

Vec3 Vec3::max(const Vec3 &b)
{
    this->x = this->x > b.x ? this->x : b.x;
    this->y = this->y > b.y ? this->y : b.y;
    this->z = this->z > b.z ? this->z : b.z;
    return *this;
}

Vec3 Vec3::min(const Vec3 &b)
{
    this->x = this->x < b.x ? this->x : b.x;
    this->y = this->y < b.y ? this->y : b.y;
    this->z = this->z < b.z ? this->z : b.z;
    return *this;
}

Vec3 Vec3::lerp(const Vec3 &b, float t) const
{
    return *this * (1 - t) + b * t;
}

bool Vec3::operator==(const Vec3 &b) const
{
    return this->x == b.x && this->y == b.y && this->z == b.z;
}

std::ostream &operator<<(std::ostream &os, const Vec3 &f3)
{
    os << "{" << f3.x << ", " << f3.y << ", " << f3.z << "}";
    return os;
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

Mat3 Mat3::rotationMatrix(float angle) {
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

Mat3 Mat3::viewMatrix(Vec2 viewSize) {
    Mat3 viewMat;
    viewMat.mat[0] = 2.0f / viewSize.x;
    viewMat.mat[1] = 0.0f;
    viewMat.mat[2] = -1.0f;
    viewMat.mat[3] = 0.0f;
    viewMat.mat[4] = -2.0f / viewSize.y;
    viewMat.mat[5] = 1.0f;
    viewMat.mat[6] = 0.0f;
    viewMat.mat[7] = 0.0f;
    viewMat.mat[8] = 1.0f;
    return viewMat;
}

void Mat3::rotateMatrix(float angle) {
    Mat3 rotMat = rotationMatrix(angle);
    *this = *this * rotMat;
}

void Mat3::scaleMatrix(Vec2 scale) {
    Mat3 scaleMat = scalingMatrix(scale);
    *this = *this * scaleMat;
}

void Mat3::translateMatrix(Vec2 translate) {
    Mat3 translateionMat = translationMatrix(translate);
    *this = *this * translateionMat;
}

float *Mat3::data() {
    return mat.data();
}

std::array<float, 3> Mat3::operator[](int index) const {
    return {mat[index * 3], mat[index * 3 + 1], mat[index * 3 + 2]};
}

std::ostream &operator<<(std::ostream &os, const Mat3 &m3) {
    os << "{" << m3.mat[0] << ", " << m3.mat[1] << ", " << m3.mat[2] << "}" << std::endl;
    os << "{" << m3.mat[3] << ", " << m3.mat[4] << ", " << m3.mat[5] << "}" << std::endl;
    os << "{" << m3.mat[6] << ", " << m3.mat[7] << ", " << m3.mat[8] << "}" << std::endl;
    return os;
}

void BoundingBox::add(const BoundingBox &other) {
    max = Vec2(std::max(max.x, other.max.x), std::max(max.y, other.max.y));
    min = Vec2(std::min(min.x, other.min.x), std::min(min.y, other.min.y));
}

bool BoundingBox::contains(Vec2 point) {
    return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
}

BoundingBox BoundingBox::intersect(const BoundingBox &other) {
    BoundingBox bb;
    bb.max = Vec2(std::min(max.x, other.max.x), std::min(max.y, other.max.y));
    bb.min = Vec2(std::max(min.x, other.min.x), std::max(min.y, other.min.y));
    return bb;
}

std::ostream &operator<<(std::ostream &os, const BoundingBox &bb) {
    os << "{" << bb.min << ", " << bb.max << "}";
    return os;
}
