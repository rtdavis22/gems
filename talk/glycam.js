HeadMap = {
  "ALA": "NALA",
  "ARG": "NARG",
  "ASN": "NASN",
  "ASP": "NASP",
  "CYS": "NCYS",
  "CYX": "NCYX",
  "GLN": "NGLN",
  "GLU": "NGLU",
  "GLY": "NGLY",
  "HID": "NHID",
  "HIE": "NHIE",
  "HIP": "NHIP",
  "ILE": "NILE",
  "LEU": "NLEU",
  "LYS": "NLYS",
  "MET": "NMET",
  "PHE": "NPHE",
  "PRO": "NPRO",
  "SER": "NSER",
  "THR": "NTHR",
  "TRP": "NTRP",
  "TYR": "NTYR",
  "VAL": "NVAL",
  "HIS": "NHIS",
  "GUA": "DG5",
  "ADE": "DA5",
  "CYT": "DC5",
  "THY": "DT5",
  "G": "RG5",
  "A": "RA5",
  "C": "RC5",
  "U": "RU5",
  "DG": "DG5",
  "DA": "DA5",
  "DC": "DC5",
  "DT": "DT5"
}

TailMap = {
  "ALA": "CALA",
  "ARG": "CARG",
  "ASN": "CASN",
  "ASP": "CASP",
  "CYS": "CCYS",
  "CYX": "CCYX",
  "GLN": "CGLN",
  "GLU": "CGLU",
  "GLY": "CGLY",
  "HID": "CHID",
  "HIE": "CHIE",
  "HIP": "CHIP",
  "ILE": "CILE",
  "LEU": "CLEU",
  "LYS": "CLYS",
  "MET": "CMET",
  "PHE": "CPHE",
  "PRO": "CPRO",
  "SER": "CSER",
  "THR": "CTHR",
  "TRP": "CTRP",
  "TYR": "CTYR",
  "VAL": "CVAL",
  "HIS": "CHIS",
  "GUA": "DG3",
  "ADE": "DA3",
  "CYT": "DC3",
  "THY": "DT3",
  "G": "RG3",
  "A": "RA3",
  "C": "RC3",
  "U": "RU3",
  "DG": "DG3",
  "DA": "DA3",
  "DC": "DC3",
  "DT": "DT3"
}

ResidueMap = {
  "HIS": "HIE",
};


for (var i in HeadMap) {
    add_head_mapping(i, HeadMap[i]);
}

for (var i in TailMap) {
    add_tail_mapping(i, TailMap[i]);
}

for (var i in ResidueMap) {
    add_residue_mapping(i, ResidueMap[i]);
}

add_path("dat/");

load_parameter_file("parm99.dat");
load_parameter_file("Glycam_06h.dat");

load_prep_file("Glycam_06.prep");
load_prep_file("Neu5Gc_a_06.prep");
load_prep_file("sulfate.prep");
load_prep_file("HOH.prep");

load_library_file("all_amino94.lib");
load_library_file("all_aminoct94.lib");
load_library_file("all_aminont94.lib");
load_library_file("tip3pbox.off");
