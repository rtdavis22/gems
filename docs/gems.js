/**
 * @name Atom
 * @constructor
 * @property {string} name The atom's name.
 * @property {string} type The atom's type.
 * @property {number} charge The atom's charge.
 * @property {string} element The atom's element.
 * @property {number[]} coordinate The atom's coordinate.
 */

/**
 * A residue!
 * @name Residue
 * @constructor
 * @property {string} name The name of the residue.
 */

/**
 * A structure!
 * @name Structure
 * @constructor
 * @property {number} size The number of atoms in the structure.
 */

/**
 * Set the head atom of the structure.
 * @name Structure.prototype.set_head
 * @function
 * @param {number|string} residue The index or id of the residue.
 * @param {string} atom The atom name.
 */

/**
 * Set the tail atom of the structure.
 * @name Structure.prototype.set_tail
 * @function
 * @param {number|string} residue The index or id of the residue.
 * @param {string} atom The atom name.
 */

/**
 * Minimize the structure.
 * @name Structure.prototype.minimize
 * @function
 * @param file A SANDER input file.
 */

/**
 * Returns a solvated copy of the structure.
 * @name Structure.prototype.solvate
 * @function
 * @param solvationInfo Solvation parameters.
 * @param {string|Structure} solvationInfo.solvent
 *        The solvent structure or residue code.
 * @param {number|number[]} solvationInfo.buffer
 *        The minimum solvent buffer.
 * @param [solvationInfo.closeness=1.5]
 *        The minimum distance between the center of a solute atom and a solvent atom.
 *        Note: is that an appropriate default?
 * @param [solvationInfo.shape="cubic"]
 *        The shape of the box. Possible values: "cubic", "rectangular".
 * @returns {Structure}
 */


/**
 * A pdb structure!!
 * @name PdbStructure
 * @constructor
 * @augments Structure
 */

/**
 * Returns the residue at <tt>index</tt> in the pdb file.
 * @name PdbStructure.prototype.pdb_residues
 * @function
 * @param {int} index The pdb residue number.
 * @returns {Residue}
 */

/**
 * Load a pdb file.
 * @name load_pdb
 * @function
 * @param {string} file The pdb file.
 * @returns {PdbStructure}
 * @throws If the file isn't found.
 */
