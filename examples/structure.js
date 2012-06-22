load("robert.js");

var glycan = buildRobertsGlycan();

for (var i = 0; i < glycan.residueCount; i++) {
    var residue = glycan.residues(i);
    print("residue:", residue.name);
    for (var j = 0; j < residue.size; j++) {
        var atom = residue.atoms(j);
        print(" > ", atom.name, atom.type, atom.charge);
    }
}
