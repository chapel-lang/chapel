module ParamType {
  class G1 {
    param x;
  }

  proc main() {
    var g1 = (new owned G1(1)).borrow();
    var g2 = (new owned G1(1:uint)).borrow();

    g1 = g2;
  }
}
