#ifndef TIMELINE_H
#define TIMELINE_H

#include <atomic>
#include <chrono>
#include <v8/v8.h>
#include "../Scripting/v8helpers.h"

class Timeline
{
    private:
        float dt;
        float ticSize;
        sf::Clock dt_clock;
        std::atomic<bool> paused;
        Timeline* anchor;
        std::chrono::time_point<std::chrono::system_clock> startTime;
        std::chrono::time_point<std::chrono::system_clock> lastTime;
        
        static void getTimelineTimeStamp(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
        static void setTimelineTimeStamp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    public:
        Timeline();
        static constexpr double SCALE_HALF = 0.5;
        static constexpr double SCALE_REAL = 1.0;
        static constexpr double SCALE_DOUBLE = 2.0;
        static Timeline *getInstance();
        float getTimeStamp();
        void pause();
        bool isPaused();
        float getDt();
        float getTicSize();
        void editTicSize(float s);
        void updateDeltaTime();
        void setAnchor(Timeline* anchor);

        v8::Local<v8::Object> exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name="default");
};

#endif