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

#ifndef _OPUSCODEC_H
#define _OPUSCODEC_H

#include <opus/opus.h>

class OurOpusEncoder {
  public:
    OurOpusEncoder();
    ~OurOpusEncoder();

    int open(int frequency, int numChannels, int bitRate, int frameSize);

    int getMaxBufSize() {return this->_maxBufSize; }
    void setBuffer(unsigned char* buf);

    int encode(const void *data);
    unsigned char* getOutput();
    int close();

    static const char *getError(int err);

  private:
    OpusEncoder* _encoder;
    unsigned char* _buf;
    int _frameSize;
    int _maxBufSize;
};

class OurOpusDecoder {
  public:
    OurOpusDecoder();
    ~OurOpusDecoder();

    int open(int frequency, int numChannels, int bitRate, int frameSize);
    int decode(const unsigned char* data, int dataSize);
    const unsigned char* getOutput();
    int close();

  private:
    OpusDecoder* _decoder;
    unsigned char* _buf;
    int _frameSize;
    int _maxBufSize;
};

#endif // _OPUSCODEC_H
