var headMap = {
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

var tailMap = {
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

var residueMap = {
  "HIS": "HIE",
};


for (var i in headMap) {
    addHeadMapping(i, headMap[i]);
}

for (var i in tailMap) {
    addTailMapping(i, tailMap[i]);
}

for (var i in residueMap) {
    addResidueMapping(i, residueMap[i]);
}

loadParameterFile("../dat/parm99.dat");
loadPrepFile("../dat/HOH.prep");
loadLibraryFile("../dat/all_aminont94.lib");
loadLibraryFile("../dat/all_aminoct94.lib");
loadLibraryFile("../dat/all_amino94.lib");
