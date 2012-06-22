// This works because the Glycam force field and prep files are
// automatically loaded.

var glycan = buildGlycamSequence("DGlcpNAcb1-6[DGlcpNAcb1-2]DManpa1-6[DGlcpNAcb1-4][DGlcpNAcb1-4[DGlcpNAcb1-2]DManpa1-3]DManpb1-4DGlcpNAcb1-4[LFucpa1-6]DGlcpNAcb1-OME");

glycan.printTopologyFile("glycan.top");
glycan.printRestartFile("glycan.rst");
