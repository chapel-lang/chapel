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
    const lvl, idx : int;
    const loc = (lvl+idx)%numLocales; 

    def this() {
        return (lvl, idx);
    }

    def path_upwards(node1: Node) {
        var l = idx;
        for n in node1.lvl..lvl by -1 {
           const node = new Node(n, l);
           yield node;
           l /= 2;
        }
    }

    def path_downwards(node1: Node) {
        for n in lvl..node1.lvl-1 {
            const node = new Node(n, node1.idx/(2**(node1.lvl-n)));
            yield node;
        }
    }

    def on_boundary() {
        return (idx < 0 || idx >= 2**lvl);
    }

    def get_children() {
        const child1 = new Node(lvl+1, 2*idx);
        const child2 = new Node(lvl+1, 2*idx+1);
        return (child1, child2);
    }

    def get_neighbors() {
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
def isNone(x) {
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
    def this(node: Node) var {
        oneAtATime$;
        if !nodes.member(node) {
            if setter {
              nodes += node;
            } else {
              // This is a getter so it shouldn't be modifying what
              // we return, should be safe to return the zero vector.
              // FIXME: Zeroes should really be a const, but can't
              //        return const from a var fcn.
              oneAtATime$ = true;
              return zeroes;
            }
        }
       
        const c => coeffs[node]; 
        oneAtATime$ = true;
        return c;
    }
  
    /** Access an element in the associative domain.  If it doesn't exist,
        return None.
     */
    def peek(node: Node) var {
        if has_coeffs(node) then
            return this(node);
        else
            return None;
    }

    /** Unordered iterator over all coefficients
     */
    def these() {
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
    def coeffs_iter(lvl: int) {
        for i in nodes do
            if i.lvl == lvl then yield coeffs[i];
    }

    /** Check if there are coefficients in box (lvl, idx)
     */
    def has_coeffs(node: Node) {
        oneAtATime$;
        const b = nodes.member(node);
        oneAtATime$ = true;
        return b;
    }

    /** Remove an element from the associative domain.  If the element
        does not exist, it is ignored.
     */
    def remove(node: Node) {
        oneAtATime$;
        if nodes.member(node) then nodes.remove(node);
        oneAtATime$ = true;
    }

    def node_iter(lvl: int) {
        for i in nodes do
            if i.lvl == lvl then yield i;
    }

    def node_iter() {
        for i in nodes do
            yield i;
    }

    def copy(t: LocTree) {
        t.nodes = nodes;
        t.coeffs = coeffs; 
    }
}

class FTree {
    const order    : int;
    const coeffDom = [0..order-1];
    
    var tree       : [LocaleSpace] LocTree;

    def initialize() {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");

        coforall loc in LocaleSpace do
            on Locales(loc) do tree[loc] = new LocTree(coeffDom);
    }

    def this(node: Node) var {
        const t = tree[node.loc];
        return t[node];
    }

    def this(loc: int) {
        for coeffs in tree[loc] do
            yield coeffs;
    }

    def these() {
        for t in tree do
            for coeffs in t do
                yield coeffs;
    }

    def coeffs_iter(lvl: int) {
        for t in tree do
            for coeffs in t.coeffs_iter(lvl) do
                yield coeffs;
    }
    
    def peek(node: Node) var {
        const t = tree[node.loc];
        return t.peek(node);
    }

    def has_coeffs(node: Node) {
        const t = tree[node.loc];
        return t.has_coeffs(node);
    }

    def remove(node: Node) {
        const t = tree[node.loc];
        t.remove(node);
    }
    
    /** Return a copy of this FTree
     */
    def copy() {
        const f = new FTree(order);
        for loc in LocaleSpace do
            tree[loc].copy(f.tree[loc]);
        return f;
    }

    def node_iter(lvl: int) {
        for t in tree do
            for i in t.node_iter(lvl) do
                yield i;
    }

    def node_iter() {
        for t in tree do
            for i in t.node_iter() do
                yield i;
    }
}

def main() {
    var f = new FTree(2);

    for (i, j) in [1..3, 2..4] {
        const node = new Node(i, j); 
        f[node] = (i, j);
    }

    for loc in LocaleSpace {
        writeln("\n\ntree on loc ", loc, " = ");
        for coeffs in f[loc] do
            writeln(coeffs);
    }
    
    var node = new Node(4, 5);
    writeln("\n\nf.has_coeffs((4, 5)) = ", f.has_coeffs(node));
    writeln("f.peek((4, 5)) = ", f.peek(node));
    writeln("f[(4, 5)] = ", f[node]);
    writeln("f.remove((4, 5))");
    f.remove(node);

    node = new Node(1, 2);
    writeln("\n\nf.has_coeffs((1, 2)) = ", f.has_coeffs(node));
    writeln("f.peek((1, 2)) = ", f.peek(node));
    writeln("f[(1, 2)] = ", f[node]);
    writeln("f.remove((1, 2))");
    f.remove(node);

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
            for i in f.node_iter(lvl) do
                writeln(i());
    }

    writeln("\n\nall tree indices = ");
    for i in f.node_iter() do
        writeln(i());
}
