#include <SFML/System/Clock.hpp>
#include <cstddef>
#include <mutex>
#include <chrono>
#include "Timeline.h"
#include <iostream>

std::mutex timeMutex;

Timeline::Timeline()
{
    paused = false;
    ticSize = 1.0;
    startTime = std::chrono::system_clock::now();
    lastTime = std::chrono::system_clock::now();
}

void Timeline::updateDeltaTime()
{
    std::lock_guard<std::mutex> lock(timeMutex);
    if ( anchor == NULL )
    {
        std::chrono::time_point<std::chrono::system_clock> currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> duration = currentTime - lastTime;

        lastTime = currentTime;
        dt = duration.count();
    }
    else
        anchor -> updateDeltaTime();
}

float Timeline::getTimeStamp()
{
    std::chrono::duration<float> elapsed = std::chrono::system_clock::now() - startTime;
    return elapsed.count();
}

// return the delta time in seconds
float Timeline::getDt()
{
    if (anchor != NULL)
        return anchor -> getDt();

    return dt;
}

float Timeline::getTicSize()
{
    // if the game is paused, the time scale is 0.0
    if (paused)
        return 0.0;
        
    return ticSize;
}

void Timeline::editTicSize(float s)
{
    std::lock_guard<std::mutex> lock(timeMutex);
    ticSize = s;
}

bool Timeline::isPaused()
{
    return paused;
}

/**
 * pauses or unpauses the timeline
 */
void Timeline::pause()
{
    std::lock_guard<std::mutex> lock(timeMutex);
    if (paused)
        paused = false;
    else
        paused = true;
}

void Timeline::setAnchor(Timeline* anchor)
{
    this -> anchor = anchor;
}

v8::Local<v8::Object> Timeline::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("timestamp", getTimelineTimeStamp, setTimelineTimeStamp));

	return v8helpers::exposeToV8("timeline", this, v, isolate, context, context_name);
}


void Timeline::getTimelineTimeStamp(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Timeline* t = static_cast<Timeline*>(ptr);
    
    float time_stamp = t->getTimeStamp();
	info.GetReturnValue().Set(time_stamp);
}

void Timeline::setTimelineTimeStamp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{ // no timestamp setter function
}
