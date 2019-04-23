#include <nan.h>
#include <Windows.h>

class Mod : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init) {
        v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
        tpl->SetClassName(Nan::New("Mod").ToLocalChecked());
        v8::Local<v8::ObjectTemplate> itpl = tpl->InstanceTemplate();
        itpl->SetInternalFieldCount(6);

        Nan::SetPrototypeMethod(tpl, "reload", Reload);
        Nan::SetAccessor(itpl, Nan::New("path").ToLocalChecked(), GetPath);
        Nan::SetAccessor(itpl, Nan::New("name").ToLocalChecked(), GetName);
        Nan::SetAccessor(itpl, Nan::New("description").ToLocalChecked(), GetDescription);
        Nan::SetAccessor(itpl, Nan::New("author").ToLocalChecked(), GetAuthor);
        Nan::SetAccessor(itpl, Nan::New("version").ToLocalChecked(), GetVersion);
        Nan::SetAccessor(itpl, Nan::New("hasSetup").ToLocalChecked(), HasSetup);

        constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
        Nan::Set(target, Nan::New("Mod").ToLocalChecked(), Nan::GetFunction(tpl).ToLocalChecked());
    }

private:
    Mod(v8::Local<v8::String> _path) {
        v8::String::Utf8Value val(_path);
        path = *val;
        HINSTANCE hInstance = LoadLibrary(path.c_str());

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

    static NAN_METHOD(Reload) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        HINSTANCE hInstance = LoadLibrary(mod->path.c_str());

        FARPROC proc = GetProcAddress(hInstance, "ModName");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            mod->name = value;
        }

        proc = GetProcAddress(hInstance, "ModDescription");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            mod->description = value;
        }

        proc = GetProcAddress(hInstance, "ModAuthors");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            mod->author = value;
        }

        proc = GetProcAddress(hInstance, "ModVersion");
        if (proc != nullptr) {
            std::string value = *(std::string*)proc;
            mod->version = value;
        }

        proc = GetProcAddress(hInstance, "setup");
        if (proc != nullptr) {
            mod->setup = true;
        }

        FreeLibrary(hInstance);
    }

    static NAN_GETTER(GetPath) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->path;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_GETTER(GetName) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->name;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_GETTER(GetDescription) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->description;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_GETTER(GetAuthor) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->author;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_GETTER(GetVersion) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        std::string val = mod->version;
        v8::Local<v8::String> value = Nan::New<v8::String>(val.c_str()).ToLocalChecked();
        
        info.GetReturnValue().Set(value);
    }

    static NAN_GETTER(HasSetup) {
        Mod* mod = Nan::ObjectWrap::Unwrap<Mod>(info.Holder());
        bool val = mod->setup;
        v8::Local<v8::Boolean> value = Nan::New<v8::Boolean>(val);

        info.GetReturnValue().Set(value);
    }

    static inline Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> my_constructor;
        return my_constructor;
    }

    std::string path;
    std::string name;
    std::string description;
    std::string author;
    std::string version;
    bool setup;
};

NODE_MODULE(smlutils, Mod::Init)