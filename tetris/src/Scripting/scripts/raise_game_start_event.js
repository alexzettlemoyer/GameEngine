function raise_game_start()
{
    if (!game_state.playing)
    {
        var event = eventobjectfactory(6, timeline.timestamp);
        eventhandler_raise(eventhandler, event);
    }
}

raise_game_start();