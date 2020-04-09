module ParamType {
  class G1 {
    param x;
  }

  proc main() {
    var g1 = new borrowed G1(1);
    var g2 = new borrowed G1(1:uint);

    g1 = g2;
  }
}
