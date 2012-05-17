loadPrepFile(getFullDataFilePath("Glycam_06.prep"));

function buildGlycamSequence(sequence) {
    var structure = new Structure();
    _setFromGlycamSequence.call(structure, sequence);
    return structure;
}
