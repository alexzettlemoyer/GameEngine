function raise_event()
{
    var event = eventobjectfactory(3, timeline.timestamp);
    eventhandler_raise(eventhandler, event);
}

raise_event();
