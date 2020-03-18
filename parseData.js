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

var Parser = require("binary-parser").Parser;
var protos = require('./amaprotos/accessories_pb');

function parseData(data) {
        var transHeader = new Parser()
                .endianess("big")
                .bit4("version")
                .bit5("streamId")
                .bit6("reserved")
                .bit1("is16Bits")
                ;

        var parsedHeader = transHeader.parse(data);
    // console.log(parsedHeader);

    var len = 0;
    var payloadOffset = 0;
    if (parsedHeader.is16Bits) {
      len = data.readUInt16BE(2);
      payloadOffset = 4;
    }else {
      len = data.readUInt8(2);
      payloadOffset = 3;
    }
    // console.log(len);

   var payload = data.slice(payloadOffset);

   // console.log(payload);

  if (parsedHeader.streamId == 0) {
     var msg = protos.ControlEnvelope.deserializeBinary(payload);
     // console.log(msg);
     // console.log("Command: ", msg.getCommand());
     return {
       streamId : 0,
       data : msg
     };
  }

  return {
      streamId : 1,
      data : payload
  };
}

module.exports = parseData;

