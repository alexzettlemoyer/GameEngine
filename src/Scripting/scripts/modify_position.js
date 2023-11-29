function move(x, y) {
    var tx = gameobject2.x;
    var ty = gameobject2.y;
    gameobject2.x = tx + x;
    gameobject2.y = ty + y;
}

print("Starting position for " << gameobject2.guid << ": " + gameobject2.x + ", " + gameobject2.y + "\nUpdating...");

move(200,-5);
print("Ending position for " << gameobject2.guid << ": " + gameobject2.x + ", " + gameobject2.y + "\nDone...");
