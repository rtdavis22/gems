load("robert.js");

var glycan = buildRobertsGlycan();

glycan.printPdbFile("glycan.pdb", {
    includeHydrogens: false
});
