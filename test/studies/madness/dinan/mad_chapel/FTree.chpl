record Coeff {
  var dom  : domain(1);
  var data : [dom] real;
}

class FTree {
  var  order    : int;
  var  coeffDom = [0..order-1];
  var  idx_t    : 2*int;         // FIXME: Can't write domain(2*int)

  var  indices: domain(idx_t);   // Indexed by 2-tuples of integers
  var  nodes  : [indices] Coeff; // Associative Mapping: (:int, :int) => Coeff


  def initialize() {
    if order == 0 then
      halt("FTree must be initialized with an order > 0");
  }
  
  
  def this(lvl: int, idx: int) var {
    if !indices.member((lvl, idx)) {
      indices += ((lvl, idx));
      nodes[(lvl, idx)] = Coeff(coeffDom);
    }

    return nodes[(lvl, idx)].data;
  }

  
  def has_coeffs(lvl: int, idx: int) {
    return indices.member((lvl, idx));
  }
}


def main() {
  var f = FTree(2);
  
  for (i, j) in [0..2, 0..2] do
    f[i, j] = (i, j);


  for (i, j) in [0..2, 0..2] do
    writeln("(",i,", ",j,") = ", f[i, j]);
}
