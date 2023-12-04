function new_level()
{
    var current_line_count = game_state.line_count
    game_state.time_step = 0.1 + ( current_line_count / 10 );
}

print("Level up!");
print("old step: " + game_state.time_step);
new_level();
print("new step: " + game_state.time_step);