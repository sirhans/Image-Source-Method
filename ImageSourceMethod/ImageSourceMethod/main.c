//
//  main.c
//  ImageSourceMethod
//
//  Created by Hans on 15/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "ISM.h"
#include "ISMRoom.h"

// forward declaration of test function
void ISMTests1();
void ISMTests2(size_t reflectionMaxOrder);


int main(int argc, const char * argv[]) {
    // ISMTests1();
    
    // set the model to go up to this order of reflections
    size_t reflectionMaxOrder = 20;
    
    // test it
    ISMTests2(reflectionMaxOrder);
    
    return 0;
}





void ISMTests1(){
    // initialize a room
    ISMRoom room;
    ISMRoom_initCuboid(&room, 3, 4, 5);
    
    // create a list of one single sound source at {1,1,1}
    ISMImageSource s [1] = {(ISMImageSource){(ISMVector3D){1,1,1},true}};
    
    // do first order reflections
    ISMImageSource imageSources1stOrder [6];
    size_t numFirstOrderImages = 6;
    ISM_reflectSources(room.walls, ISM_ROOM_NUM_WALLS,
                       s, 1,
                       imageSources1stOrder, numFirstOrderImages);
    
    // do second order reflections
    ISMImageSource imageSources2ndOrder [18];
    size_t numSecondOrderImages = 18;
    ISM_reflectSources(room.walls, ISM_ROOM_NUM_WALLS,
                       imageSources1stOrder, 6,
                       imageSources2ndOrder, numSecondOrderImages);
    
    // do third order reflections
    ISMImageSource imageSources3rdOrder [38];
    size_t numThirdOrderImages = 38;
    ISM_reflectSources(room.walls, ISM_ROOM_NUM_WALLS,
                       imageSources2ndOrder, 18,
                       imageSources3rdOrder, numThirdOrderImages);
    
    printf("First order: {");
    for(size_t i=0; i<numFirstOrderImages; i++){
        ISM_printImageSource(imageSources1stOrder[i]);
        printf(", ");
    }
    printf("}\n\n");
    
    printf("Second order: {");
    for(size_t i=0; i<numSecondOrderImages; i++){
        ISM_printImageSource(imageSources2ndOrder[i]);
        printf(", ");
    }
    printf("}\n\n");
    
    
    printf("Third order: {");
    for(size_t i=0; i<numThirdOrderImages; i++){
        ISM_printImageSource(imageSources3rdOrder[i]);
        printf(", ");
    }
    printf("}\n\n");
    
    // how many duplicates between 2nd and 3rd order?
    size_t duplicates = ISM_countDuplicates(imageSources2ndOrder, numSecondOrderImages, imageSources3rdOrder, numThirdOrderImages);
    printf("2,3 order duplicates: %zu\n", duplicates);
    // how many duplicates between 1st and 3rd order?
    duplicates = ISM_countDuplicates(imageSources1stOrder, numFirstOrderImages, imageSources3rdOrder, numThirdOrderImages);
    printf("1,3 order duplicates: %zu\n", duplicates);
    duplicates = ISM_countDuplicates(imageSources1stOrder, numFirstOrderImages, imageSources2ndOrder, numSecondOrderImages);
    printf("1,2 order duplicates: %zu\n", duplicates);
    
}



void ISMTests2(size_t reflectionMaxOrder){
    
    // allocate memory for the outputs of the simulation
    ISMVector3D* imageSources;
    float* reflectionTimes;
    float* reflectionGains;
    size_t* reflectionOrders;
    imageSources = malloc(sizeof(ISMImageSource)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionTimes = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionGains = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionOrders = malloc(sizeof(ISMImageSource)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    
    
    // set up inputs for the simulation
    float length = 3.0;
    float height = 4.0;
    float width  = 5.0;
    float absorptionCoefficient = 0.9;
    float speedOfSound = 340.0;
    ISMVector3D sourcePosition   = (ISMVector3D){1,1,1};
    ISMVector3D listenerLeftEar  = (ISMVector3D){2,2,2};
    ISMVector3D listenerRightEar = (ISMVector3D){2,2.2,2};
    
    // do the simulation
    ISM_simulateRoom(length, height, width,
                     absorptionCoefficient,
                     speedOfSound,
                     reflectionMaxOrder,
                     sourcePosition,
                     listenerLeftEar,
                     listenerRightEar,
                     imageSources,           // (output) source positions
                     reflectionTimes,        // (output)
                     reflectionGains,        // (output)
                     reflectionOrders);      // (output) order of each I.S.
    
    printf("Reflection Gains {");
    for(size_t i=0; i<ISM_totalSourcesForOrder(reflectionMaxOrder); i++)
      printf("%f, ",reflectionGains[i]);
    printf("}\n\n");
    
    printf("Reflection Times {");
    for(size_t i=0; i<ISM_totalSourcesForOrder(reflectionMaxOrder); i++)
        printf("%f, ",reflectionTimes[i]);
    printf("}\n\n");
}
