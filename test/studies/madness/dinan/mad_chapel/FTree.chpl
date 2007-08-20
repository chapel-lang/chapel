class FTree {
  type coeff_t = real;
  type idx_t   = 2*int;

  var  indices: domain(idx_t);
  var  nodes  : [indices] coeff_t;

  def this(lvl: int, idx: int) var {
    if !indices.member((lvl, idx)) then
      indices += ((lvl, idx));

    return nodes[(lvl, idx)];
  }
}

/*
def main() {
  var f = FTree();

  for (i, j) in [0..2, 0..2] do
    f[i, j] = i*3+j;


  for (i, j) in [0..2, 0..2] do
    writeln("(",i,", ",j,") = ", f[i, j]);
}
*/
