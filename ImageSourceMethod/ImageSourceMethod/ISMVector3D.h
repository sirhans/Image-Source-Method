//
//  ISMVector3D.h
//  ImageSourceMethod
//
//  Created by Hans on 15/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#ifndef ISMVector3D_h
#define ISMVector3D_h

#include <math.h>
#include <stdbool.h>

typedef struct ISMVector3D {
    float x,y,z;
} ISMVector3D;



ISMVector3D v3_crossProduct(ISMVector3D u, ISMVector3D v);


float v3_dotProduct(ISMVector3D u, ISMVector3D v);


/*
 * the l2 norm (length) of v
 */
float v3_l2Norm(ISMVector3D v);


ISMVector3D v3_normalize(ISMVector3D v);


/*
 * returns u - v
 */
ISMVector3D v3_sub(ISMVector3D u, ISMVector3D v);


/*
 * returns s*v
 */
ISMVector3D v3_scalarMul(ISMVector3D v, float s);


/*
 * returns the projection of u onto v
 */
ISMVector3D v3_project(ISMVector3D u, ISMVector3D v);


bool v3_isEqual(ISMVector3D u, ISMVector3D v);


bool v3_isPerpendicular(ISMVector3D u, ISMVector3D v);


bool v3_isParallel(ISMVector3D u, ISMVector3D v);


#endif /* Point3D_h */
