// We can define our own functions:

var charge = function(structure) {
    var sum = 0.0;
    for (var i = 0; i < structure.size; i++) {
        sum += structure.atoms(i).charge;
    }
    return sum.toFixed(3);
}
