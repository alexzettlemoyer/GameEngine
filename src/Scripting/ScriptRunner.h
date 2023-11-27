#ifndef SCRIPT_RUNNER_H
#define SCRIPT_RUNNER_H

#include "ScriptManager.h"
#include "../GraphicsObject/GraphicsObject.h"
#include <v8/v8.h>
#include <memory>

void runOnObj(std::shared_ptr<GraphicsObject> obj);

// class ScriptRunner
// {
//     public:
//         ScriptRunner(std::shared_ptr<GraphicsObject> obj);
// };

#endif