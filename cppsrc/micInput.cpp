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

#include "micInput.h"
#include <stdio.h> // fprintf

AlsaSource::AlsaSource(const char* deviceName, snd_pcm_format_t format, unsigned numChannels, unsigned samplingFreq, unsigned numFrames) { 
    this->_deviceName = deviceName;
    this->_format = format;
    this->_numChannels = numChannels;
    this->_samplingFreq = samplingFreq;
    this->_numFrames = numFrames;

    this->_handle = 0;
    this->_buffer = 0;
}

AlsaSource::~AlsaSource() { 
    close(); 
}

int AlsaSource::close() { 
    if (this->_handle != 0) {
        snd_pcm_close(this->_handle);
        this->_handle = 0;

        // https://stackoverflow.com/questions/13478861/alsa-mem-leak
        snd_config_update_free_global(); 
    }

    delete[] this->_buffer;
    this->_buffer = 0;

    return 0;
}

static int openSoundDevice(bool isRecording, 
   snd_pcm_t*& _handle, const std::string& _deviceName,
   snd_pcm_format_t _format, unsigned _numChannels, unsigned _samplingFreq
) {
  _snd_pcm_stream streamType =
      isRecording ? SND_PCM_STREAM_CAPTURE : SND_PCM_STREAM_PLAYBACK;

  int err  = snd_pcm_open (&_handle, _deviceName.c_str(), streamType, 0);

  if (err  < 0) {
    fprintf (stderr, "cannot open audio device %s (%s)\n",
	     _deviceName.c_str(), snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "audio interface opened\n");

  snd_pcm_hw_params_t* hw_params;

  if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
    fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
       snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params allocated\n");

  if ((err = snd_pcm_hw_params_any (_handle, hw_params)) < 0) {
    fprintf (stderr,
        "Cannot initialize hardware parameter structure (%s)\n",
        snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params initialized\n");

  if ((err =
       snd_pcm_hw_params_set_access (_handle, hw_params,
				     SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
    fprintf (stderr, "cannot set access type (%s)\n", snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params access setted\n");

  if ((err =
       snd_pcm_hw_params_set_format (_handle, hw_params, _format)) < 0)
  {
    fprintf (stderr, "cannot set sample format (%s)\n", snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params format setted\n");

  if ((err =
       snd_pcm_hw_params_set_rate_near (_handle, hw_params, &_samplingFreq,
					0)) < 0) {
    fprintf (stderr, "cannot set sample rate (%s)\n", snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params rate setted\n");

  if ((err =
       snd_pcm_hw_params_set_channels (_handle, hw_params, _numChannels)) < 0) {
    fprintf (stderr, "cannot set channel count (%s)\n", snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params channels setted\n");

  if ((err = snd_pcm_hw_params (_handle, hw_params)) < 0) {
    fprintf (stderr, "cannot set parameters (%s)\n", snd_strerror (err));
    return err;
  }

  // fprintf (stdout, "hw_params set\n");

  snd_pcm_hw_params_free (hw_params);

  // fprintf (stdout, "hw_params freed\n");

  if ((err = snd_pcm_prepare (_handle)) < 0) {
    fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
	     snd_strerror (err));
    return err;
  }

  fprintf (stdout, "Audio interface prepared...\n");

  return err;
} 

int AlsaSource::open() {
  int err = openSoundDevice(true, _handle, _deviceName, _format, _numChannels,
                            _samplingFreq);

  if (err != 0) {
    return err;
  }

  int formatWidth = snd_pcm_format_width(_format) / 8 * _numChannels; // 2 bytes per channel. We have 2 channels
  _bufSize = _numFrames * formatWidth;

  // fprintf(stdout, "Bufsize: %d\n", _bufSize);
  _buffer = new unsigned char[_bufSize];

  return 0;
}

const unsigned char* AlsaSource::readData(int& size, int& err) {
  err = snd_pcm_readi(_handle, _buffer, _numFrames);
  
  if ((unsigned) err != _numFrames) {
      fprintf (stderr, "read from audio interface failed %d: (%s)\n",
	       err, snd_strerror (err));
      return 0;
  }

  size = _bufSize;
  return _buffer;
}

std::string AlsaSource::getError(int err) { 
    return snd_strerror(err); 
}

