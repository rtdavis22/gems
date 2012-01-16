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

var results = glycan.minimize("min.in");

print("minimized energy:", results.energy);

glycan.print_topology_file("12.top");
glycan.print_coordinate_file("12.rst");


// Here's what happens under the hood:
// - topology and restart files are printed out
// - a sander process is started and fed the files
// - SANDER does its thing
// - the sander output file is parsed to get the results
// - the new coordinates are loaded into the structure
//
// You could also do this with solvated structures.


// Should I make it so you don't even need a SANDER input file, so
// you could do something like this:
// glycan.minimize({ maxcyc: 1000, ntmin: 1, ncyc: 1000 });
// Comments?
