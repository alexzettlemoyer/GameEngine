function handle_rotation()
{
    var originalRotation = piece.rotation;
    piece.rotation = (originalRotation + 1) % 4;
}

handle_rotation();


// 0 + 1 % 4 = 1
// 1 + 1 % 4 = 2
// 2 + 1 % 4 = 3
// 3 + 1 % 4 = 0