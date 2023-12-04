function handle_pause()
{
    var currentState = game_state.playing;
    if (currentState)
        game_state.playing = false;
    else
        game_state.playing = true;
}

handle_pause();