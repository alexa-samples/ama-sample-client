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

var bleno = require('bleno');
var AmaService = require('./amaService');
var service = new AmaService();

// Some global values
global.name = 'AMA Sample Client'; // Name of your device
global.deviceID = "AAAAAAAAAAA"; // Device ID from devportal
global.deviceSerial = "20180527000006"; // Serial num of this device
global.mainService = service; // This is our global singleton

bleno.on('stateChange', function(state) {
  if (state === 'poweredOn') {
    //
    // We will also advertise the service ID in the advertising packet,
    // so it's easier to find.
    //
    console.log("BLE starting...");
    console.log("Device name: " + global.name);
    bleno.startAdvertising(global.name, [service.uuid], function(err) {
      if (err) {
        console.log(err);
      }      
      
    });
  }
  else {
    console.log(state);
    bleno.stopAdvertising();
  }
});

bleno.on('advertisingStart', function(err) {
  if (err) {
    console.log("Advertising start error:" + error);
    return;
  }

 console.log('Advertising...');
    bleno.setServices([
      service
    ]);

  }
);

const readline = require('readline');
readline.emitKeypressEvents(process.stdin);
process.stdin.setRawMode(true);
process.stdin.on('keypress', (str, key) => {
  if (key.name == 's') {
     service.startAudio();
  }
  if (key.name == 'q') {
    service.closeAudio();
    console.log("Stopped listening...");
    process.exit();
  }
});

console.log("Press s each time you speak...");
console.log("Press q to quit the client...");

