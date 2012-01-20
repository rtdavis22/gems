load("glycam.js");

var substructure = [ { at: "02", code: "4YB" }, "0VB" ];

var glycan = build([
  "ROH",
  "4YB",
  { at: "O4", code: "4YB" },
  { id: "one", at: "O4", code: "VMB" }, [
    { id: "two", at: "O3", code: "YMA" }, [
      substructure
    ],
    { at: "O4", code: "0YB" }
  ],
  { at: "O6", code: "2MA" },
  substructure
]);


// We can query the structure:
var residue = glycan.residues("one"); // or glycan.residues(3);

print("residue name:", residue.name);

// We can assign a new name to the residue:
residue.name = "XYZ";
print("new name:", residue.name);

// XYZ is printed in the output file instead:
glycan.print_topology_file("08.top");


// We can look at this residue's atoms:
var atom = residue.atoms("C3");

print("atom name:", atom.name);
print("atom type:", atom.type);
print("atom charge:", atom.charge);
print("atom coordinate:", atom.coordinate);

// We can modify the atom:
atom.charge += 0.03;

// Or we conditionally modify it:
if (atom.charge > 10.0) {
    print("This atom has an unusually large charge. I'm adjusting it.");
    atom.charge -= 2.0;
}

// The size property of the residue is the number of atoms in it.
print("residue size:", residue.size);

// We can loop through the atoms in the residue:
for (var i = 0; i < residue.size; i++) {
    var atom = residue.atoms(i);
    // Why one would want to do this, I don't know.
    if (atom.type == "CG")
        atom.type = "C1";
}

// This won't work! (you'll get an insufficient parameter error)
// glycan.print_topology_file("wrong.top");

// We can also get bonding information.
// The bonds() function returns an array of adjacent atoms:
var adjacent = atom.bonds();

for (var i = 0; i < adjacent.length; i++) {
    var neighbor = adjacent[i];
    print("Atom", atom.name, "is adjacent to", neighbor.name);
}

// Lots of possibilites!

