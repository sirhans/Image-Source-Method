//
//  ERImpulseResponse.h
//  ImageSourceMethod
//
//  Created by Hans on 22/5/17.
//  Copyright © 2017 Hans. All rights reserved.
//

#ifndef ERImpulseResponse_h
#define ERImpulseResponse_h

#define HRTF_IR_LENGTH 64 // length in samples of filter impulse responses

#include <stdio.h>

/*
 * Given angles, gains and delay times for left and right channels, 
 * this function produces the stereo impulse response for early reflections
 *
 * @param timesL      - delay tap times
 * @param timesR      - delay tap times
 * @param gainsL      - delay tap gains
 * @param gainsR      - delay tap gains
 * @param anglesL     - angle from left ear for each delay tap
 * @param anglesR     - angle from right ear for each delay tap
 * @param leftOut     - left channel audio output
 * @param rightOut    - right channel audio output
 * @param numSources  - length of the input arrays (all must be the same)
 * @param outLength   - length in samples of the output arrays
 * @param fs          - sample rate
 */
void ERImpulseResponse(const float* timesL,  const float* timesR,
                       const float* gainsL,  const float* gainsR,
                       const float* anglesL, const float* anglesR,
                       float* leftOut, float* rightOut,
                       size_t numSources, size_t outLength,
                       float fs);


#endif /* ERImpulseResponse_h */
