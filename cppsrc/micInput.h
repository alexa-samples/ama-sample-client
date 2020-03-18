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

#ifndef _PIPELINE_ALSA_H
#define  _PIPELINE_ALSA_H

#include <alsa/asoundlib.h>
#include <string>

class AlsaSource {
public:
 AlsaSource(const char* deviceName = "default",
            snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE,
            unsigned numChannels = 2, unsigned samplingFreq = 48000,
            unsigned numFrames = 960);
 virtual ~AlsaSource();

 virtual int open();
 virtual const unsigned char* readData(int& size, int& err);
 virtual int close();
 virtual std::string getError(int err);

private:
 std::string _deviceName;
 snd_pcm_format_t _format;
 unsigned _numChannels;
 unsigned _samplingFreq;
 unsigned _numFrames;

 snd_pcm_t* _handle;
 unsigned char* _buffer;
 int _bufSize;
};


#endif // _PIPELINE_ALSA_H
