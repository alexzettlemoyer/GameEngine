#include "ScriptManager.h"
#include <map>
// #include "v8helpers.h"

/** Definition of static container */
std::map<std::string, ContextContainer> ScriptManager::context_containers;

ScriptManager::ScriptManager(v8::Isolate *isolate, v8::Local<v8::Context> &context)
{
	if(context_containers.empty())
	{
		std::map<std::string, ScriptMetaData> scripts;
		ContextContainer context_container{ "default", isolate, context, scripts};
		context_containers.emplace(std::make_pair("default", context_container));
	}
}