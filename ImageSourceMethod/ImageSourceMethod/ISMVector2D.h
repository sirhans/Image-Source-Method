//
//  ISMVector2D.h
//  ImageSourceMethod
//
//  Created by Hans on 22/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#ifndef ISMVector2D_h
#define ISMVector2D_h

#include <stdio.h>
#include "ISMVector3D.h"

typedef struct ISMVector2D{
    float x, y;
}ISMVector2D;

float dotProduct(ISMVector2D u, ISMVector2D v);


ISMVector2D v2_normalize(ISMVector2D v);

float v2_norm(ISMVector2D v);

ISMVector2D flatten2D(ISMVector3D v);

/*
 * returns a vector at right angle to the input, rotated counter-clockwise
 */
ISMVector2D v2_counterClockwisePerpendicular(ISMVector2D v);



/*
 * returns the angle of rotation that would put u parallel to v
 */
float v2_angle(ISMVector2D u, ISMVector2D v);

#endif /* ISMVector2D_h */
