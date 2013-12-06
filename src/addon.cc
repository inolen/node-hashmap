#include <node.h>
#include <v8.h>
#include <string>
#include "hashmap.h"

using namespace v8;

void Init(Handle<Object> exports) {
	HandleScope scope;

	Local<Object> keyInt32 = Object::New();
	keyInt32->Set(String::NewSymbol("int32"), HashMap<int32_t, int32_t>::Init());
	keyInt32->Set(String::NewSymbol("string"), HashMap<int32_t, std::string>::Init());

	Local<Object> keyString = Object::New();
	keyString->Set(String::NewSymbol("int32"), HashMap<std::string, int32_t>::Init());
	keyString->Set(String::NewSymbol("string"), HashMap<std::string, std::string>::Init());

	exports->Set(String::NewSymbol("int32"), keyInt32);
	exports->Set(String::NewSymbol("string"), keyString);
}

NODE_MODULE(addon, Init);
