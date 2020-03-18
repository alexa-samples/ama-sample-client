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
var protos = require('./amaprotos/accessories_pb');
var generalUtils = require('./generalUtils');

function AmaTxCharacteristic() {
  bleno.Characteristic.call(this, {
    uuid: 'F04EB177-3005-43A7-AC61-A390DDF83076',
    properties: [ 'write', 'writeWithoutResponse'],
    value: null
  });  
}

util.inherits(AmaTxCharacteristic, bleno.Characteristic);

AmaTxCharacteristic.prototype.onWriteRequest = function(data, offset, withoutResponse, updateValueCallback) {
    console.log("R: ", generalUtils.arrayToHexString(data));
	 
     global.mainService.processReceivedMsg(data);
     updateValueCallback(this.RESULT_SUCCESS);
};

module.exports = AmaTxCharacteristic;

