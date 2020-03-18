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

#include "opusCodec.h"
#include <iostream>

OurOpusEncoder::OurOpusEncoder() {
    this->_buf = 0;
    this->_encoder = 0;

    this->_frameSize = this->_maxBufSize = 0;
}

OurOpusEncoder::~OurOpusEncoder() {
    close();
}

int OurOpusEncoder::open(int frequency, int numChannels, int bitRate, int frameSize) {
    if (this->_encoder != 0) {
        std::cerr << "Encoder already open" << std::endl;
        exit(1);
    }

    int err;
    this->_encoder = opus_encoder_create(frequency, numChannels, OPUS_APPLICATION_VOIP, &err);
    if (err != 0) {
        return err;
    }

    err = opus_encoder_ctl(this->_encoder, OPUS_SET_BITRATE(bitRate));
    if (err != 0) {
        return err;
    }

    // CBR Only
    err = opus_encoder_ctl(_encoder, OPUS_SET_VBR(0));
    if (err != OPUS_OK) {
        return false;
    }

    // 20ms framesize
    err = opus_encoder_ctl(_encoder, OPUS_SET_EXPERT_FRAME_DURATION(OPUS_FRAMESIZE_20_MS));
    if (err != OPUS_OK) {
        return false;
    }

    this->_frameSize = frameSize;
    this->_maxBufSize = frameSize * 2 * numChannels;

    // NOTE: In our case, the buf is being set by the caller
    // this->_buf = new uint8_t[_maxBufSize];

    return 0;
}

void OurOpusEncoder::setBuffer(unsigned char* buf) {
   this->_buf = buf;
}

int OurOpusEncoder::encode(const void* data) {
    int numBytes = opus_encode(this->_encoder, reinterpret_cast<const opus_int16 *>(data), this->_frameSize, this->_buf, this->_maxBufSize);
    return numBytes;
}

unsigned char* OurOpusEncoder::getOutput() {
    return this->_buf;
}

int OurOpusEncoder::close() {
    if (this->_encoder != 0) {
        opus_encoder_destroy(this->_encoder);
        this->_encoder = 0;
    }

    // NOTE: WE DONT OWN THIS BUF
    // delete[] this->_buf; 
    this->_buf = 0;

    return 0;
}

const char* OurOpusEncoder::getError(int err) {
    return opus_strerror(err);
}

OurOpusDecoder::OurOpusDecoder() {
    this->_decoder = 0;
    this->_buf = 0;
    this->_maxBufSize = this->_frameSize = 0;
}
OurOpusDecoder::~OurOpusDecoder() {
    close();
}

int OurOpusDecoder::open(int frequency, int numChannels, int bitRate, int frameSize) {
    if (this->_decoder != 0) {
        std::cerr << "Decoder already open" << std::endl;
        exit(1);
    }

    int err;
    this->_decoder = opus_decoder_create(frequency, numChannels, &err);
    if (err != 0) {
        return err;
    }

    this->_frameSize = frameSize;
    this->_maxBufSize = this->_frameSize * numChannels * 2;
    this->_buf = new uint8_t[this->_maxBufSize];

    return 0;
}

int OurOpusDecoder::decode(const uint8_t* data, int dataSize) {
    int numBytes = opus_decode(this->_decoder, data, dataSize, reinterpret_cast<opus_int16*>(this->_buf), this->_frameSize, 0);
    return numBytes;
}

const uint8_t* OurOpusDecoder::getOutput() {
    return this->_buf;
}

int OurOpusDecoder::close() {
    if (this->_decoder != 0) {
        opus_decoder_destroy(this->_decoder);
        this->_decoder = 0;
    }
    delete[] this->_buf;
    this->_buf = 0;
    return 0;
}
