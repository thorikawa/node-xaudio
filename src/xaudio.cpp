/*********************************************************************
 * NAN - Native Abstractions for Node.js
 *
 * Copyright (c) 2016 NAN contributors
 *
 * MIT License <https://github.com/nodejs/nan/blob/master/LICENSE.md>
 ********************************************************************/

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <nan.h>
#include "xaudio.hpp"

using namespace v8;

Nan::Callback *playDoneCallback = NULL;
Mix_Music *music;
uv_async_t async; // keep this instance around for as long as we might need to do the periodic callback

NAN_MODULE_INIT(XAudio::Init) {
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("XAudio").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  SetPrototypeMethod(tpl, "play", Play);
  SetPrototypeMethod(tpl, "stop", Stop);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
  Nan::Set(target, Nan::New("XAudio").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
}

XAudio::XAudio() {

}

XAudio::~XAudio() {

}

void XAudio::MusicFinished() {
  // Since SDL's callback is not on the thread of node.js event loop and it is not allowed to access
  // V8 data structure outside of the node.js event loop, we need to send notification to uv async which 
  // is initialized in the event loop to execute callback method.
  uv_async_send(&async);
}

void XAudio::FinishedCalback(uv_async_t* handle) {
  Nan::HandleScope scope;
  if (playDoneCallback != NULL) {
    playDoneCallback->Call(0, 0);
  }
}

NAN_METHOD(XAudio::New) {

  uv_loop_t* loop = uv_default_loop();
  uv_async_init(loop, &async, XAudio::FinishedCalback);

  if (info.IsConstructCall()) {
    auto frequency = info[0]->IsUndefined() ? MIX_DEFAULT_FREQUENCY : Nan::To<int>(info[0]).FromJust();
    auto format = info[1]->IsUndefined() ? MIX_DEFAULT_FORMAT : Nan::To<int>(info[1]).FromJust();
    auto channels = info[2]->IsUndefined() ? 2 : Nan::To<int>(info[2]).FromJust();
    auto chunksize = info[3]->IsUndefined() ? 4096 : Nan::To<int>(info[3]).FromJust();
    if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
      SDL_Quit();
      return Nan::ThrowError(Mix_GetError());
    }

    // XAudio *obj = new XAudio(value);
    // obj->Wrap(info.This());
    // info.GetReturnValue().Set(info.This());

    Mix_HookMusicFinished(XAudio::MusicFinished);

  } else {
    const int argc = 1;
    v8::Local<v8::Value> argv[argc] = {info[0]};
    v8::Local<v8::Function> cons = Nan::New(constructor);
    info.GetReturnValue().Set(cons->NewInstance(argc, argv));
  }
}

NAN_METHOD(XAudio::Play) {
  Nan::Utf8String filePath(info[0]);
  auto func = info[1].As<Function>();
  if (info[1]->IsFunction()) {
    playDoneCallback = new Nan::Callback(info[1].As<Function>());
  }
  music = Mix_LoadMUS(*filePath);
  if (!music) {
    return Nan::ThrowError(Mix_GetError());
  }
  Mix_VolumeMusic(64);
  if (Mix_PlayMusic(music, 1) == -1) {
    return Nan::ThrowError(Mix_GetError());
  }
}

NAN_METHOD(XAudio::Stop) {
  Mix_HaltMusic();
  Mix_FreeMusic(music);
}

Nan::Persistent<v8::Function> XAudio::constructor;
