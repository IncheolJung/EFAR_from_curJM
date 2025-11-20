#pragma once
#include "typeDef.hpp"


struct Vec3 {
    myFloat x,y,z;
    Vec3():x(0),y(0),z(0){}
    Vec3(myFloat X,myFloat Y,myFloat Z):x(X),y(Y),z(Z){}
    Vec3 operator+(const Vec3& o)const{return Vec3(x+o.x,y+o.y,z+o.z);}
    Vec3 operator-(const Vec3& o)const{return Vec3(x-o.x,y-o.y,z-o.z);}
    Vec3 operator*(myFloat s)const{return Vec3(x*s,y*s,z*s);}
};

static Vec3 cross(const Vec3 &a,const Vec3 &b){
    return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
static myFloat dot(const Vec3 &a,const Vec3 &b){ return a.x*b.x + a.y*b.y + a.z*b.z; }
static myFloat norm(const Vec3 &a){ return sqrt(dot(a,a)); }

// Complex vector
struct CVec3 {
    myComplex x,y,z;
    CVec3():x(0),y(0),z(0){}
    CVec3(myComplex X,myComplex Y,myComplex Z):x(X),y(Y),z(Z){}
    CVec3 operator+(const CVec3& o)const{return CVec3(x+o.x,y+o.y,z+o.z);}
    CVec3 operator*(const myComplex s)const{return CVec3(x*s,y*s,z*s);}
    void operator+=(const CVec3& o){x+=o.x; y+=o.y; z+=o.z;}
};
static CVec3 cross(const CVec3 &a,const CVec3 &b){
    return CVec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
static myComplex dot(const CVec3 &a,const Vec3 &b){
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

using currentData = std::vector<CVec3>;

// Assume cur1 is deterministic, cur2 is stochastic (only defined at aperture)
static void mergeCurrents (
    currentData &cur1, currentData &cur2
) {
    int N1 = cur1.size();
    int N2 = cur2.size();

    if (N1 < N2) {
        throw std::runtime_error(
            "cur1 must be larger than cur2. " + std::to_string(N1) + " < " + std::to_string(N2)
        );
    }
    for (int i=0; i<N2; ++i){
        cur1[i] += cur2[i];
    }
}