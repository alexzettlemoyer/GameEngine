function move(x, y) {
    var tx = gameobject2.x;
    var ty = gameobject2.y;
    gameobject2.x = tx + x;
    gameobject2.y = ty + y;

    var tOgX = gameobject2.ogX;
    var tOgY = gameobject2.ogY;
    gameobject2.ogX = tOgX + x;
    gameobject2.ogY = tOgY + y;
}

print("Running script modify_position.js on " + gameobject2.guid + ". Updating...")
move(1, 0);
print("Done.\n")

