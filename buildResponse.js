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

var accessories_pb = require('./amaprotos/accessories_pb');
var device_pb = require('./amaprotos/device_pb.js');
var common_pb = require('./amaprotos/common_pb.js');
var builders = require('./buildControlBuffer.js');

function buildResponse(cmd, obj) {

    var res = new accessories_pb.Response();

    switch(cmd) {
      case accessories_pb.ControlEnvelope.PayloadCase.GET_DEVICE_INFORMATION:
        var di = buildDeviceInfo();
        res.setDeviceInformation(di);
        break;
      case accessories_pb.ControlEnvelope.PayloadCase.GET_DEVICE_CONFIGURATION:
        var dc = new device_pb.DeviceConfiguration;
        res.setDeviceConfiguration(dc);
        break;
      case accessories_pb.ControlEnvelope.PayloadCase.GET_STATE:
        var feature = obj.getGetState().getFeature();
        var isSupported = false; // (feature == 0x132); // We support A2DP
        var state = new accessories_pb.State;
        state.setFeature(feature);
        state.setBoolean(isSupported);
        res.setState(state);
        break;
    }

    var ce = new accessories_pb.ControlEnvelope();
    ce.setCommand(cmd);
    ce.setResponse(res);

    var finalVal = builders.buildControlBuffer(ce);
    return finalVal;
}

function buildDeviceInfo() {
    var di = new device_pb.DeviceInformation;
    di.setName(global.name);
    di.setDeviceType(global.deviceID);
    di.setSerialNumber(global.deviceSerial);

    di.setSupportedTransportsList([common_pb.Transport.BLUETOOTH_LOW_ENERGY]);
    return di;
}

module.exports = buildResponse;
