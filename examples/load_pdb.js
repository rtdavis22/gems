load("ff99SB.js");

var protein = loadPdb("1RVZ.pdb", {
    residueMappings: [
        { from: { chainId: 'A', resNum: 8 }, to: "CYX" },
        { from: { chainId: 'B', resNum: 637 }, to: "CYX" }
    ],
});

protein.printTopologyFile("protein.top");
protein.printRestartFile("protein.rst");
