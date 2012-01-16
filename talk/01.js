add_path("../../gmml/localdat/");

load_parameter_file("param_files/parm99.dat.mod");
load_parameter_file("param_files/Glycam_06g.dat");

load_prep_file("prep_files/Glycam_06.prep");
load_prep_file("prep_files/Neu5Gc_a_06.prep");
load_prep_file("prep_files/sulfate.prep");

var glycan = build(["ROH", "3GA", "0MA"]);

glycan.print_topology_file("01.top");
glycan.print_coordinate_file("01.rst");
