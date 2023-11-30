function edit_gravity()
{
    var current_gravity = character.gravity;
    if ( current_gravity === 0 )
    {
        character.gravity = 700;
        character.acceleration = -2500;
    }
    else
    {
        character.gravity = 0;
        character.acceleration = -1500;
    }
}

print("Running script handle_triple_up_event.js. Updating...")
edit_gravity();
print("Updated character gravity. Done!\n")