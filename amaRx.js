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

var util = require('util');
var bleno = require('bleno');
var generalUtils = require('./generalUtils.js');

function AmaRxCharacteristic() {
  bleno.Characteristic.call(this, {
    uuid: '2BEEA05B-1879-4BB4-8A2F-72641F82420B',
    properties: [ 'read', 'notify'],
    value: null
  });  
  this._updateValueCallback = null;
}

util.inherits(AmaRxCharacteristic, bleno.Characteristic);


AmaRxCharacteristic.prototype.onReadRequest = function(offset, updateValueCallback) {
  // console.log('Phone is trying to read. Why???');  
  updateValueCallback(this.RESULT_SUCCESS, new Buffer(0));
}

AmaRxCharacteristic.prototype.onSubscribe = function(maxValueSize, updateValueCallback) {
  console.log('Connection established...');  
  this._updateValueCallback = updateValueCallback;

  var a = buildFirstPacket();
  var buf = Buffer.from(a);
  this.sendData(buf);
  
/*
    console.log('Buffer: ', buf);
    updateValueCallback(buf);
*/
};

AmaRxCharacteristic.prototype.onUnsubscribe = function() {
  console.log('AmaRxCharacteristic - onUnsubscribe');
  this._updateValueCallback = null;
};

AmaRxCharacteristic.prototype.sendData = function(data) {
  if (data.length < 80) {
  var s = generalUtils.arrayToHexString(data);
   console.log("S: ", s);
  }else {
    // console.log("S: %d bytes", data.length)
  }
   this._updateValueCallback(data);
}


function buildFirstPacket() {
    var a = new Uint8Array([
        0xFE,
        0x03,
        0x01,  // Major version
        0x00,
        0x03,  // transport packet size 800 = 0x0320
        0x20,
        0x13,  // max data size 5000 = 0x1388
        0x88,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    ]);
    return a;

}

module.exports = AmaRxCharacteristic;
