//
//  ISMRoom.c
//  ImageSourceMethod
//
//  Created by Hans on 16/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include "ISMRoom.h"
#include "stdlib.h"


void ISMRoom_initCuboid(ISMRoom* This, float l, float h, float w){
    
    // three walls anchored at origin
    This->walls[0] = (ISMWall){(ISMVector3D){0,0,0},
                               (ISMVector3D){0,h,0},
                               (ISMVector3D){l,0,0}};
    This->walls[1] = (ISMWall){(ISMVector3D){0,0,0},
                               (ISMVector3D){0,0,w},
                               (ISMVector3D){0,h,0}};
    This->walls[2] = (ISMWall){(ISMVector3D){0,0,0},
                               (ISMVector3D){l,0,0},
                               (ISMVector3D){0,0,w}};
    
    // three walls anchored at opposite corner
    This->walls[3] = (ISMWall){(ISMVector3D){l,h,w},
                               (ISMVector3D){-l,0,0},
                               (ISMVector3D){0,-h,0}};
    This->walls[4] = (ISMWall){(ISMVector3D){l,h,w},
                               (ISMVector3D){0,-h,0},
                               (ISMVector3D){0,0,-w}};
    This->walls[5] = (ISMWall){(ISMVector3D){l,h,w},
                               (ISMVector3D){0,0,-w},
                               (ISMVector3D){-l,0,0}};
}
