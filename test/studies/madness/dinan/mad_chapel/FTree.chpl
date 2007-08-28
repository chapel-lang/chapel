record Coeff {
    var dom  : domain(1);
    var data : [dom] real;
}

class FTree {
    var order    : int;
    var coeffDom = [0..order-1];
    var idx_t    : 2*int;         // FIXME: Can't write domain(2*int)

    var indices: domain(idx_t);   // Indexed by 2-tuples of integers
    var nodes  : [indices] Coeff; // Associative Mapping: (:int, :int) => Coeff


    def initialize() {
        if order == 0 then
            halt("FTree must be initialized with an order > 0");
    }


    /** Access an element in the associative domain.  If the element
        doesn't exist it will be created.
     */
    def this(lvl: int, idx: int) var {
        if !indices.member((lvl, idx)) {
            indices += ((lvl, idx));
            nodes[(lvl, idx)] = Coeff(coeffDom);
        }

        return nodes[(lvl, idx)].data;
    }


    /** Unordered iterator over all coefficients
     */
    def these() {
      for i in indices yield i;
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
        writeln("(",i,", ",j,") = ", f[i, j]);
}
