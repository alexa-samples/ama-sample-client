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

var accessories_pb = require('./amaprotos/accessories_pb');
var device_pb = require('./amaprotos/device_pb.js');
var common_pb = require('./amaprotos/common_pb.js');

// ce is an instance of ControlEnvelope
function buildControlBuffer(ce) {
    var buf = ce.serializeBinary();

    var finalArray = new Uint8Array(3 + buf.length);

    // Hardcoding the first two bytes. More efficient.
    finalArray[0] = 0x10;
    finalArray[1] = 0x00;

    finalArray[2] = buf.length;
    finalArray.set(buf, 3);

    // Uncomment the lines for debugging
    // var s = arrayToHexString(finalArray);
    // fs.writeFileSync('/tmp/my.txt', s);

    return finalArray;
}

function buildUnsupportedResponse(cmd) {
    var res = new accessories_pb.Response();
    res.setErrorCode(common_pb.ErrorCode.UNSUPPORTED);

    var ce = new accessories_pb.ControlEnvelope();
    ce.setCommand(cmd);
    ce.setResponse(res);

    return buildControlBuffer(ce);    
}

// For debugging purposes
// Builds two-digit hex values separated by space
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

module.exports.buildControlBuffer = buildControlBuffer;
module.exports.buildUnsupportedResponse = buildUnsupportedResponse;
