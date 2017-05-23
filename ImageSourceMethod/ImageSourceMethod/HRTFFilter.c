//
//  HRTFFilter.c
//  Head Related Transfer Function, first order model
//
//  Created by Hans on 22/5/17.
//  Everyone may use this code freely without restrictions.
//

#include <math.h>
#include "HRTFFilter.h"




/*
 * process one sample of input through the filter
 */
float HRTF_process(HRTFFilter* This, float sample){
    float out = sample*This->b0 + This->zb*This->b1 - This->za*This->a1;
    This->zb = sample;
    This->za = out;
    return out;
}






/*
 * Sets the angle of incidence for a first order
 * Head Related Transfer Function filter model.
 *
 * @param theta - angle from the source to the ear with 0 pointing in
 *                  the direction out from the ear
 * @param fs    - sample rate
 */
void HRTF_setAngle(HRTFFilter* This, float theta, float fs){
    
    float theta0 = 150.0f ;
    float alpha_min = 0.1f ;
    float c = 334.0f; // speed of sound
    float a = 0.08f; // radius of head
    float w0 = c/a;
    float alpha = (1.0f + alpha_min/2.0f) +
                  (1.0f - alpha_min/2.0f) * cos((theta / theta0) * M_PI);
    
    This->b0 =  (alpha+w0/fs)/(1.0f+w0/fs);
    This->b1 = (-alpha+w0/fs)/(1.0f+w0/fs);
    This->a1 =   -(1.0f-w0/fs)/(1.0f+w0/fs);
    
    This->za = This->zb = 0.0f;
}




float HRTF_angleForLeftEar(float frontZeroAngle){
    return frontZeroAngle + 90.0f;
}



float HRTF_angleForRightEar(float frontZeroAngle){
    return frontZeroAngle - 90.0f;
}
