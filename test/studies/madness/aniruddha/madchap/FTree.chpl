/** FTree: The tree data structure used to store the numerical representation
    of a function.
    
    For 1d Madness, an FTree is a binary tree, for 2d it would
    be a quad tree, 3d an octtree, and so on.  The FTree used here uses level
    and index hashing to store the tree rather than storing connectivity
    information.  So, it is important that you do a has_coeffs() before
    indexing into the tree to be sure you don't create coeffs when you don't
    mean to.
 */
config const numLocs = 3;

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

record LocTree {
    const coeffDom : domain(1);
    var idx_t      : 2*int;
    var locIndices : domain(idx_t.type); // Indexed by 2-tuples of integers
    var locNodes   : [locIndices] Coeff; // Associative Mapping: (:int, :int)
                                         // => Coeff

    var zeroes     : [coeffDom] real;    // Return zeroes from this() when
                                         // reading something that has not yet
                                         // been set.

    /** Access an element in the associative domain.  If the element
        doesn't exist it will be created.

        We have to be really careful about creating extra boxes when we don't
        mean to.  The algorithms tend to be really sensitive to the structure
        of the tree.  Scaling coefficients are used to mark the leaves in the
        sum tree, so if you accidentally create scaling coeffs somewhere in the
        middle of the tree you will short circuit a whole subtree!

     */
    def this(lvl: int, idx: int) var {
        if !locIndices.member((lvl, idx)) {
            if setter {
              locIndices += ((lvl, idx));
              locNodes[(lvl, idx)] = Coeff(coeffDom);
            } else {
              // This is a getter so it shouldn't be modifying what
              // we return, should be safe to return the zero vector.
              // FIXME: Zeroes should really be a const, but can'ti
              //        return const from a var fcn.
              return zeroes;
            }
        }

        return locNodes[(lvl, idx)].data;
    }
    
    /** Access an element in the associative domain.  If it doesn't exist,
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
        for n in locNodes yield n.data;
    }
    
    /** Unordered iterator over all boxes in a particular level.
        This is not a particularly fast way to do this, but for many of the
        Madness levelwise iterators that do all levels from 0..max_level this
        will be much faster when the data is not all the way down at max_level.
        A sparse array or an array of associative arrays may be more conducive
        to this type of iteration vs. the associative domain used here.
     */
    def lvl_iter(lvl: int) {
        // AGS - Why are we doing indices.member(i)?
        for i in locIndices do
            if i(1) == lvl && locIndices.member(i) then yield locNodes[i].data;
    }

    /** Check if there are coefficients in box (lvl, idx)
     */
    def has_coeffs(lvl: int, idx: int) {
        return locIndices.member((lvl, idx));
    }

    /** Remove an element from the associative domain.  If the element
        does not exist, it is ignored.
     */
    def remove(lvl: int, idx: int) {
        if locIndices.member((lvl, idx)) then locIndices.remove((lvl, idx));
    }

    // AGS - new function to compile test_showboxes
    def idx_iter(lvl: int) {
        for idx in locIndices do
            if idx(1) == lvl then yield idx;
    }

    // AGS - new function to compile MB's gaxpy
    def idx_iter() {
        for idx in locIndices do
            yield idx;
    }
}

class FTree {
    const order    : int;
    const coeffDom = [0..order-1];
    
    const locDom = [0..numLocs);
    var nodes    : [locDom] LocTree;

    def initialize() {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");

        [loc in locDom] nodes[loc] = LocTree(coeffDom);
    }

    def mapNodeToLoc(lvl, idx) {
        return (lvl + idx) % numLocs; 
    }
    
    def this(lvl: int, idx: int) var {
        const n => nodes[mapNodeToLoc(lvl, idx)];
        return n[lvl, idx];
    }

    def this(loc: int) {
        for data in nodes[loc] do
            yield data;
    }

    def these() {
        for n in nodes do
            for data in n do
                yield data;
    }

    def lvl_iter(lvl: int) {
        for n in nodes do
            for data in n.lvl_iter(lvl) do
                yield data;
    }
    
    def peek(lvl: int, idx: int) var {
        const n => nodes[mapNodeToLoc(lvl, idx)];
        return n.peek(lvl, idx);
    }

    def has_coeffs(lvl: int, idx: int) {
        const n => nodes[mapNodeToLoc(lvl, idx)];
        return n.has_coeffs(lvl, idx);
    }

    def remove(lvl: int, idx: int) {
        const n => nodes[mapNodeToLoc(lvl, idx)];
        n.remove(lvl, idx);
    }
    
    /** Return a copy of this FTree
     */
    def copy() {
        var t = FTree(order);
        t.nodes = nodes;
        return t;
    }

    // AGS - new function to compile test_showboxes
    def idx_iter(lvl: int) {
        for n in nodes do
            for idx in n.idx_iter(lvl) do
                yield idx;
    }

    // AGS - new function to compile MB's gaxpy
    def idx_iter() {
        for n in nodes do
            for idx in n.idx_iter() do
                yield idx;
    }
}

def main() {
    /*
    var f = FTree(2);

    for (i, j) in [0..2, 0..2] do f[i, j] = (i, j);

    for (i, j) in [1..0, 1..0] do f.remove(i, j);
    
    for (i, j) in [0..1, 0..1] do f[i, j] = (-(i:real), -(j:real));
    
    for (i, j) in [2..1, 2..1] do f.remove(i, j);
    
    for (i, j) in [1..2, 1..2] do f[i, j] = (-(i:real), -(j:real));
    
    for (i, j) in [0..2, 0..2] do
        writeln("(",i,", ",j,") = ", f.peek(i, j));

    for i in 0..2 do
        for n in f.lvl_iter(i) do
            writeln(i, ": ", n);
    */

    var f = FTree(2);
    
    for (i, j) in [1..3, 2..4] do f[i, j] = (i, j);
  
    for loc in 0..numLocs-1 {
        writeln("\n\ntree on loc ", loc, " = ");
        for data in f[loc] do
            writeln(data);
    }

    writeln("\n\nf.has_coeffs(4, 5) = ", f.has_coeffs(4, 5));
    writeln("f.peek(4, 5) = ", f.peek(4, 5));
    writeln("f[4, 5] = ", f[4, 5]);
    writeln("f.remove(4, 5)"); 
    f.remove(4, 5);
    
    writeln("\n\nf.has_coeffs(1, 2) = ", f.has_coeffs(1, 2));
    writeln("f.peek(1, 2) = ", f.peek(1, 2));
    writeln("f[1, 2] = ", f[1, 2]);
    writeln("f.remove(1, 2)"); 
    f.remove(1, 2);

    writeln("\n\nf.remove(3, 4)"); 
    f.remove(3, 4);

    for loc in 0..numLocs-1 {
        writeln("\n\ntree on loc ", loc, " = ");
        for data in f[loc] do
            writeln(data);
    }
    
    writeln("\n\nentire tree = ");
    for data in f do
        writeln(data);

    //writeln("norm2 of tree = ",sqrt(+ reduce [data in f] normf(data)**2));
        
    writeln("\n\nentire tree = ");
    writeln(f);

    var f1 = f.copy();

    writeln("\n\nf.remove(3, 2)"); 
    f.remove(3, 2);

    writeln("\n\nentire tree = ");
    writeln(f);

    writeln("\n\ntree copy = ");
    writeln(f1);

    for lvl in 1..3 {
        writeln("\n\ndata on lvl ", lvl, " = ");
        for data in f.lvl_iter(lvl) do
            writeln(data);
    }

    for lvl in 1..3 {
        writeln("\n\nindices on lvl ", lvl, " = ");
            for idx in f.idx_iter(lvl) do
                writeln(idx);
    }
    
    writeln("\n\nall tree indices = ");
    for (n, l) in f.idx_iter() do
        writeln(n,l);
}
