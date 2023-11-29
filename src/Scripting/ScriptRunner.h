#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H

#include "ScriptManager.h"
#include "../GraphicsObject/GraphicsObject.h"
#include <v8/v8.h>
#include <memory>

class ScriptRunner
{
    // private:
    //     static ScriptRunner* instancePtr;
    //     ScriptRunner();
    //     std::unique_ptr<v8::Platform> platform;
    //     v8::Isolate* isolate;
    //     std::map<std::string, v8::Persistent<v8::Context>> contexts;
    //     std::shared_ptr<ScriptManager> scriptManager;

    // public:
    //     static ScriptRunner* getInstance();
    //     ~ScriptRunner();
    //     void runOnObj(std::shared_ptr<GraphicsObject> obj);

    private:
        std::unique_ptr<v8::Platform> platform;
        v8::Isolate* isolate;
        v8::Persistent<v8::Context> defaultContext;
        std::unordered_map<std::string, v8::Persistent<v8::Context>> contexts;
        v8::Local<v8::Context> default_context;
        std::shared_ptr<ScriptManager> scriptManager;

    public:
        ScriptRunner();
        ~ScriptRunner();
        void runOnObj(std::shared_ptr<GraphicsObject> obj);

};

#endif