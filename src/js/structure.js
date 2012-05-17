// Structure module
function Structure() {
}

Structure.prototype = _getStructurePrototype();

Structure.prototype.atoms = function(index) {
    return new Atom(this, index);
}

