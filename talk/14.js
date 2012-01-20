load("glycam.js");

// The first mapping is a global mapping from HIS to HIP that replaces the
// HIS -> HIE mapping in glycam.js
// The other mappings are individual mappings of single residues.
// Individual mappings take precedence over global mappings.
var protein = load_pdb("dat/1RVZ_New.pdb", [
    { from: "HIS", to: "HIP" },
    { from: { chain_id: 'A', res_num: 12 }, to: "HIE" },
    { from: { chain_id: 'A', res_num: 8 }, to: "CYX" },
    { from: { chain_id: 'B', res_num: 637 }, to: "CYX" }
]);

// All better...
protein.print_topology_file("14.top");
protein.print_coordinate_file("14.rst");

