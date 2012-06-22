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
    global.Structure = function() {
        this.idMap = {};
    };

    global.createStructure = function() {
        var structure = new Structure();
        structure.init();
        return structure;
    };

    global.Structure.prototype = _getStructurePrototype();

    global.Structure.prototype.atoms = function(index) {
        return new Atom(this, index);
    };

    global.Structure.prototype.residues = function(index) {
        return new Residue(this, index);
    };

    global.Structure.prototype.getBondedAtoms = function(index) {
        var indices = this._getBondedAtoms(index),
            atoms = [];
        for (var i = 0; i < indices.length; i++) {
            atoms.push(this.atoms(indices[i]));
        }
        return atoms;
    };

    global.Structure.prototype.attach = function(arg) {
        if (arg instanceof Array) {
            return this._attach(build(arg));
        } else if (typeof arg === 'string') {
            return this._attach(build(arg));
        }
        return this._attach(arg);
    };

    global.Structure.prototype.residues = function(index) {
        return new Residue(this, index);
    };

    global.Structure.prototype.setDihedral = function(res1, atom1,
                                                      res2, atom2,
                                                      res3, atom3,
                                                      res4, atom4, value) {
        this._setDihedral((typeof res1 === 'string')?this.idMap[res1]:res1,
                          atom1,
                          (typeof res2 === 'string')?this.idMap[res2]:res2,
                          atom2,
                          (typeof res3 === 'string')?this.idMap[res3]:res3,
                          atom3,
                          (typeof res4 === 'string')?this.idMap[res4]:res4,
                          atom4,
                          value);
    };

    global.Structure.prototype.solvate = function(options) {
        if (typeof options.solvent === 'string') {
            options.solvent = build(options.solvent);
        }
        options.solute = this;

        var solvated = new Structure();
        solvated._initSolvatedStructure(options);
        return solvated;
    };

    global.build = function(arg) {
        var structure = new Structure();

        if (arg instanceof Array) {
            structure.init();
            structure.append(build(arg[0]));
            for (var i = 1; i < arg.length; i++) {
                structure.attach(build(arg[i]));
            }
        } else if (typeof arg === 'string') {
            structure.initFromString(arg);
        } else {

        }

        return structure;
    };
}(this));
