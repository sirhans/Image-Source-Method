//
//  HRTFFilter.c
//  Head Related Transfer Function, first order model
//
//  Created by Hans on 22/5/17.
//  Everyone may use this code freely without restrictions.
//

#ifndef HRTFFilter_h
#define HRTFFilter_h

#include <stdio.h>
#include <stdbool.h>



typedef struct HRTFFilter{
    float za, zb;     // one sample delay units
    float a1, b0, b1; // first order IIR filter coefficients
} HRTFFilter;



/*
 * process one sample of input through the filter
 */
float HRTF_process(HRTFFilter* This, float sample);





/*
 * Sets the angle of incidence for a first order
 * Head Related Transfer Function filter model.
 *
 * @param theta - angle from the source to the listener with 0 at 12 o'clock
 *                positive angles go toward the ear that the filter is 
 *                modeling, so negative angles are on the opposite side of
 *                the head.
 * @param fs    - sample rate
 */
void HRTF_setAngle(HRTFFilter* This, float theta, float fs);



/*
 * In the 3d model we put 0 degrees toward the front of the listener's face
 * but the HRTF filters put 0 degrees outward towards the left or right ear.
 *
 * This function converts from 0 degrees at front units to 0 degrees at 
 * left ear units.
 */
float HRTF_angleForLeftEar(float frontZeroAngle);



/*
 * In the 3d model we put 0 degrees toward the front of the listener's face
 * but the HRTF filters put 0 degrees outward towards the left or right ear.
 *
 * This function converts from 0 degrees at front units to 0 degrees at
 * right ear units.
 */
float HRTF_angleForRightEar(float frontZeroAngle);


#endif /* HRTFFilter_h */
