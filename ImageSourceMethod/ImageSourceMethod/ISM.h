//
//  ISM.h
//  ImageSourceMethod
//
//  Created by Hans on 15/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#ifndef ISM_h
#define ISM_h

#include <stdio.h>
#include "ISMRoom.h"


typedef struct ISMImageSource {
    ISMVector3D location;
    bool valid;
} ISMImageSource;

void ISM_simulateRoom(float length, float height, float width,
                      float wallAbsorbtionCoefficient,
                      float speedOfSound,
                      size_t reflectionMaxOrder,
                      ISMVector3D sourcePosition,
                      ISMVector3D listenerLeftEar,
                      ISMVector3D listenerRightEar,
                      ISMVector3D* imageSources,
                      float* reflectionTimes,
                      float* reflectionGains,
                      size_t* reflectionOrders);

void ISM_reflectSources(const ISMWall* walls,
                        size_t numWalls,
                        const ISMImageSource* inputSources,
                        size_t numInputSources,
                        ISMImageSource* reflectedSources,
                        size_t numReflectedSources);

void ISM_printImageSource (ISMImageSource s);


size_t ISM_numSourcesAtOrder(size_t N);

size_t ISM_totalSourcesForOrder(size_t N);


/*
 * return the number of elements in listA that are also in listB
 */
size_t ISM_countDuplicates(ISMImageSource* listA, size_t aLength,
                       ISMImageSource* listB, size_t bLength);

#endif /* ISM_h */
