#ifndef VECTOR3_H_
#define VECTOR3_H_

#pragma once
 
#include <math.h>
 
class Vector3
{
	//http://dasl.mem.drexel.edu/~ducNguyen/projects-2/programming/3d-vector-calculation-in-c/
public:
    Vector3(void);
    Vector3(float X, float Y, float Z);
    ~Vector3(void);
    float Length();
    Vector3 Normalize();
    Vector3 operator-(Vector3& right);
    Vector3 operator*(Vector3& right);
    Vector3 operator*(float right);
    Vector3 operator^(float right);
    Vector3 operator/(float right);
    Vector3 operator^(int right);
    Vector3 operator+(Vector3& right);

    
    
    void set_x(float x);
    void set_y(float y);
    void set_z(float z);
    float x();
    float y();
    float z();
    float DistanceTo(Vector3* vector);
    float X,Y,Z;
    
    float grad_dist(Vector3& pos1, Vector3& pos2,float  d_min, float d_max, float gain);
};


#endif /*VECTOR3_H_*/
