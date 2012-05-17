function PdbFileStructure(file, mappings) {
    _initPdbFileStructure.call(this, file, mappings);
}

inherits(PdbFileStructure, Structure);

mergeInto(PdbFileStructure.prototype, _getPdbStructurePrototype);

function loadPdb(file, mappings) {
    return new PdbFileStructure(file, mappings);
}

