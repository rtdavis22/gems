load("robert.js");

var glycan = buildRobertsGlycan();

var residue = glycan.residues(2);
print(residue.name);

for (var i = 0; i < residue.size; i++) {
    var atom = residue.atoms(i);
    print(atom.name, atom.type, atom.charge);
}
