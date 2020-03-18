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

const fs = require('fs');

function arrayToHexString(a) {

    var ret = "";
    var len = a.length;

    for(let i=0;i<len;i++) {
        var x = a[i];
        var value = ('0' + x.toString(16)).slice(-2);

        if (i > 0) {
            ret = ret + ' ';
        }

        ret = ret + value;
    }

    return ret;
}

module.exports.arrayToHexString = arrayToHexString;


var accessories_pb  = require('./amaprotos/accessories_pb');

function lookupCommand(val) {
   var l = accessories_pb.ControlEnvelope.PayloadCase;
   for(var item in l) {
       if (l[item] == val) return item;
   }
   return "Unknown";
}

function lookupSpeechState(val) {
   var l = accessories_pb.SpeechState;
   for(var item in l) {
       if (l[item] == val) return item;
   }
   return "Unknown";
}

module.exports.lookupCommand = lookupCommand;
module.exports.lookupSpeechState = lookupSpeechState;


