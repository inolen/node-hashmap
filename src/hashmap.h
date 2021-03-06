#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <node.h>
#include <node_internals.h>
#include <v8.h>
#include <unordered_map>

template <typename T> bool IsA(const v8::Local<v8::Value> &arg);
template <typename T> T UnwrapValue(const v8::Local<v8::Value> &arg);
template <typename T> v8::Handle<v8::Value> WrapValue(const T &value);

template <typename K, typename V>
class HashMap : public node::ObjectWrap {
private:
	std::unordered_map<K, V> map;

	class It : public node::ObjectWrap {
	private:
		const std::unordered_map<K, V> *map;
		typename std::unordered_map<K, V>::const_iterator it;

	public:
		It(const std::unordered_map<K, V> *map) : map(map) {
			it = map->begin();
		}

		static v8::Handle<v8::Value> New(const std::unordered_map<K, V> *map) {
			v8::Local<v8::Object> obj = GetCtor()->NewInstance();
			It *it = new It(map);

			it->Wrap(obj);

			return obj;
		}

		static v8::Handle<v8::Value> HasNext(const v8::Arguments &args) {
			v8::HandleScope scope;
			It *obj = node::ObjectWrap::Unwrap<It>(args.This());

			return v8::Boolean::New(obj->it != obj->map->end());
		}

		static v8::Handle<v8::Value> Next(const v8::Arguments &args) {
			v8::HandleScope scope;
			It *obj = node::ObjectWrap::Unwrap<It>(args.This());

			v8::Handle<v8::Array> result = v8::Array::New(2);
      result->Set(0, WrapValue(obj->it->first));
      result->Set(1, WrapValue(obj->it->second));

			obj->it++;

			return scope.Close(result);
		}

		static v8::Persistent<v8::Function> GetCtor() {
			static v8::Persistent<v8::Function> persistentCtor;

			if (persistentCtor.IsEmpty()) {
				auto tpl = v8::FunctionTemplate::New();
				tpl->SetClassName(v8::String::NewSymbol("HashMapIt"));
				tpl->InstanceTemplate()->SetInternalFieldCount(1);

				auto prototype = tpl->PrototypeTemplate();
				prototype->Set("hasNext", v8::FunctionTemplate::New(HasNext)->GetFunction());
				prototype->Set("next", v8::FunctionTemplate::New(Next)->GetFunction());

				persistentCtor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
			}

			return persistentCtor;
		}
	};

	static v8::Handle<v8::Value> New(const v8::Arguments &args) {
		HashMap *obj = new HashMap();

		obj->Wrap(args.This());

		return args.This();
	}

	static v8::Handle<v8::Value> Contains(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		if (args.Length() < 1 || !IsA<K>(args[0])) {
			return node::ThrowTypeError("invalid type for argument 0");
		}

		K key = UnwrapValue<K>(args[0]);

		return scope.Close(v8::Boolean::New(obj->map.count(key) > 0));
	}

	static v8::Handle<v8::Value> Get(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		if (args.Length() < 1 || !IsA<K>(args[0])) {
			return node::ThrowTypeError("invalid type for argument 0");
		}

		K key = UnwrapValue<K>(args[0]);
		auto it = obj->map.find(key);

		if (it == obj->map.end()) {
			return scope.Close(v8::Undefined());
		}

		return scope.Close(WrapValue<V>(it->second));
	}

	static v8::Handle<v8::Value> Iterator(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		return scope.Close(It::New(&obj->map));
	}

	static v8::Handle<v8::Value> Put(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		if (args.Length() < 1 || !IsA<K>(args[0])) {
			return node::ThrowTypeError("invalid type for argument 0");
		}
		if (args.Length() < 2 || !IsA<V>(args[1])) {
			return node::ThrowTypeError("invalid type for argument 1");
		}

		K key = UnwrapValue<K>(args[0]);
		V value = UnwrapValue<V>(args[1]);

		obj->map.insert(std::make_pair(key, value));

		return scope.Close(v8::Undefined());
	}

	static v8::Handle<v8::Value> Remove(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		if (args.Length() < 1 || !IsA<K>(args[0])) {
			return node::ThrowTypeError("invalid type for argument 0");
		}

		K key = UnwrapValue<K>(args[0]);

		obj->map.erase(key);

		return scope.Close(v8::Undefined());
	}

	static v8::Handle<v8::Value> Clear(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		obj->map.clear();

		return scope.Close(v8::Undefined());
	}

	static v8::Handle<v8::Value> Empty(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		return scope.Close(v8::Boolean::New(obj->map.empty()));
	}

	static v8::Handle<v8::Value> Size(const v8::Arguments &args) {
		v8::HandleScope scope;
		HashMap *obj = node::ObjectWrap::Unwrap<HashMap>(args.This());

		return scope.Close(v8::Number::New(obj->map.size()));
	}

public:
	static v8::Persistent<v8::Value> Init() {
		v8::HandleScope scope;

		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
		tpl->SetClassName(v8::String::NewSymbol("HashMap"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		auto prototype = tpl->PrototypeTemplate();
		prototype->Set("clear", v8::FunctionTemplate::New(Clear)->GetFunction());
		prototype->Set("contains", v8::FunctionTemplate::New(Contains)->GetFunction());
		prototype->Set("empty", v8::FunctionTemplate::New(Empty)->GetFunction());
		prototype->Set("get", v8::FunctionTemplate::New(Get)->GetFunction());
		prototype->Set("iterator", v8::FunctionTemplate::New(Iterator)->GetFunction());
		prototype->Set("put", v8::FunctionTemplate::New(Put)->GetFunction());
		prototype->Set("remove", v8::FunctionTemplate::New(Remove)->GetFunction());
		prototype->Set("size", v8::FunctionTemplate::New(Size)->GetFunction());

		return v8::Persistent<v8::Function>::New(tpl->GetFunction());
	}
};

// int32_t specialization
template <> 
bool IsA<int32_t>(const v8::Local<v8::Value> &arg) {
	return arg->IsInt32();
}

template <> 
int32_t UnwrapValue<int32_t>(const v8::Local<v8::Value> &arg) {
	return arg->Int32Value();
}

template <> 
v8::Handle<v8::Value> WrapValue<int32_t>(const int32_t &value) {
	return v8::Integer::New(value);
}

// std::string specialization
template <> 
bool IsA<std::string>(const v8::Local<v8::Value> &arg) {
	return arg->IsString();
}

template <>
std::string UnwrapValue<std::string>(const v8::Local<v8::Value> &arg) {
	v8::Local<v8::String> local = v8::Local<v8::String>::Cast(arg);
	v8::String::Utf8Value utf(local);
	return *utf;
}

template <>
v8::Handle<v8::Value> WrapValue<std::string>(const std::string &value) {
	return v8::String::New(value.c_str());
}

#endif
