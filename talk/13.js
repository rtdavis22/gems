load("glycam.js");

// This is the same as the LEaP command. It uses the mappings in the
// configuration file to fill in missing atoms and set atom types and charges.
var protein1 = load_pdb("dat/1RVZ_New.pdb");

// I can use the protein just like any other structure:
// protein.set_dihedral(...);


// What if I want to specify additional mappings for this particular pdb file?
// The second argument is optional. It is an array of "custom" mappings.

// This overrides the HIS -> HIE mapping in the configuration file for this
// protein.
var protein2 = load_pdb("dat/1RVZ_New.pdb", [
    { from: "HIS", to: "HIP" }
]);


// We can find a residue in the structure based on the chain_id, residue number,
// and insertion code in the pdb. chain_id and i_code are optional.
var residue = protein2.pdb_residue({ chain_id: 'A', res_num: 7 });


print("residue name:", residue.name);

for (var i = 0; i < residue.size; i++) {
    print("atom: ", residue.atoms(i).name);
}

print("residue name:", residue.name);

// This results in an error. Let's see why...
protein2.print_topology_file("13.top");
protein2.print_coordinate_file("13.rst");




