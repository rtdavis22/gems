load("robert.js");

loadLibraryFile("tip3pbox.off");

var glycan = buildRobertsGlycan();

var solvated = glycan.solvate({
    solvent: "TIP3PBOX",
    buffer: 10.0,
    closeness: 1.0,
    shape: "rectangular"
});

solvated.printTopologyFile("solvated.top");
solvated.printRestartFile("solvated.rst");
