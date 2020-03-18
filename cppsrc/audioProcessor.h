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

#ifndef AUDIOPROCESSOR_H
#define  AUDIOPROCESSOR_H

class AlsaSource;
class OurOpusEncoder;

class AudioProcessor {
public:
   AudioProcessor();
   ~AudioProcessor();

   int open(const char* micDevice, int samplingFreq = 48000);
   int close();

   // both the methods for  opus
   int getMaxBufSize();
   void setBuffer(unsigned char* buf);

   int  processAudio();

private:
   AlsaSource* _micInput;
   OurOpusEncoder* _encoder;
};

#endif //  AUDIOPROCESSOR_H

