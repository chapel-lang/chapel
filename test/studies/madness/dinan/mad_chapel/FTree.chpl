/** FTree: The tree data structure used to store the numerical representation
    of a function.
    
    For 1d Madness, an FTree is a binary tree, for 2d it would
    be a quad tree, 3d an octtree, and so on.  The FTree used here uses level
    and index hashing to store the tree rather than storing connectivity
    information.  So, it is important that you do a has_coeffs() before
    indexing into the tree to be sure you don't create coeffs when you don't
    mean to.
 */ 

// FIXME: The Coeff record works around arrays of arrays.  Ideally, nodes
//        should be a mapping from indices onto :[coeffDom] real.
record Coeff {
    var dom  : domain(1);
    var data : [dom] real;
}

// The empty vector, returned when there are no Coeffs.
var None: [0..-1] real;

// Check if we have None.
def isNone(x) {
    return x.numElements == 0;
}

class FTree {
    var order    : int;
    var coeffDom = [0..order-1];
    var idx_t    : 2*int;           // FIXME: Can't write domain(2*int)

    var indices  : domain(idx_t);   // Indexed by 2-tuples of integers
    var nodes    : [indices] Coeff; // Associative Mapping: (:int, :int) => Coeff

    var zeroes   : [coeffDom] real; // Return zeroes from this() when reading
                                    // something that has not yet been set.


    def initialize() {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");
    }


    /** Access an element in the associative domain.  If the element
        doesn't exist it will be created.
     */
    def this(lvl: int, idx: int) var {
        if !indices.member((lvl, idx)) {
            if setter {
              indices += ((lvl, idx));
              nodes[(lvl, idx)] = Coeff(coeffDom);
            } else {
              // This is a getter so it shouldn't be modifying what
              // we return, should be safe to return the zero vector.
              return zeroes;
            }
        }

        return nodes[(lvl, idx)].data;
    }


    /** Access an element in the FTree.  If it doesn't exist, 
        return None.
     */  
    def peek(lvl: int, idx: int) var {
        if has_coeffs(lvl, idx) then
            return this(lvl, idx);
        else
            return None;
    }


    /** Unordered iterator over all coefficients
     */
    def these() {
      for i in indices yield nodes[i].data;
    }


    /** Return a copy of this FTree
     */
    def copy() {
        var t = FTree(order);
        t.indices = indices;
        t.nodes   = nodes;
        return t;
    }


    /** Check if there are coefficients in box (lvl, idx)
     */
    def has_coeffs(lvl: int, idx: int) {
        return indices.member((lvl, idx));
    }


    /** Remove an element from the associative domain.  If the element
        does not exist, it is ignored.
     */
    def remove(lvl: int, idx: int) {
        if indices.member((lvl, idx)) then indices.remove((lvl, idx));
    }
}


def main() {
    var f = FTree(2);

    for (i, j) in [0..2, 0..2] do f[i, j] = (i, j);

    for (i, j) in [1..0, 1..0] do f.remove(i, j);
    
    for (i, j) in [0..1, 0..1] do f[i, j] = (-(i:real), -(j:real));
    
    for (i, j) in [2..1, 2..1] do f.remove(i, j);
    
    for (i, j) in [1..2, 1..2] do f[i, j] = (-(i:real), -(j:real));
    
    for (i, j) in [0..2, 0..2] do
        writeln("(",i,", ",j,") = ", f.peek(i, j));
}
