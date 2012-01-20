load("glycam.js");

// For reference:
// DGalpNAcb1-4DGlcpNAcb1-2DManpa1-6[DGlcpNAcb1-4[DGalpNAcb1-4DGlcpNAcb1-2]DManpa1-3]DManpb1-4DGlcpNAcb1-4DGlcpNAcb1-OH

// This is the LEaP way of building stuff. It is NOT encouraged in gems.

var glycan = build(["ROH", "4YB"]);

glycan.set_tail(1, "O4");
glycan.attach("4YB");

glycan.set_tail(2, "O4");
glycan.attach("VMB");

glycan.set_tail(3, "O3");
glycan.attach("YMA");

glycan.set_tail(4, "O2");
glycan.attach(["4YB", "0VB"]);

glycan.set_tail(4, "O4");
glycan.attach("0YB");

glycan.set_tail(3, "O6");
glycan.attach("2MA");

glycan.set_tail(8, "O2");
glycan.attach(["4YB", "0VB"]);

glycan.print_topology_file("03.top");
glycan.print_coordinate_file("03.rst");
