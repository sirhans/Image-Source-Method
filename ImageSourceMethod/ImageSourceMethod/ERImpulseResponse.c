//
//  ERImpulseResponse.c
//  ImageSourceMethod
//
//  Created by Hans on 22/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#include <string.h>
#include "ERImpulseResponse.h"
#include "HRTFFilter.h"
#include <math.h>


#define HRTF_NUM_ANGLES 360 // number of angles for which to save IRs





/*
 * find the index in the IR cache of the IR closest to angle
 */
size_t angleToIdx(float angle){
    // wrap angle into [0,360)
    while(angle < 0.0) angle += 360.0f;
    while(angle >= 360.0) angle -= 360.0f;
    
    float idxf = (float)HRTF_NUM_ANGLES * (angle/360.0f);
    return (size_t)floor(idxf);
}







void ERImpulseResponse(const float* timesL, const float* timesR,
                       const float* gainsL, const float* gainsR,
                       const float* anglesL, const float* anglesR,
                       float* leftOut, float* rightOut,
                       size_t numSources, size_t outLength,
                       float fs){
    
    // initialize storage for filter impulse responses
    float impulseResponses [HRTF_NUM_ANGLES][HRTF_IR_LENGTH];
    
    // initialize storage for filter impulse input in the form {1,0,0,...}
    float inputImpulse [HRTF_IR_LENGTH];
    // clear to zero
    memset(inputImpulse,0,sizeof(float)*HRTF_IR_LENGTH);
    // set initial impulse
    inputImpulse[0] = 1.0f;
    
    // create an HRTF filter object
    HRTFFilter filter;
    
    // compute filter impulse responses for each angle from 0 to HRTF_NUM_ANGLES
    for(size_t i=0; i<HRTF_NUM_ANGLES; i++){
        // set the filter to model angle = i/HRTF_NUM_ANGLES
        float angle = 360.0f*(float)i/(float)HRTF_NUM_ANGLES;
        HRTF_setAngle(&filter, angle, fs);
        
        // generate the impulse response for angle = i/HRTF_NUM_ANGLES
        for(size_t j=0; j<HRTF_IR_LENGTH; j++)
            impulseResponses[i][j] = HRTF_process(&filter, inputImpulse[j]);
    }
    
    // initialize the L and R outputs to zero
    memset(leftOut,0,sizeof(float)*outLength);
    memset(rightOut,0,sizeof(float)*outLength);
    
    // for each output tap, copy the filter impulse response at the
    // corresponding angle into the audio output
    for(size_t i=0; i<numSources; i++){
        // find the start position in the output for copying this
        // filter IR
        size_t startPositionL = timesL[i]*fs;
        size_t startPositionR = timesR[i]*fs;
        
        // find the index where the filter IR for anglesL[i] is stored
        size_t angleIdxL = angleToIdx(anglesL[i]);
        size_t angleIdxR = angleToIdx(anglesR[i]);
        
        // copy the filter IR to output
        for(size_t j=0; j<HRTF_IR_LENGTH; j++){
            // copy the L and R filter impulses to the output, scalaing
            // according to the gain coefficient of the output tap
            leftOut[j+startPositionL] =
                        impulseResponses[angleIdxL][j]*gainsL[i];
            rightOut[j+startPositionR] =
                        impulseResponses[angleIdxR][j]*gainsR[i];
        }
    }
}
