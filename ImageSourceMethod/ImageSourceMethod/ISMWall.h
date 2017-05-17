//
//  ISMWall.h
//  ImageSourceMethod
//
//  Created by Hans on 15/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#ifndef ISMWall_h
#define ISMWall_h

#include "ISMVector3D.h"

/*
 * corner is any point in space
 * edge1 and edge2 are two edges with tails at corner
 */
typedef struct ISMWall {
    ISMVector3D corner, edge1, edge2;
} ISMWall;



/*
 * returns true if w is orthogonal to v
 */
bool isOrthogonal(ISMWall w, ISMVector3D v);



ISMVector3D getSurfaceNormal(ISMWall w);



/*
 * returns the shortest vector from w to v
 */
ISMVector3D separationVector(ISMWall w, ISMVector3D v);



#endif /* ISMWall_h */
