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

'use strict';

var isPi = require('detect-rpi');

const micDevicePi = "plughw:1,0"; // For Pi
const micDeviceUbu = "default"; // for Ubuntu
const micDevice = isPi() ? micDevicePi : micDeviceUbu;
const sampFreq = 48000;

const apLib = require('./build/Release/audioprocessor.node');

function audioSender() {
  // Create our audio processor and allocate buffer
  var audioProcessor = new apLib.APWrapper();
  audioProcessor.open(micDevice, sampFreq);

  var maxBufSize = audioProcessor.getMaxBufSize();
  var opusBuf = Buffer.allocUnsafe(4 + maxBufSize); // First 4 bytes are the header
  opusBuf[0] = 0x10;
  opusBuf[1] = 0x81;
  audioProcessor.setBuffer(opusBuf);

  this._audioProcessor = audioProcessor;
  this._opusBuf = opusBuf;
}

var readAudio = true;
var readSize = 0;

audioSender.prototype.sendAudio = function() {
    if (readAudio) {
      readSize = this._audioProcessor.processAudio();
      this._opusBuf.writeUInt16BE(readSize, 2); // write at location two
    // fs.writeSync(fd, opusBuf, 0, readSize);
    }else {
      global.mainService.sendAudio(this._opusBuf.slice(0, 4+readSize));
    }
    readAudio = !readAudio;
};

audioSender.prototype.close = function() {
  this._audioProcessor.close();
};

module.exports = audioSender;

