/*

Copyright Amazon.com, Inc. and its affiliates. All Rights Reserved.
 
SPDX-License-Identifier: LicenseRef-.amazon.com.-AmznSL-1.0

Licensed under the Amazon Software License (the "License").
You may not use this file except in compliance with the License.
A copy of the License is located at

  http://aws.amazon.com/asl/

or in the "license" file accompanying this file. This file is distributed
on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
express or implied. See the License for the specific language governing
permissions and limitations under the License.

*/

#include "audioProcessor.h"
#include "micInput.h"
#include "opusCodec.h"
#include <stdio.h>

// From AMA  specifications
const snd_pcm_format_t SNDFORMAT  = SND_PCM_FORMAT_S16_LE;
const int NUMCHANNELS = 2;
const float FRAMELENGTH  = 20.0; // each frame length in milliseconds
const int OPUSBITRATE = 32000;

AudioProcessor::AudioProcessor() {
  this->_micInput = 0;
  this->_encoder = 0;
}

AudioProcessor::~AudioProcessor() {
  close();
}

int AudioProcessor::open(const char* micDevice, int samplingFreq) { 
  // printf("Open: %s. Freq: %d\n", micDevice, samplingFreq);

  float numFramesF = ((float) samplingFreq) / 1000.0f * FRAMELENGTH; 
  int numFrames = (int) numFramesF;

  this->_micInput = new AlsaSource(micDevice, SNDFORMAT, NUMCHANNELS,
	samplingFreq, numFrames);
  int err = this->_micInput->open();
  if (err != 0) return err;

  this->_encoder = new OurOpusEncoder();
  err = this->_encoder->open(samplingFreq, NUMCHANNELS, OPUSBITRATE, numFrames);

  return err;
}

int AudioProcessor::close() {
  // printf("Close\n");
  delete _encoder; _encoder = 0;
  delete _micInput; _micInput = 0;

  return 0;
}

int AudioProcessor::getMaxBufSize() {
   return this->_encoder->getMaxBufSize();
}

void AudioProcessor::setBuffer(unsigned char* buf) {
   this->_encoder->setBuffer(buf+4); // ignore first four bytes
}

int AudioProcessor::processAudio() {
   int bufSize, err;
   const unsigned char* buffer = this->_micInput->readData(bufSize, err);

   int numBytes = this->_encoder->encode(buffer);

   return numBytes;
}

