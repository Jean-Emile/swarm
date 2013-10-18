#include "Vector3.h"
// Vector3.cpp
 
#include <iostream>
 
Vector3::Vector3(void)
{
}
 
Vector3::Vector3(float X, float Y, float Z){
    this->X = X;
    this->Y = Y;
    this->Z = Z;
}
 
// Returns the length of the vector
float Vector3::Length(){
    return sqrt(X * X + Y * Y + Z * Z);
}
 




// Normalizes the vector
Vector3 Vector3::Normalize(){
    Vector3 vector;
    float length = this->Length();

    if(length != 0){
        vector.X = X/length;
        vector.Y = Y/length;
        vector.Z = Z/length;
    }
 
    return vector;
}

Vector3 Vector3::toUnitVector(){
    Vector3 vector;
	double lat = X/ 180 * M_PI;
	double lng = Y/ 180 * M_PI;

    vector.X = cos(lng)*cos(lat);
        vector.Y = sin(lng)*cos(lat);
        vector.Z = sin(lat);
	return vector;
}
Vector3     Vector3::orthogonal(){
    Vector3 vector;

  double minNormal = abs(X);
            int minIndex = 0;
            if (abs(Y) < minNormal)
            {
                minNormal = abs(Y);
                minIndex = 1;
            }
            if (abs(Z) < minNormal)
            {
                minNormal = abs(Z);
                minIndex = 2;
            }

            Vector3 B;
            switch (minIndex)
            {
                case 0:
B.X = 1;
B.Y =0;
B.Z = 0;
                    break;
                case 1:
            	        B.X = 0;
			B.Y =1;
			B.Z = 0;
                    break;
                default:
                 B.X = 0;
		B.Y =0;
		B.Z = 1;
                    break;
            }

     Vector3  result;

 result.X = (minNormal *X) -B.X;
 result.Y = (minNormal *Y) -B.Y;
 result.Z = (minNormal *Z) -B.Z;

            return result.Normalize();
}


void Vector3::set_x(float x){
	 this->X = x;
}
void Vector3::set_y(float y){
	 this->Y = y;
}
void Vector3::set_z(float z){
	 this->Z =z;
}
 
float Vector3::x(){
	return  this->X;
}
float Vector3::y(){
	return  this->Y;
}

float Vector3::z(){
	return  this->Z;
}



Vector3 Vector3::operator*( Vector3& right)
{
	Vector3 result;
    result.set_x(x() - right.x());
    result.set_y(y() - right.y());
    result.set_z(z() - right.z());
    return result;
}


Vector3 Vector3::operator-( Vector3& right)
{
	Vector3 result;
    result.set_x(x() - right.x());
    result.set_y(y() - right.y());
    result.set_z(z() - right.z());
    return result;
}


Vector3 Vector3::operator+( Vector3 right)
{
	Vector3 result;
    result.set_x(x() + right.x());
    result.set_y(y() + right.y());
    result.set_z(z() + right.z());
    return result;
}


Vector3 Vector3::operator^(float right){
	Vector3 result;
    result.set_x(pow(x(),right));
    result.set_y(pow(y() ,right));
    result.set_z(pow(z(),right));
    return result;	
}

Vector3 Vector3::operator^(int right){
	Vector3 result;
    result.set_x(pow(x(),right));
    result.set_y(pow(y() ,right));
    result.set_z(pow(z(),right));
    return result;	
}

Vector3 Vector3::operator*(float right){
	Vector3 result;
    result.set_x(x() * right);
    result.set_y(y() * right);
    result.set_z(z() * right);
    return result;	
}
Vector3 Vector3::operator/(float right){
	Vector3 result;
    result.set_x(x() / right);
    result.set_y(y() / right);
    result.set_z(z() / right);
    return result;	
}

float Vector3::DistanceTo(Vector3* v){
    float distance = sqrt(pow(v->X-X,2)+pow(v->Y-Y,2)+pow(v->Z-Z,2));
    return distance;
}
 
Vector3::~Vector3(void)
{
}


Vector3 Vector3::cross(Vector3 &that){
  return Vector3(Y * that.Z - Z * that.Y, Z * that.X - X * that.Z, X * that.Y - Y * that.X);
}
