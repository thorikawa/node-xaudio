#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <nan.h>

using namespace v8;
using namespace std;

class XAudio: public Nan::ObjectWrap {
public:
  static NAN_MODULE_INIT(Init);

private:
  explicit XAudio();
  ~XAudio();
  static void MusicFinished();
  static void FinishedCalback(uv_async_t* handle);
  static NAN_METHOD(New);
  static NAN_METHOD(Play);
  static NAN_METHOD(Stop);
  static Nan::Persistent<v8::Function> constructor;
};

NODE_MODULE(objectwrapper, XAudio::Init);
