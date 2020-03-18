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

#include <napi.h>

class AudioProcessor;

class APWrapper : public Napi::ObjectWrap<APWrapper> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports); 

  APWrapper(const Napi::CallbackInfo& info); 

 private:
  Napi::Value Open(const Napi::CallbackInfo& info); 
  Napi::Value Close(const Napi::CallbackInfo& info); 
  Napi::Value GetMaxBufSize(const Napi::CallbackInfo& info); 
  Napi::Value SetBuffer(const Napi::CallbackInfo& info); 
  Napi::Value ProcessAudio(const Napi::CallbackInfo& info); 

  AudioProcessor* _actual; 

 private:
  static Napi::FunctionReference g_constructor; 
};

