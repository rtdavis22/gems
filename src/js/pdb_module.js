// Copyright (c) 2012 The University of Georgia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Author: Robert Davis

(function(global) {
    global.PdbFileStructure = function(file, options) {
        _initPdbFileStructure.call(this, file, options || {});
    };

    inherits(PdbFileStructure, Structure);

    mergeInto(PdbFileStructure.prototype, _getPdbStructurePrototype());

    global.PdbFileStructure.prototype.residues = function(residue) {
        var index = -1;
        if (typeof residue === 'object') {
            index = this._getResidueIndex(residue);
        } else {
            index = residue;
        }
        return Structure.prototype.residues.call(this, index);
    };

    global.PdbFileStructure.prototype.setTail = function(residue, atom) {
        if (typeof residue === 'object') {
            residue = this._getResidueIndex(residue);
        }
        return Structure.prototype.setTail.call(this, residue, atom);
    };

    global.loadPdb = function(file, mappings) {
        return new PdbFileStructure(file, mappings);
    };

    Structure.prototype.printPdbFile = function(file, options) {
        _printPdbFile.call(this, file, options || {});
    };
}(this));
