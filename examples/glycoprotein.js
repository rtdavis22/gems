load("ff99SB.js");
load("robert.js");

var protein = loadPdb("1RVZ.pdb", {
    residueMappings: [
        { from: { chainId: 'A', resNum: 8 }, to: "CYX" },
        { from: { chainId: 'B', resNum: 637 }, to: "CYX" },
        { from: { chainId: 'B', resNum: 646 }, to: "NLN" }
    ]
});

protein.setTail({ chainId: 'B', resNum: 646 }, "ND2");

var glycan = buildRobertsGlycan();
glycan.removeResidue(0);
glycan.setHead(0, "C1");

protein.attach(glycan);

protein.printTopologyFile("glycoprotein.top");
protein.printRestartFile("glycoprotein.rst");
