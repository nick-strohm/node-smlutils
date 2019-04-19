#include <nan.h>
#include <Windows.h>

class Mod : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init) {
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
        tpl->SetClassName(Nan::New("Mod").ToLocalChecked());
        tpl->InstanceTemplate()->SetInternalFieldCount(6);

        Nan::SetPrototypeMethod(tpl, "getName", GetName);
        Nan::SetPrototypeMethod(tpl, "getDescription", GetDescription);
        Nan::SetPrototypeMethod(tpl, "getAuthor", GetAuthor);
        Nan::SetPrototypeMethod(tpl, "getVersion", GetVersion);
        Nan::SetPrototypeMethod(tpl, "hasSetup", HasSetup);

        constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
        Nan::Set(target, Nan::New("Mod").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
    }

private:
    Mod(v8::Local<v8::String> _path) {
        v8::String::Utf8Value val(_path);
        hInstance = LoadLibrary(*val);

        FARPROC proc = GetProcAddress(hInstance, "ModName");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            name = value;
        }

        proc = GetProcAddress(hInstance, "ModDescription");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            description = value;
        }

        proc = GetProcAddress(hInstance, "ModAuthors");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            author = value;
        }

        proc = GetProcAddress(hInstance, "ModVersion");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            version = value;
        }

        proc = GetProcAddress(hInstance, "setup");
        if (proc != nullptr) {
            setup = true;
        }
    }

    ~Mod() {
        FreeLibrary(hInstance);
    }

    static NAN_METHOD(New) {
        if (info.IsConstructCall()) {
            v8::Local<v8::String> value = info[0]->IsUndefined() ? Nan::EmptyString() : Nan::To<v8::String>(info[0]).ToLocalChecked();
            Mod* mod = new Mod(value);
            mod->Wrap(info.This());
            info.GetReturnValue().Set(info.This());
            return;
        }

        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New(constructor());
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }

    static NAN_METHOD(GetName) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->name;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_METHOD(GetDescription) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->description;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_METHOD(GetAuthor) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->author;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_METHOD(GetVersion) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->version;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_METHOD(HasSetup) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        bool val = mod->setup;
        v8::Local<v8::Boolean> value = Nan::New<v8::Boolean>(val);

        info.GetReturnValue().Set(value);
    }

    static inline Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> my_constructor;
        return my_constructor;
    }

    HINSTANCE hInstance;

    std::string name;
    std::string description;
    std::string author;
    std::string version;
    bool setup;
};

NODE_MODULE(smlutils, Mod::Init)