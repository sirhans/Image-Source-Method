//
//  ISM.c
//  ImageSourceMethod
//
//  Created by Hans on 15/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include "ISM.h"
#include <assert.h>
#include <stdlib.h>


/*
 * find the distance from s to the left or right ear, whichever is shortest
 */
float ISM_distanceFromL(ISMVector3D listenerLeftEar,
                        ISMVector3D listenerRightEar,
                        ISMImageSource s){
    // find distance to left ear
    float distanceL = v3_l2Norm(v3_sub(s.location, listenerLeftEar));
    
    // find distance to right ear
    float distanceR = v3_l2Norm(v3_sub(s.location, listenerRightEar));
    
    // return the minimum of the two
    return distanceL < distanceR ? distanceL : distanceR;
}


size_t ISM_numSourcesAtOrder(size_t N){
    if (N==0) return 1;
    return ISM_totalSourcesForOrder(N) - ISM_totalSourcesForOrder(N-1);
}


size_t ISM_totalSourcesForOrder(size_t N){
    return (4*N*N*N + 6*N*N + 8*N + 3)/3;
}


void ISM_simulateRoom(float length, float height, float width,
                      float wallAbsorbtionCoefficient,
                      float speedOfSound,
                      float micDistance,
                      size_t reflectionMaxOrder,
                      ISMVector3D sourcePosition,
                      ISMVector3D listenerLeftEar,
                      ISMVector3D listenerRightEar,
                      ISMVector3D* imageSources,
                      float* reflectionTimes,
                      float* reflectionGains,
                      size_t* reflectionOrders){
    
    // allocate a 2d array for storing all the image sources at each order
    ISMImageSource** imageSourcesByLevel;
    imageSourcesByLevel = malloc((reflectionMaxOrder+1)*sizeof(ISMImageSource*));
    for(size_t i=0; i<=reflectionMaxOrder; i++)
        imageSourcesByLevel[i] = malloc(ISM_numSourcesAtOrder(i)*sizeof(ISMImageSource));
    
    
    // create a simulated room
    // initialize a room
    ISMRoom room;
    ISMRoom_initCuboid(&room, length, height, width);

    
    // insert the source as the first image source
    imageSourcesByLevel[0][0] = (ISMImageSource){sourcePosition,true};
    
    // find the image source locations for each order
    for(size_t order=0; order<reflectionMaxOrder; order++){
        // reflect from the sources at order to the sources at order+1
        ISM_reflectSources(room.walls, 6,
                           imageSourcesByLevel[order], ISM_numSourcesAtOrder(order),
                           imageSourcesByLevel[order+1], ISM_numSourcesAtOrder(order+1));
    }
    
    
    // allocate memory for image source distances
    float* distances = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    
    
    /*
     * find distance, propagation time, air energy loss, reflection loss
     */
    size_t k=0;
    for(size_t i=0; i<=reflectionMaxOrder; i++){
        for(size_t j=0; j<ISM_numSourcesAtOrder(i); j++){
            // find the distance from listener for each source
            distances[k] = ISM_distanceFromL(listenerLeftEar,
                                               listenerRightEar,
                                               imageSourcesByLevel[i][j]);
            // convert distance to time
            reflectionTimes[k] = distances[k]/speedOfSound;
            
            // find the loss due to reflections
            reflectionGains[k] = powf(-wallAbsorbtionCoefficient,(float)i);
            
            // model the 1/d pressure loss
            reflectionGains[k] *= micDistance/distances[k];
            
            // copy the position of the sound source to the output list
            imageSources[k] = imageSourcesByLevel[i][j].location;
            
            k++;
        }
    }
    
    free(distances);
}



/*
 * return the reflection of s in w
 */
ISMImageSource ISM_reflect(ISMWall w, ISMImageSource s){
    ISMImageSource reflection;
    
    // vector from w to s, perpendicular to w
    ISMVector3D ws = separationVector(w, s.location);
    
    // is the source on the front side of the wall?
    bool onFrontSide = v3_dotProduct(ws, getSurfaceNormal(w)) > 0.0f;
    
    // if it's on the front, reflect and mark the image source active
    if(onFrontSide){
        // subtract two separation vectors from s and you reach the image source.
        ISMVector3D ws2 = v3_scalarMul(ws, 2.0f);
        reflection.location = v3_sub(s.location, ws2);
        reflection.valid = true;
    }
    
    // if s was on the back side of the wall, mark the reflection invalid
    else reflection.valid = false;
    
    return reflection;
}




bool ISM_isEqual(ISMImageSource u, ISMImageSource v){
    return u.valid == v.valid && v3_isEqual(u.location,v.location);
}





/*
 * Insert to list without duplicates
 */
void ISM_insertSource(ISMImageSource* list, size_t length, ISMImageSource s){
    // do not insert an invalid source
    if (!s.valid) return;
    
    size_t i=0;
    // iterate through the list
    while(i<length){
        // insert at the first empty (marked invalid) position and stop
        if (!list[i].valid){
            list[i] = s;
            return;
        }
        
        // stop without insert on duplicate entry
        if (ISM_isEqual(list[i],s)) return;
        
        // advance the counter
        i++;
    }
}



void ISM_printImageSource (ISMImageSource s) {
    if(s.valid)
        printf("v:{%f, %f, %f}",s.location.x,s.location.y,s.location.z);
    else
        printf("i:{%f, %f, %f}",s.location.x,s.location.y,s.location.z);
}


void ISM_reflectSources(const ISMWall* walls,
                        size_t numWalls,
                        const ISMImageSource* inputSources,
                        size_t numInputSources,
                        ISMImageSource* reflectedSources,
                        size_t numReflectedSources){
    // init the list of reflected sources to empty
    for(size_t i=0; i<numReflectedSources; i++)
        reflectedSources[i].valid = false;
    
    for(size_t i=0; i<numInputSources; i++){
        for(size_t j=0; j<numWalls; j++){
            // get the reflection of each source in each wall
            ISMImageSource s = ISM_reflect(walls[j], inputSources[i]);
            // add to list of reflected sources, ensuring no duplicates
            ISM_insertSource(reflectedSources,numReflectedSources,s);
        }
    }
}




/*
 * return the number of elements in listA that are also in listB
 */
size_t ISM_countDuplicates(ISMImageSource* listA, size_t aLength,
                       ISMImageSource* listB, size_t bLength){
    size_t count = 0;
    for(size_t i=0; i<aLength; i++)
        for(size_t j=0; j<bLength; j++)
            if(ISM_isEqual(listA[i],listB[j]))
               count++;
               
    return count;
}
