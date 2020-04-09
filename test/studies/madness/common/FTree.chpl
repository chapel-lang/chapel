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
const rootNode = (0,0);
const emptyNode = (-1,-1);

// Check if we have None.
proc isNone(x) {
    return x.size == 0;
}

class FTree {
    var order    : int;
    var coeffDom = {0..order-1};

    var indices  : domain(2*int);   // Indexed by 2-tuples of integers
    var nodes    : [indices] Coeff; // Associative Mapping: (:int, :int) => Coeff

    var zeroes   : [coeffDom] real; // Return zeroes from this() when reading
                                    // something that has not yet been set.


    proc postinit() {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");
    }


    /** Access an element in the associative domain.  If the element
        doesn't exist it will be created.
        
        We have to be really careful about creating extra boxes when we don't
        mean to.  The algorithms tend to be really sensitive to the structure
        of the tree.  Scaling coefficients are used to mark the leaves in the
        sum tree, so if you accidentally create scaling coeffs somewhere in the
        middle of the tree you will short circuit a whole subtree!
        
     */
    proc this(lvl: int, idx: int) ref {
        if !indices.contains((lvl, idx)) {
            indices += ((lvl, idx));
            nodes[(lvl, idx)] = new Coeff(coeffDom);
        }

        return nodes[(lvl, idx)].data;
    }
    proc this(lvl: int, idx: int) {
        if !indices.contains((lvl, idx)) {
            // This is a getter so it shouldn't be modifying what
            // we return, should be safe to return the zero vector.
            // FIXME: Zeroes should really be a const, but can'ti
            //        return const from a var fcn.
            return zeroes;
        }

        return nodes[(lvl, idx)].data;
    }


    proc this((lvl, idx)) ref {
        return this(lvl,idx);
    }
    proc this((lvl, idx)) {
        return this(lvl,idx);
    }


    iter path_upwards((lvl0, idx0), (lvl1, idx1)) {
        var l = idx0;
        for n in lvl1..lvl0 by -1 {
           yield ((n,l));
           l /= 2;
        }
    } 

    iter path_downwards((lvl0, idx0), (lvl1, idx1)) {
        for n in (lvl0..lvl1-1) {
          yield (n, idx1/(2**(lvl1-n)));
        }
    }

    proc on_boundary((lvl, idx)) {
        return ((idx < 0 || idx >= 2**lvl));
    }

    proc get_children((lvl, idx)) {
        return ((lvl+1, 2*idx), (lvl+1, 2*idx+1));
    }

    proc get_neighbors((lvl, idx)) {
        return ((lvl, idx-1), (lvl, idx+1));
    }

    /** Access an element in the FTree.  If it doesn't exist, 
        return None.
     */  
    proc peek(lvl: int, idx: int) ref {
        if has_coeffs((lvl, idx)) then
            return this(lvl, idx);
        else
            return None;
    }


    /** Unordered iterator over all coefficients
     */
    iter these() {
        for n in nodes do yield n.data;
    }

    /** Unordered iterator over all boxes in a particular level.
        This is not a particularly fast way to do this, but for many of the
        Madness levelwise iterators that do all levels from 0..max_level this
        will be much faster when the data is not all the way down at max_level.
        A sparse array or an array of associative arrays may be more conducive
        to this type of iteration vs. the associative domain used here.
     */ 
    iter lvl_iter(lvl: int) {
        for i in indices do
            if i(0) == lvl && indices.contains(i) then yield nodes[i].data;
    }

    iter index_iter() {
       for i in indices do
          yield i;
    }
        

    /** Return a copy of this FTree
     */
    proc copy() {
        var t = new unmanaged FTree(order);
        t.indices = indices;
        // get around restriction of matching domains for assoc arrays
        forall i in t.nodes.domain do t.nodes(i) = nodes(i);
        return t;
    }


    /** Check if there are coefficients in box (lvl, idx)
     */
    proc has_coeffs((lvl, idx)) {
        return indices.contains((lvl, idx));
    }


    /** Remove an element from the associative domain.  If the element
        does not exist, it is ignored.
     */
    proc remove((lvl,idx)) {
        if indices.contains((lvl, idx)) then indices.remove((lvl, idx));
    }
}

proc main() {
    var f = new unmanaged FTree(2);

    for (i, j) in {0..2, 0..2} do f[i, j] = (i, j);

    for (i, j) in {1..0, 1..0} do f.remove((i, j));
    
    for (i, j) in {0..1, 0..1} do f[i, j] = (-(i:real), -(j:real));
    
    for (i, j) in {2..1, 2..1} do f.remove((i, j));
    
    for (i, j) in {1..2, 1..2} do f[i, j] = (-(i:real), -(j:real));
    
    for (i, j) in {0..2, 0..2} do
        writeln("(",i,", ",j,") = ", f.peek(i, j));

    for i in 0..2 do
        for n in f.lvl_iter(i) do
            writeln(i, ": ", n);

    delete f;
}
