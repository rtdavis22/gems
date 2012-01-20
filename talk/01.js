add_path("dat/");

load_parameter_file("parm99.dat");
load_parameter_file("Glycam_06h.dat");

load_prep_file("Glycam_06.prep");
load_prep_file("Neu5Gc_a_06.prep");
load_prep_file("sulfate.prep");

// This is analogous to LEaP's sequence command.
var glycan = build(["ROH", "3GA", "0MA"]);

glycan.print_topology_file("01.top");
glycan.print_coordinate_file("01.rst");
