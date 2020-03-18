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

console.log(ce.toString());



