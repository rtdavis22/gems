load("glycam.js");

// You can also use ids with LEaP-style building.

var glycan = build(["ROH", "4YB"]);

var substructure = ["4YB", "0VB"];

glycan.set_tail(1, "O4");
glycan.attach("4YB");

glycan.set_tail(2, "O4");
glycan.attach({ id: "one", code: "VMB" });

glycan.set_tail("one", "O3");
glycan.attach({ id: "two", code: "YMA" });

glycan.set_tail("two", "O2");
glycan.attach(substructure);

glycan.set_tail("two", "O4");
glycan.attach("0YB");

glycan.set_tail(3, "O6");
glycan.attach("2MA");

glycan.set_tail(8, "O2");
glycan.attach(substructure);

glycan.set_dihedral("two", "C2",
                    "two", "C1",
                    "one", "O4",
                    "one", "C4",
                    180.0);

glycan.print_topology_file("07.top");
glycan.print_coordinate_file("07.rst");

