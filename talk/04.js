load("glycam.js");

// For reference:
// "DGalpNAcb1-4DGlcpNAcb1-2DManpa1-6[DGlcpNAcb1-4[DGalpNAcb1-4DGlcpNAcb1-2]DManpa1-3]DManpb1-4DGlcpNAcb1-4DGlcpNAcb1-OH"

// Left bracket starts a branch, right bracket ends the branch.
var glycan = build([
  "ROH",
  "4YB",
  { at: "O4", code: "4YB" },
  { at: "O4", code: "VMB" }, [
    { at: "O3", code: "YMA" }, [
      { at: "O2", code: "4YB" },
      "0VB"
    ],
    { at: "O4", code: "0YB" }
  ],
  { at: "O6", code: "2MA" },
  { at: "O2", code: "4YB" },
  "0VB"
]);

glycan.print_topology_file("04.top");
glycan.print_coordinate_file("04.rst");
