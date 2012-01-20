// Bringing it all together: creating a glycoprotein.

load("glycam.js");

// Load the protein from the pdb file and add a custom mapping from a
// particular ASN residue to NLN.
var protein = load_pdb("dat/1RVZ_New.pdb", [
    { from: { chain_id: 'A', res_num: 12 }, to: "HIP" },
    { from: { chain_id: 'A', res_num: 8 }, to: "CYX" },
    { from: { chain_id: 'B', res_num: 637 }, to: "CYX" },
    { from: { chain_id: 'B', res_num: 646 }, to: "NLN" }
]);

// Build the glycan from earlier.
var substructure = [ { at: "02", code: "4YB" }, "0VB" ];
var glycan = build([
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

var nln = protein.pdb_residue({ chain_id: 'B', res_num: 646 });

protein.set_tail(nln.index, "ND2");
protein.attach(glycan);

// You probably want to set some dihedrals here.

protein.print_topology_file("15.top");
protein.print_coordinate_file("15.rst");

