var glycan = build(["ROH", "4YB"]);

glycan.setTail(1, "O4");
glycan.attach("4YB");

glycan.setTail(2, "O4");
glycan.attach("VMB");

glycan.setTail(3, "O3");
glycan.attach("YMA");

glycan.setTail(4, "O2");
glycan.attach(["4YB", "0VB"]);

glycan.setTail(4, "O4");
glycan.attach("0YB");

glycan.setTail(3, "O6");
glycan.attach("2MA");

glycan.setTail(8, "O2");
glycan.attach(["4YB", "0VB"]);

glycan.printTopologyFile("glycan.top");
glycan.printRestartFile("glycan.rst");
