function loadJs(file) {
    load(_data_path + "/src/js/" + file);
}

function getFullDataFilePath(file) {
    return _data_path + "/dat/" + file;
}

loadJs("utils.js");
loadJs("structure.js");
loadJs("pdb.js");
loadJs("atom.js");
loadJs("environment.js");
loadJs("glycam.js");
