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

// This syntax is changing very soon to use object properties for arguments.
var solvated = glycan.solvate("TIP3PBOX", 10.0, 1.0);

// Works with anything:
// var solvated = glycan.solvate("0GA", 10.0, 1.0);

solvated.print_topology_file("11.top");
solvated.print_coordinate_file("11.rst");
