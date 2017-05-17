//
//  ISMVector3D.c
//  ImageSourceMethod
//
//  Created by Hans on 16/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include <math.h>
#include <stdbool.h>
#include "ISMVector3D.h"


ISMVector3D v3_crossProduct(ISMVector3D u, ISMVector3D v){
    ISMVector3D result = {u.y*v.z - u.z*v.y,
        u.z*v.x - u.x*v.z,
        u.x*v.y - u.y*v.x};
    return result;
}



float v3_dotProduct(ISMVector3D u, ISMVector3D v){
    return u.x*v.x + u.y*v.y + u.z*v.z;
}


/*
 * the l2 norm (length) of v
 */
float v3_l2Norm(ISMVector3D v){
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}



ISMVector3D v3_normalize(ISMVector3D v){
    float norm = v3_l2Norm(v);
    v.x /= norm;
    v.y /= norm;
    v.z /= norm;
    
    return v;
}



/*
 * returns u - v
 */
ISMVector3D v3_sub(ISMVector3D u, ISMVector3D v){
    u.x -= v.x;
    u.y -= v.y;
    u.z -= v.z;
    return u;
}



/*
 * returns s*v
 */
ISMVector3D v3_scalarMul(ISMVector3D v, float s){
    v.x *= s;
    v.y *= s;
    v.z *= s;
    
    return v;
}



/*
 * returns the projection of u onto v
 */
ISMVector3D v3_project(ISMVector3D u, ISMVector3D v){
    v = v3_normalize(v);
    return v3_scalarMul(v, v3_dotProduct(u, v));
}


bool v3_isEqual(ISMVector3D u, ISMVector3D v){
    return u.x == v.x && u.y == v.y && u.z == v.z;
}


bool v3_isPerpendicular(ISMVector3D u, ISMVector3D v){
    return v3_dotProduct(u,v) == 0;
}




bool v3_isParallel(ISMVector3D u, ISMVector3D v){
    return v3_l2Norm(v3_crossProduct(u,v)) == 0;
}
