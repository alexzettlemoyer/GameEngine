
function select_piece() {
    if (piece_selector.length == 0)
        piece_selector = ['i','i','i','i','j','j','j','j','l','l','l','l','o','o','o','o','s','s','s','s','t','t','t','t','z','z','z','z'];
    
    var randomIndex = Math.floor(Math.random() * piece_selector.length);
    var type = piece_selector.splice(randomIndex, 1)[0]; // remove a single piece

    piece_selector_value = type;
    return type;
};

var selected = select_piece();
