function raise_event()
{
    var event = eventobjectfactory(11, timeline.timestamp);
    addcharactervariant(event, characterPtr);
    eventhandler_raise(eventhandler, event);
}

print("Running script raise_triple_up_event.js. Updating...")
raise_event();
print("Done!\n")



