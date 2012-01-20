load("glycam.js");

// This is a sequence in GLYCAM condensed nomenclature.
var sequence = "DGalpNAcb1-4DGlcpNAcb1-2DManpa1-6[DGlcpNAcb1-4[DGalpNAcb1-4DGlcpNAcb1-2]DManpa1-3]DManpb1-4DGlcpNAcb1-4DGlcpNAcb1-OH";

var glycan = build_glycam(sequence);

glycan.print_topology_file("02.top");
glycan.print_coordinate_file("02.rst");

