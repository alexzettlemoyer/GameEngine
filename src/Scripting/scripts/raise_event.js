function raise_event()
{
    // object (11, timeline.timestamp)  11 = enum value for TRIPLE_UP

    // std::shared_ptr<Event> e = std::make_shared<Event>(Event::C_UP, t -> getTimeStamp()); 
    // e -> addCharacterVariant(ClientGameState::getInstance() -> getCharacter().get());
    // e -> addMetaData("Client from input up");
    // eventHandler -> onEvent(e);
}

var event = eventobjectfactory(11, timeline.timestamp);
addcharactervariant(event, character);

print("Added event of type TRIPlE_UP with character variant.");

eventhandler_raise(eventhandler, event);

// access character pointer using character