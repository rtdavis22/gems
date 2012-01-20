load("glycam.js");

var substructure = [ { at: "02", code: "4YB" }, "0VB" ];

// The id property is a string associated with the substructure that is used
// to uniquely identify it.
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

glycan.set_dihedral("two", "C2",
                    "two", "C1",
                    "one", "O4",
                    "one", "C4",
                    180.0);

// This has the same effect, but we don't want to count residues!
// glycan.set_dihedral(5, "C2",
//                     5, "C1",
//                     4, "O4",
//                     4, "C4",
//                     180.0);

glycan.print_topology_file("06.top");
glycan.print_coordinate_file("06.rst");
