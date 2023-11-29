#include "ScriptRunner.h"
#include "ScriptManager.h"
#include <v8/v8.h>
#include <libplatform/libplatform.h>
#include "v8helpers.h"
#include "../GraphicsObject/GraphicsObject.h"
#include <iostream>


ScriptRunner::ScriptRunner() 
{
    platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.release());
    v8::V8::InitializeICU();
    v8::V8::Initialize();
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
    isolate = v8::Isolate::New(create_params);

    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
    // Bind your global functions here...
    global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));
    // global->Set(isolate, "gameobjectfactory", v8::FunctionTemplate::New(isolate, ScriptedGameObjectFactory));
    // global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, getHandleFromScript));
    
    default_context = v8::Context::New(isolate, NULL, global);
	v8::Context::Scope default_context_scope(default_context); // enter the context

    scriptManager = std::make_shared<ScriptManager>(isolate, default_context);

        // Create a new context
    v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
    scriptManager->addContext(isolate, object_context, "object_context");
    scriptManager->addScript("modify_position", "src/Scripting/scripts/modify_position.js", "object_context");

    // defaultContext.Reset(isolate, v8::Context::New(isolate, nullptr, global));
}

ScriptRunner::~ScriptRunner()
{
    // defaultContext.Reset();
    for (auto& pair : contexts) {
        pair.second.Reset();
    }
    isolate->Dispose();
    v8::V8::Dispose();
}

void ScriptRunner::runOnObj(std::shared_ptr<GraphicsObject> obj)
{
    ContextContainer container = scriptManager -> getContextContainer("object_context");

    v8::Isolate::Scope isolate_scope(container.isolate); // must enter the virtual machine to do stuff
    v8::HandleScope handle_scope(container.isolate);
    v8::Context::Scope context_scope(container.context);

    obj -> exposeToV8(container.isolate, container.context, "object_context");

    std::cout << "Native position before: (" << obj -> getPosition().x << ", " << obj -> getPosition().y << ")"  << std::endl;
	scriptManager->runOne("modify_position", false, "object_context");
	std::cout << "Native position after: (" << obj -> getPosition().x << ", " << obj -> getPosition().y << ")"  << std::endl;
}

// ScriptRunner* ScriptRunner::instancePtr = nullptr;

// // std::unique_ptr<v8::Platform> platform;
// // v8::Isolate* isolate;
// // std::map<std::string, v8::Persistent<v8::Context>> contexts;
// // std::shared_ptr<ScriptManager> scriptManager;

// ScriptRunner* ScriptRunner::getInstance()
// {
//     if (instancePtr == nullptr)
//         { instancePtr = new ScriptRunner(); }
//     return instancePtr;
// }

// ScriptRunner::~ScriptRunner()
// {
//     isolate->Dispose();
//     v8::V8::Dispose();
//     v8::V8::ShutdownPlatform();
// }

// ScriptRunner::ScriptRunner()
// { 
//     platform = v8::platform::NewDefaultPlatform();
//     v8::V8::InitializePlatform(platform.release());
//     v8::V8::InitializeICU();
//     v8::V8::Initialize();
//     v8::Isolate::CreateParams create_params;
//     create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
//     isolate = v8::Isolate::New(create_params);

//     { // anonymous scope for managing handle scope
//         v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
//         v8::HandleScope handle_scope(isolate);

// 		// Best practice to isntall all global functions in the context ahead of time.
//         v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);
//         // Bind the global 'print' function to the C++ Print callback.
//         global->Set(isolate, "print", v8::FunctionTemplate::New(isolate, v8helpers::Print));

//         // TODO: Create new game objects
// 		// Bind the global static factory function for creating new GameObject instances
// 		// global->Set(isolate, "gameobjectfactory", v8::FunctionTemplate::New(isolate, GameObject::ScriptedGameObjectFactory));
		
//         // Bind the global static function for retrieving object handles
// 		global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
//         v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
// 		v8::Context::Scope default_context_scope(default_context); // enter the context

//         scriptManager = std::make_shared<ScriptManager>(isolate, default_context); 

// 		// Without parameters, these calls are made to the default context
//         // scriptManager->addScript("hello_world", "scripts/hello_world.js");
//         // scriptManager->addScript("perform_function", "scripts/perform_function.js");

// 		// Create a new context
// 		v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
// 		scriptManager->addContext(isolate, object_context, "object_context");

// 		// GameObject *go = new GameObject();
// 		// go->exposeToV8(isolate, object_context);

// 		// With the "object_context" parameter, these scripts are put in a
// 		// different context than the prior three scripts
// 		// scriptManager->addScript("create_object", "scripts/create_object.js", "object_context");
// 		// scriptManager->addScript("random_object", "scripts/random_object.js", "object_context");
// 		// scriptManager->addScript("random_position", "scripts/random_position.js", "object_context");

//         // obj -> exposeToV8(isolate, object_context);

// 		// scriptManager->addScript("modify_position", "src/Scripting/scripts/modify_position.js", "object_context");
       
//         // std::cout << obj -> guid << std::endl;

//         // std::cout << "Native position before: (" << obj->getPosition().x << ", " << obj->getPosition().y << ")"  << std::endl;
//         // scriptManager -> runOne("modify_position", false, "object_context");
// 	    // std::cout << "Native position after: (" << obj->getPosition().x << ", " << obj->getPosition().y << ")"  << std::endl;


// 		// Use the following 4 lines in place of the above 4 lines as the
// 		// reference if you don't plan to use multiple contexts
// 		/* sm->addScript("create_object", "scripts/create_object.js"); */
// 		/* sm->addScript("random_object", "scripts/random_object.js"); */
// 		/* sm->addScript("random_position", "scripts/random_position.js"); */
// 		/* sm->addScript("modify_position", "scripts/modify_position.js"); */
//     }
// }

// void ScriptRunner::runOnObj(std::shared_ptr<GraphicsObject> obj)
// {
//     if (!isolate || isolate->IsDead() || isolate->IsExecutionTerminating())
//         std::cout << "Isolate is inactive or terminated!" << std::endl;

//     ContextContainer contextContainer = scriptManager->getContextContainer("object_context");

//     if (contextContainer.isolate != isolate || contextContainer.context.IsEmpty())
//         std::cout << "Invalid or empty context!" << std::endl;

//     // Enter the context associated with the object
//     v8::Isolate::Scope isolate_scope(contextContainer.isolate);
//     v8::HandleScope handle_scope(contextContainer.isolate);
//     v8::Context::Scope context_scope(contextContainer.context);

//     // Expose the object to the context
//     obj->exposeToV8(contextContainer.isolate, contextContainer.context, "object_context");

// 	scriptManager->addScript("modify_position", "src/Scripting/scripts/modify_position.js", "object_context");

//     std::cout << "Native position before: (" << obj->getPosition().x << ", " << obj->getPosition().y << ")"  << std::endl;
//     scriptManager -> runOne("modify_position", false, "object_context");
//     std::cout << "Native position after: (" << obj->getPosition().x << ", " << obj->getPosition().y << ")"  << std::endl;


//     // v8::Isolate::Scope isolate_scope(isolate.get());
//     // v8::HandleScope handle_scope(isolate.get());
//     // v8::Local<v8::Context> object_context = scriptManager -> getContextContainer("object_context");
//     // v8::Context::Scope context_scope(object_context);

//     // { // anonymous scope for managing handle scope
//     //     // v8::Isolate::Scope isolate_scope(isolate); // must enter the virtual machine to do stuff
//     //     // v8::HandleScope handle_scope(isolate);

// 	// 	// // Best practice to isntall all global functions in the context ahead of time.
//         // v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate.get());
//         // // Bind the global 'print' function to the C++ Print callback.
//         // global->Set(isolate.get(), "print", v8::FunctionTemplate::New(isolate.get(), v8helpers::Print));

//         // TODO: Create new game objects
// 		// Bind the global static factory function for creating new GameObject instances
// 		// global->Set(isolate, "gameobjectfactory", v8::FunctionTemplate::New(isolate, GameObject::ScriptedGameObjectFactory));
		
//         // // Bind the global static function for retrieving object handles
// 		// global->Set(isolate, "gethandle", v8::FunctionTemplate::New(isolate, ScriptManager::getHandleFromScript));
//         // v8::Local<v8::Context> default_context =  v8::Context::New(isolate, NULL, global);
// 		// v8::Context::Scope default_context_scope(default_context); // enter the context

//         // ScriptManager *scriptManager = new ScriptManager(isolate, default_context); 

// 		// Without parameters, these calls are made to the default context
//         // scriptManager->addScript("hello_world", "scripts/hello_world.js");
//         // scriptManager->addScript("perform_function", "scripts/perform_function.js");

// 		// Create a new context
// 		// v8::Local<v8::Context> object_context = v8::Context::New(isolate, NULL, global);
// 		// scriptManager->addContext(isolate, object_context, "object_context");

// 		// GameObject *go = new GameObject();
// 		// go->exposeToV8(isolate, object_context);

// 		// With the "object_context" parameter, these scripts are put in a
// 		// different context than the prior three scripts
// 		// scriptManager->addScript("create_object", "scripts/create_object.js", "object_context");
// 		// scriptManager->addScript("random_object", "scripts/random_object.js", "object_context");
// 		// scriptManager->addScript("random_position", "scripts/random_position.js", "object_context");

//         // obj -> exposeToV8(isolate.get(), object_context);

// 		// scriptManager->addScript("modify_position", "src/Scripting/scripts/modify_position.js", "object_context");
       
//         // std::cout << obj -> guid << std::endl;

//         // std::cout << "Native position before: (" << obj->getPosition().x << ", " << obj->getPosition().y << ")"  << std::endl;
//         // scriptManager -> runOne("modify_position", false, "object_context");
// 	    // std::cout << "Native position after: (" << obj->getPosition().x << ", " << obj->getPosition().y << ")"  << std::endl;


// 		// Use the following 4 lines in place of the above 4 lines as the
// 		// reference if you don't plan to use multiple contexts
// 		/* sm->addScript("create_object", "scripts/create_object.js"); */
// 		/* sm->addScript("random_object", "scripts/random_object.js"); */
// 		/* sm->addScript("random_position", "scripts/random_position.js"); */
// 		/* sm->addScript("modify_position", "scripts/modify_position.js"); */
//     // }
// }