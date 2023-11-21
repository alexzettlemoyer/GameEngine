#ifndef SCRIPT_MANAGER
#define SCRIPT_MANAGER


#include <v8/v8.h>
#include <map>


/**
 * Container for details of scripts. 
 */
struct ScriptMetaData
{
	std::string script_id;
	std::string file_name;
	v8::Local<v8::Script> script;
};

/**
 * Container for details of v8 context objects
 */
struct ContextContainer
{
	std::string context_name;
	v8::Isolate *isolate;
	v8::Local<v8::Context> context;
	std::map<std::string, ScriptMetaData> scripts;
};

class ScriptManager
{
    private:
		/** map to keeping track of context information */
		static std::map<std::string, ContextContainer> context_containers;

    public:
    /**
     * v8 is a bit picky about parameter types...make sure to use Isolate*
     * and Context& when receiving these parameters from where they are
     * created (i.e., in main() in this example. When forwarding these
     * variables by making another function call with this class' variable,
     * use Isolate* and Context (without the &). 
     */
    ScriptManager(v8::Isolate *isolate, v8::Local<v8::Context> &context);
    ~ScriptManager();
};


#endif