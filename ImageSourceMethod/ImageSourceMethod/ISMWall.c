//
//  ISMWall.c
//  ImageSourceMethod
//
//  Created by Hans on 16/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include "ISMWall.h"


/*
 * returns true if w is orthogonal to v
 */
bool isOrthogonal(ISMWall w, ISMVector3D v){
    // true if both edges perpendicular to v
    return v3_isPerpendicular(w.edge1, v) && v3_isPerpendicular(w.edge2, v);
}




ISMVector3D getSurfaceNormal(ISMWall w){
    return v3_normalize(v3_crossProduct(w.edge2, w.edge1));
}




/*
 * returns the shortest vector from w to v
 */
ISMVector3D separationVector(ISMWall w, ISMVector3D v){
    // get the surface normal of w
    ISMVector3D n = getSurfaceNormal(w);
    
    // get a vector from the corner of w to x
    ISMVector3D wv = v3_sub(v, w.corner);
    
    // project wx onto the surface normal and return
    return v3_project(wv, n);
}

