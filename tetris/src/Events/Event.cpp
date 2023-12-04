#include "Event.h"

Event::Event(EventType eventType, float timeStamp)
{ 
    this -> eventType = eventType;
    Variant v;
    v.type = Variant::TYPE_TIME_STAMP;
    v.timeStamp = timeStamp;
    parameters[Variant::TYPE_TIME_STAMP] = v;
}

void Event::setEventType(EventType t)
{
    this -> eventType = t;
}

void Event::addMetaData(std::string data)
{
    this -> metaData = data;
}

void Event::addTimeVariant(float timeStamp)
{
    Variant v;
    v.type = Variant::TYPE_TIME_STAMP;
    v.timeStamp = timeStamp;
    parameters[Variant::TYPE_TIME_STAMP] = v;   
}

void Event::addCharacterVariant(Character* charRef)
{
    Variant v;
    v.type = Variant::TYPE_CHAR_REF;
    v.characterRef = charRef;
    parameters[Variant::TYPE_CHAR_REF] = v;
}

void Event::addCharacterIdVariant(int characterId)
{
    Variant v;
    v.type = Variant::TYPE_CHAR_ID;
    v.characterId = characterId;
    parameters[Variant::TYPE_CHAR_ID] = v;
}

void Event::addGraphicsObjectVariant(GraphicsObject* graphicsObject)
{
    Variant v;
    v.type = Variant::TYPE_GRAPHICS_OBJ;
    v.graphicsObject = graphicsObject;
    parameters[Variant::TYPE_GRAPHICS_OBJ] = v;
}

void Event::addCollisionDirectionVariant(int collisionDir)
{
    Variant v;
    v.type = Variant::TYPE_COLLISION_DIR;
    v.collisionDirection = collisionDir;
    parameters[Variant::TYPE_COLLISION_DIR] = v;    
}

void Event::addTimelineVariant(Timeline* timelineRef)
{
    Variant v;
    v.type = Variant::TYPE_TIMELINE;
    v.timeline = timelineRef;
    parameters[Variant::TYPE_TIMELINE] = v;
}

void Event::addTicScaleVariant(double newTicScale)
{
    Variant v;
    v.type = Variant::TYPE_TIC_SCALE;
    v.ticScale = newTicScale;
    parameters[Variant::TYPE_TIC_SCALE] = v;
}

v8::Local<v8::Object> Event::exposeToV8(v8::Isolate *isolate, v8::Local<v8::Context> &context, std::string context_name)
{
    std::vector<v8helpers::ParamContainer<v8::AccessorGetterCallback, v8::AccessorSetterCallback>> v;
    v.push_back(v8helpers::ParamContainer("timestamp", getEventTimeStamp, setEventTimeStamp));

	return v8helpers::exposeToV8("event", this, v, isolate, context, context_name);
}

void Event::setEventTimeStamp(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    // no setter for event time stamps
}

void Event::getEventTimeStamp(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Local<v8::Object> self = info.Holder();
	v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(self->GetInternalField(0));
	void* ptr = wrap->Value();

    Event* obj = static_cast<Event*>(ptr);
    float t = obj -> parameters[Variant::TYPE_TIME_STAMP].timeStamp;

	// int x_val = int(currentOgPosition.x);
	info.GetReturnValue().Set(t);
}

v8::Local<v8::Object> createV8ObjectFromSharedPtr(v8::Isolate* isolate, std::shared_ptr<Event> eventPtr) {
    v8::Local<v8::ObjectTemplate> templateObj = v8::ObjectTemplate::New(isolate);
    templateObj->SetInternalFieldCount(1);
    v8::Local<v8::Object> obj = templateObj->NewInstance(isolate->GetCurrentContext()).ToLocalChecked();

    // Store the shared_ptr<Event> within the V8 object using External
    v8::Local<v8::External> external = v8::External::New(isolate, eventPtr.get());
    obj->SetInternalField(0, external);

    return obj;
}

void Event::ScriptedEventFactory(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);

	std::string context_name("default");
	if(args.Length() != 2)
	{
        std::cout << "Failed to create Event" << std::endl;
        return;
	}

    // Retrieving EventType and timeStamp
    EventType eventType = static_cast<EventType>(args[0]->Int32Value(context).FromMaybe(0)); // Get EventType from args
    float timeStamp = static_cast<float>(args[1]->NumberValue(context).FromMaybe(0)); // Get timeStamp from args

    // Creating a new Event instance
    Event* new_event = new Event(eventType, timeStamp);
    v8::Local<v8::Object> v8_obj = new_event->exposeToV8(isolate, context, context_name);
    args.GetReturnValue().Set(handle_scope.Escape(v8_obj));
}

// Expose addCharacterVariant to V8
void Event::ScriptedAddCharacterVariant(const v8::FunctionCallbackInfo<v8::Value>& args) {
    v8::Isolate *isolate = args.GetIsolate();
	v8::Local<v8::Context> context = isolate->GetCurrentContext();
	v8::EscapableHandleScope handle_scope(args.GetIsolate());
	v8::Context::Scope context_scope(context);

    if (args.Length() != 2)
    {
        std::cout << "Failed to add character variant to event" << std::endl;
        return;
    }

    // get the event args[0]
    v8::Local<v8::Object> eventObject = args[0]->ToObject(context).ToLocalChecked();
    v8::Local<v8::External> wrap = v8::Local<v8::External>::Cast(eventObject->GetInternalField(0));
    Event* eventPtr = static_cast<Event*>(wrap->Value());

    // get the character args[1]
    v8::Local<v8::External> characterExternal = v8::Local<v8::External>::Cast(args[1]);
    Character* characterPtr = static_cast<Character*>(characterExternal->Value());

    eventPtr->addCharacterVariant(characterPtr);
}
