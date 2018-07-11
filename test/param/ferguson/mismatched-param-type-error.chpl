module ParamType {
  class G1 {
    param x;
  }

  proc main() {
    var g1 = new G1(1);
    var g2 = new G1(1:uint);

    g1 = g2;
    delete g1, g2;
  }
}
