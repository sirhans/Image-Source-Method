//
//  Room.h
//  ImageSourceMethod
//
//  Created by Hans on 16/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#ifndef Room_h
#define Room_h

#define ISM_ROOM_NUM_WALLS 6

#include <stdio.h>
#include "ISMWall.h"

typedef struct ISMRoom {
    ISMWall walls [ISM_ROOM_NUM_WALLS];
} ISMRoom;

void ISMRoom_initCuboid(ISMRoom* This, float l, float h, float w);

#endif /* Room_h */
