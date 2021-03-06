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
#include "ERImpulseResponse.h"
#include "ISMVector2D.h"

// forward declaration of test function
void ISMTests1();
void ISMTests2(size_t reflectionMaxOrder);
void ISMImpulseResponseTest();


int main(int argc, const char * argv[]) {
    // ISMTests1();
    
    // set the model to go up to this order of reflections
    //size_t reflectionMaxOrder = 3;
    
    // test it
    //ISMTests2(reflectionMaxOrder);
    
    ISMImpulseResponseTest();
    
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
    float* reflectionTimesL, *reflectionTimesR;
    float* reflectionGainsL, *reflectionGainsR;
    size_t* reflectionOrders;
    imageSources = malloc(sizeof(ISMImageSource)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionTimesL = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionTimesR = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionGainsL = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionGainsR = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionOrders = malloc(sizeof(ISMImageSource)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    
    
    // set up inputs for the simulation
    float length = 3.0;
    float height = 4.0;
    float width  = 5.0;
    float absorptionCoefficient = 0.9;
    float speedOfSound = 340.0;
    float micDistance = 0.3; // this is d_M
    ISMVector3D sourcePosition   = (ISMVector3D){1,1,1};
    ISMVector3D listenerLeftEar  = (ISMVector3D){2,2,2};
    ISMVector3D listenerRightEar = (ISMVector3D){2,2.2,2};
    
    // do the simulation
    ISM_simulateRoom(length, height, width,
                     absorptionCoefficient,
                     speedOfSound,
                     micDistance,
                     reflectionMaxOrder,
                     sourcePosition,
                     listenerLeftEar,
                     listenerRightEar,
                     imageSources,           // (output) source positions
                     reflectionTimesL,       // (output)
                     reflectionTimesR,       // (output)
                     reflectionGainsL,       // (output)
                     reflectionGainsR,       // (output)
                     reflectionOrders);      // (output) order of each I.S.
    
    printf("Reflection Gains L {");
    for(size_t i=0; i<ISM_totalSourcesForOrder(reflectionMaxOrder); i++)
      printf("%f, ",reflectionGainsL[i]);
    printf("}\n\n");
    
    printf("Reflection Times L {");
    for(size_t i=0; i<ISM_totalSourcesForOrder(reflectionMaxOrder); i++)
        printf("%f, ",reflectionTimesL[i]);
    printf("}\n\n");
    
    // free memory
    free(imageSources);
    free(reflectionTimesL);
    free(reflectionTimesR);
    free(reflectionGainsL);
    free(reflectionGainsR);
    free(reflectionOrders);
}


void ISMImpulseResponseTest(){
    size_t reflectionMaxOrder = 3;
    
    // allocate memory for the outputs of the simulation
    ISMVector3D* imageSources;
    float* reflectionTimesL, *reflectionTimesR;
    float* reflectionGainsL, *reflectionGainsR;
    size_t* reflectionOrders;
    imageSources = malloc(sizeof(ISMImageSource)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionTimesL = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionTimesR = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionGainsL = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionGainsR = malloc(sizeof(float)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    reflectionOrders = malloc(sizeof(ISMImageSource)*ISM_totalSourcesForOrder(reflectionMaxOrder));
    
    
    // set up inputs for the simulation
    float length = 3.0;
    float height = 4.0;
    float width  = 5.0;
    float absorptionCoefficient = 0.9;
    float speedOfSound = 340.0;
    float micDistance = 0.3; // this is d_M
    ISMVector3D sourcePosition   = (ISMVector3D){1,1,1};
    ISMVector3D listenerLeftEar  = (ISMVector3D){2,2,2};
    ISMVector3D listenerRightEar = (ISMVector3D){2,2,2.16};
    
    // do the simulation
    ISM_simulateRoom(length, height, width,
                     absorptionCoefficient,
                     speedOfSound,
                     micDistance,
                     reflectionMaxOrder,
                     sourcePosition,
                     listenerLeftEar,
                     listenerRightEar,
                     imageSources,           // (output) source positions
                     reflectionTimesL,       // (output)
                     reflectionTimesR,       // (output)
                     reflectionGainsL,       // (output)
                     reflectionGainsR,       // (output)
                     reflectionOrders);      // (output) order of each I.S.
    

    // find the longest reflection time
    float longestReflectionTime = 0.0f;
    size_t numSources = ISM_totalSourcesForOrder(reflectionMaxOrder);
    for(size_t i=0; i<numSources; i++){
        if(reflectionTimesL[i] > longestReflectionTime)
            longestReflectionTime = reflectionTimesL[i];
        if(reflectionTimesR[i] > longestReflectionTime)
            longestReflectionTime = reflectionTimesR[i];
    }
        
    
    
    // allocate memory for producing the impulse response
    float sampleRate = 44100;
    size_t ISM_IRLength = 1+(longestReflectionTime*sampleRate)+HRTF_IR_LENGTH;
    float* leftOut  = malloc(sizeof(float)*ISM_IRLength);
    float* rightOut = malloc(sizeof(float)*ISM_IRLength);
    float* anglesLeft = malloc(sizeof(float)*numSources);
    float* anglesRight = malloc(sizeof(float)*numSources);

    
    // get the angle for each source, from right and left ears
    ISMVector2D leftToRightEar = flatten2D(v3_sub(listenerRightEar, listenerLeftEar));
    ISMVector2D faceFront = v2_counterClockwisePerpendicular(leftToRightEar);
    for(size_t i=0; i<numSources; i++){
        ISMVector3D earToSourceL = v3_sub(imageSources[i], listenerLeftEar);
        ISMVector3D earToSourceR = v3_sub(imageSources[i], listenerRightEar);
        ISMVector2D leftDirection = flatten2D(earToSourceL);
        ISMVector2D rightDirection = flatten2D(earToSourceR);
        anglesLeft[i]  = v2_angle(faceFront, leftDirection);
        anglesRight[i] = v2_angle(rightDirection, faceFront);
    }
    
    ERImpulseResponse(reflectionTimesL, reflectionTimesR,
                      reflectionGainsL, reflectionGainsR,
                      anglesLeft, anglesRight,
                      leftOut, rightOut,
                      numSources, ISM_IRLength,
                      sampleRate);
    
    // print impulse response Left
    printf("left impulse response: \n\n{");
    for(size_t i=0; i<ISM_IRLength; i++)
        printf("%f, ",leftOut[i]);
    // right
    printf("\n\n\nright impulse response: \n\n{");
    for(size_t i=0; i<ISM_IRLength; i++)
        printf("%f, ", rightOut[i]);
        
    
    
    // free memory
    free(imageSources);
    free(reflectionTimesL);
    free(reflectionTimesR);
    free(reflectionGainsL);
    free(reflectionGainsR);
    free(reflectionOrders);
    free(leftOut);
    free(rightOut);
    free(anglesLeft);
    free(anglesRight);
}
