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

#include "apWrapper.h"
#include "audioProcessor.h"
#include <stdio.h>

Napi::FunctionReference APWrapper::g_constructor;

Napi::Object APWrapper::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "APWrapper", {
    InstanceMethod("open", &APWrapper::Open),
    InstanceMethod("close", &APWrapper::Close),
    InstanceMethod("getMaxBufSize", &APWrapper::GetMaxBufSize),
    InstanceMethod("setBuffer", &APWrapper::SetBuffer),
    InstanceMethod("processAudio", &APWrapper::ProcessAudio),
  });

  g_constructor = Napi::Persistent(func);
  g_constructor.SuppressDestruct(); // VERY IMPORTANT

  exports.Set("APWrapper", func);
  return exports;
}

APWrapper::APWrapper(const Napi::CallbackInfo& info) : Napi::ObjectWrap<APWrapper>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  this->_actual = new AudioProcessor();
}


Napi::Value APWrapper::Open(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();
  if (length != 2) {
    Napi::TypeError::New(env, "Two arguments expected").ThrowAsJavaScriptException();
  }
	 
	if(!info[0].IsString()) {
    Napi::TypeError::New(env, "First argument string expected").ThrowAsJavaScriptException();
  }

  Napi::String arg0 = info[0].As<Napi::String>();
  std::string micDevice = arg0;

	if(!info[1].IsNumber()) {
    Napi::TypeError::New(env, "Second argument number expected").ThrowAsJavaScriptException();
  }

  Napi::Number arg1 = info[1].As<Napi::Number>();
  int sampFreq = arg1.Int32Value();
  int err = this->_actual->open(micDevice.c_str(), sampFreq);

  return Napi::Number::New(env, err);
}

Napi::Value APWrapper::Close(const Napi::CallbackInfo& info) {
  int err = 0;
  if (this->_actual != 0) {
    err = this->_actual->close();
    delete this->_actual;
    this->_actual = 0;
  }

  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  return Napi::Number::New(env, err);
}

Napi::Value APWrapper::GetMaxBufSize(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  double num = this->_actual->getMaxBufSize();
  return Napi::Number::New(env, num);
}

Napi::Value APWrapper::SetBuffer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  Napi::Buffer<unsigned char> buf = info[0].As<Napi::Buffer<unsigned char>>();
  unsigned char* buf1 = buf.Data();
  this->_actual->setBuffer(buf1);

  return env.Null();
}

Napi::Value APWrapper::ProcessAudio(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int val = this->_actual->processAudio();
  return Napi::Number::New(env, val);
}

/*
Napi::Value APWrapper::Add(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (  info.Length() != 1 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number toAdd = info[0].As<Napi::Number>();
  double answer = this->_actual->add(toAdd.DoubleValue());

  return Napi::Number::New(info.Env(), answer);
}
*/

