//
//  ISMVector2D.c
//  ImageSourceMethod
//
//  Created by Hans on 22/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include "ISMVector2D.h"



ISMVector2D flatten2D(ISMVector3D v){
    return (ISMVector2D){v.x,v.z};
}



ISMVector2D v2_counterClockwisePerpendicular(ISMVector2D v){
    return (ISMVector2D){v.y,-v.x};
}



/*
 * returns the angle of rotation that would put u parallel to v
 */
float v2_angle(ISMVector2D u, ISMVector2D v){
    // get the angles from u and v to x axis
    float vToXaxis = atan2(v.y, v.x);
    float uToXaxis = atan2(u.y, u.x);
    
    // get the angle between u and v
    float angle = uToXaxis - vToXaxis;
    
    // wrap to [0,360)
    while (angle < 0.0f) angle += 360.0f;
    while (angle >= 360.0f) angle -= 360.0f;
    
    return angle;
}



float dotProduct(ISMVector2D u, ISMVector2D v){
    return u.x*v.x + u.y*v.y;
}




ISMVector2D v2_normalize(ISMVector2D v){
    float norm = v2_norm(v);
    return (ISMVector2D){v.x/norm, v.y/norm};
}




float v2_norm(ISMVector2D v){
    return sqrt(v.x*v.x + v.y*v.y);
}
