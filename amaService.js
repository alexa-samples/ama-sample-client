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

var amaTxCharacteristic = require('./amaTx');
var amaRxCharacteristic = require('./amaRx');
var accessories_pb = require('./amaprotos/accessories_pb');
var builders = require('./buildControlBuffer');
var parseData = require("./parseData");
var buildResponse = require('./buildResponse');
var generalUtils = require('./generalUtils');
var audioSender = require('./audioSender');

let timeHandle = null;

function AMAService(ama) {
    this._tx = new amaTxCharacteristic();
    this._rx = new amaRxCharacteristic();
    this._audioSender = new audioSender();
    bleno.PrimaryService.call(this, {
        uuid: '0000FE03-0000-1000-8000-00805F9B34FB',
        characteristics: [
            this._tx,
            this._rx
        ]
    });
}

// May throw an exception
function processInput(buffer) {
    var v = parseData(buffer);
    if (v.streamId != 0) {
      throw "Only support control id";
    }
 
    var cmd = v.data.getCommand();
    
    console.log("Command: ", generalUtils.lookupCommand(cmd));

    if (cmd == accessories_pb.ControlEnvelope.PayloadCase.START_SPEECH) {
      // Now, set the interval to send voice data
      console.log("Now sending audio data...");
      timeHandle = setInterval(sendAudio, 2);
      return null;
    }

    if (v.data.getResponse() != null) {
       // this is just a response message. Let's just keep quiet.
       console.log("Ignoring response...");
       return null;
    }
    
    switch(cmd) {
      case accessories_pb.ControlEnvelope.PayloadCase.GET_DEVICE_INFORMATION:
      case accessories_pb.ControlEnvelope.PayloadCase.GET_DEVICE_CONFIGURATION:
      case accessories_pb.ControlEnvelope.PayloadCase.SYNCHRONIZE_SETTINGS:
      case accessories_pb.ControlEnvelope.PayloadCase.START_SETUP:
      case accessories_pb.ControlEnvelope.PayloadCase.COMPLETE_SETUP:
      case accessories_pb.ControlEnvelope.PayloadCase.SYNCHRONIZE_STATE:
      case accessories_pb.ControlEnvelope.PayloadCase.KEEP_ALIVE:
      case accessories_pb.ControlEnvelope.PayloadCase.GET_STATE:
        var output = buildResponse(cmd, v.data);
        return output;
      case accessories_pb.ControlEnvelope.PayloadCase.NOTIFY_SPEECH_STATE:
        var state = v.data.getNotifySpeechState().getState(); 
        console.log("Speech state: ", generalUtils.lookupSpeechState(state));
        var output = buildResponse(cmd, v.data);
        return output;
      case accessories_pb.ControlEnvelope.PayloadCase.ENDPOINT_SPEECH:
        global.mainService.stopAudio();
        var output = buildResponse(cmd, v.data);
        return output;
      default:
        console.log("Unsupported command...");
        return builders.buildUnsupportedResponse(cmd);
    }

}

AMAService.prototype.processReceivedMsg = function(buffer) {
    try {
        var output = processInput(buffer);
	if (output != null) {
          this._rx.sendData(output);
        }
    }catch(err) {
        console.log("Error processing msg: ", err);
        var output = builders.buildUnsupportedResponse(1);
        this._rx.sendData(output);
    }
    
};

AMAService.prototype.startAudio = function() {
  if (timeHandle != null) {
     return;
  }
  console.log("Starting audio...");
  var spSettings = new accessories_pb.SpeechSettings;
  spSettings.setAudioProfile(accessories_pb.AudioProfile.NEAR_FIELD);
  spSettings.setAudioFormat(accessories_pb.AudioFormat.OPUS_16KHZ_32KBPS_CBR_0_20MS);
  
  var initiator = new accessories_pb.SpeechInitiator;
  initiator.setType(accessories_pb.SpeechInitiator.Type.TAP);
  
  var dlg = new accessories_pb.Dialog;
  dlg.setId(1);
  
  var startSp = new accessories_pb.StartSpeech;
  startSp.setSettings(spSettings);
  startSp.setInitiator(initiator);
  startSp.setDialog(dlg);
  startSp.setSuppressearcon(true);
  
  var ce = new accessories_pb.ControlEnvelope();
  ce.setCommand(accessories_pb.ControlEnvelope.PayloadCase.START_SPEECH);
  ce.setStartSpeech(startSp);

  var finalVal = builders.buildControlBuffer(ce);
  this._rx.sendData(finalVal);

};

function sendAudio() {
    global.mainService._audioSender.sendAudio();
}

AMAService.prototype.stopAudio = function() {
    if (timeHandle != null) {
      console.log("Stopping audio.."); 
      clearInterval(timeHandle);
      timeHandle = null;
    }
}

AMAService.prototype.sendAudio = function(buf) {
  this._rx.sendData(buf);
}

AMAService.prototype.closeAudio = function() {
  this.stopAudio();
  this._audioSender.close();
}

util.inherits(AMAService, bleno.PrimaryService);

module.exports = AMAService;
