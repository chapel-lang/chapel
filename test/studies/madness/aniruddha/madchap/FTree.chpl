/** FTree: The tree data structure used to store the numerical representation
    of a function.
    
    For 1d Madness, an FTree is a binary tree, for 2d it would
    be a quad tree, 3d an octtree, and so on.  The FTree used here uses level
    and index hashing to store the tree rather than storing connectivity
    information.  So, it is important that you do a has_coeffs() before
    indexing into the tree to be sure you don't create coeffs when you don't
    mean to.
 */

record Node {
    var lvl, idx : int;

    proc get_coords() {
        return (lvl, idx);
    }

    iter path_upwards(node1: Node) {
        var l = idx;
        for n in node1.lvl..lvl by -1 {
           const node = new Node(n, l);
           yield node;
           l /= 2;
        }
    }

    iter path_downwards(node1: Node) {
        for n in lvl..node1.lvl-1 {
            const node = new Node(n, node1.idx/(2**(node1.lvl-n)));
            yield node;
        }
    }

    proc on_boundary() {
        return (idx < 0 || idx >= 2**lvl);
    }

    proc get_children() {
        const child1 = new Node(lvl+1, 2*idx);
        const child2 = new Node(lvl+1, 2*idx+1);
        return (child1, child2);
    }

    proc get_neighbors() {
        const neighbor1 = new Node(lvl, idx-1);
        const neighbor2 = new Node(lvl, idx+1);
        return (neighbor1, neighbor2);
    }
}

const rootNode = new Node(0, 0);
const emptyNode = new Node(-1, -1);

// The empty vector, returned when there are no Coeffs.
var None: [0..-1] real;

// Check if we have None.
proc isNone(x) {
    return x.numElements == 0;
}

class LocTree {
    const coeffDom : domain(1);
    var nodes      : domain(Node);
    var coeffs     : [nodes] [coeffDom] real;

    var zeroes     : [coeffDom] real;    // Return zeroes from this() when
                                         // reading something that has not yet
                                         // been set.

    var oneAtATime$: sync bool = true;

    /** Access an element in the associative domain.  If the element
        doesn't exist it will be created.

        We have to be really careful about creating extra boxes when we don't
        mean to.  The algorithms tend to be really sensitive to the structure
        of the tree.  Scaling coefficients are used to mark the leaves in the
        sum tree, so if you accidentally create scaling coeffs somewhere in the
        middle of the tree you will short circuit a whole subtree!

     */
    proc this(node: Node) ref {
        oneAtATime$;
        if !nodes.contains(node) {
            nodes += node;
        }
       
        ref c = coeffs[node];
        oneAtATime$ = true;
        return c;
    }
    proc this(node: Node) const ref {
        oneAtATime$;
        if !nodes.contains(node) {
          // This is a getter so it shouldn't be modifying what
          // we return, should be safe to return the zero vector.
          // FIXME: Zeroes should really be a const, but can't
          //        return const from a var fcn.
          oneAtATime$ = true;
          return zeroes;
        }
       
        ref c = coeffs[node];
        oneAtATime$ = true;
        return c;
    }
 
  
    /** Access an element in the associative domain.  If it doesn't exist,
        return None.
     */
    proc peek(node: Node) ref {
        if has_coeffs(node) then
            return this(node);
        else
            return None;
    }

    /** Unordered iterator over all coefficients
     */
    iter these() {
        for c in coeffs do 
            yield c;
    }
    
    /** Unordered iterator over all boxes in a particular level.
        This is not a particularly fast way to do this, but for many of the
        Madness levelwise iterators that do all levels from 0..max_level this
        will be much faster when the data is not all the way down at max_level.
        A sparse array or an array of associative arrays may be more conducive
        to this type of iteration vs. the associative domain used here.
     */
    iter coeffs_iter(lvl: int) {
        for node in nodes do
            if node.lvl == lvl then yield coeffs[node];
    }

    /** Check if there are coefficients in box (lvl, idx)
     */
    proc has_coeffs(node: Node) {
        oneAtATime$;
        const b = nodes.contains(node);
        oneAtATime$ = true;
        return b;
    }

    /** Remove an element from the associative domain.  If the element
        does not exist, it is ignored.
     */
    proc remove(node: Node) {
        oneAtATime$;
        if nodes.contains(node) then nodes.remove(node);
        oneAtATime$ = true;
    }

    iter node_iter(lvl: int) {
        for node in nodes do
            if node.lvl == lvl then yield node;
    }

    iter node_iter() {
        for node in nodes do
            yield node;
    }

    //AGS: Provide constructor that can copy a tree instead of create-then-copy
    proc copy(t: unmanaged LocTree) {
        nodes = t.nodes;
        // get around restriction of matching domains for assoc arrays
        forall i in coeffs.domain do coeffs(i) = t.coeffs(i);
    }
}

class FTree {
    const order   : int;
    const coeffDom: domain(1);
    const tree    : [LocaleSpace] unmanaged LocTree;

    proc init(order: int) {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");

        this.order = order;
        this.coeffDom = {0..order-1};

        var tree: [LocaleSpace] unmanaged LocTree?;
        coforall loc in Locales do
            on loc do tree[loc.id] = new unmanaged LocTree(coeffDom);
        this.tree = tree!;
    }

    proc deinit() {
        coforall loc in Locales do
            on loc do delete tree[loc.id];
    }

    proc node2loc(node: Node) {
        const loc = (2**node.lvl + node.idx)%numLocales;
        return Locales(loc);
    }

    proc this(node: Node) ref {
        const t = tree[node2loc(node).id];
        return t[node];
    }

    iter this(loc: int) {
        for coeffs in tree[loc] do
            yield coeffs;
    }

    iter these() {
        for t in tree do
            for coeffs in t do
                yield coeffs;
    }

    // BLC: Really want to replace these with a non-zippered parallel
    // iterator
    iter these(param tag: iterKind) where tag == iterKind.leader {
      coforall t in tree do
        yield t;
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      for coeffs in followThis do
        yield coeffs;
    }

    iter coeffs_iter(lvl: int) {
        for t in tree do
            for coeffs in t.coeffs_iter(lvl) do
                yield coeffs;
    }
    
    proc peek(node: Node) ref {
        const t = tree[node2loc(node).id];
        return t.peek(node);
    }

    proc has_coeffs(node: Node) {
        const t = tree[node2loc(node).id];
        return t.has_coeffs(node);
    }

    proc remove(node: Node) {
        const t = tree[node2loc(node).id];
        t.remove(node);
    }
   
    //AGS: Provide constructor that can copy a tree instead of create-then-copy
    /** Return a copy of this FTree
     */
    proc copy() {
        const f = new unmanaged FTree(order);
        for loc in LocaleSpace do
            f.tree[loc].copy(tree[loc]);
        return f;
    }

    iter node_iter(lvl: int) {
        for t in tree do
            for node in t.node_iter(lvl) do
                yield node;
    }

    iter node_iter() {
        for t in tree do
            for node in t.node_iter() do
                yield node;
    }
}

proc main() {
    var f = new unmanaged FTree(2);

    for (i, j) in {1..3, 2..4} {
        const node = new unmanaged Node(i, j); 
        f[node] = (i, j);
    }

    for loc in LocaleSpace {
        writeln("\n\ntree on loc ", loc, " = ");
        for coeffs in f[loc] do
            writeln(coeffs);
    }
    
    var node = new unmanaged Node(4, 5);
    writeln("\n\nf.has_coeffs((4, 5)) = ", f.has_coeffs(node));
    writeln("f.peek((4, 5)) = ", f.peek(node));
    writeln("f[(4, 5)] = ", f[node]);
    writeln("f.remove((4, 5))");
    f.remove(node);
    delete node;

    node = new unmanaged Node(1, 2);
    writeln("\n\nf.has_coeffs((1, 2)) = ", f.has_coeffs(node));
    writeln("f.peek((1, 2)) = ", f.peek(node));
    writeln("f[(1, 2)] = ", f[node]);
    writeln("f.remove((1, 2))");
    f.remove(node);
    delete node;

    node = new Node(3, 4);
    writeln("\n\nf.remove((3, 4))");
    f.remove(node);

    for loc in LocaleSpace {
        writeln("\n\ntree on loc ", loc, " = ");
        for coeffs in f[loc] do
            writeln(coeffs);
    }

    writeln("\n\nentire tree = ");
    for coeffs in f do
        writeln(coeffs);
    delete node;

    var f1 = f.copy();

    node = new Node(3, 2);
    writeln("\n\nf.remove((3, 2))");
    f.remove(node);

    writeln("\n\nentire tree = ");
    for coeffs in f do
        writeln(coeffs);

    writeln("\n\ntree copy = ");
    for coeffs in f1 do
        writeln(coeffs);

    for lvl in 1..3 {
        writeln("\n\ncoeffs on lvl ", lvl, " = ");
        for coeffs in f.coeffs_iter(lvl) do
            writeln(coeffs);
    }

    for lvl in 1..3 {
        writeln("\n\nindices on lvl ", lvl, " = ");
            for node in f.node_iter(lvl) do
                writeln(node.get_coords());
    }

    writeln("\n\nall tree indices = ");
    for node in f.node_iter() do
        writeln(node.get_coords());

    delete node;
    delete f1;
    delete f;
}
