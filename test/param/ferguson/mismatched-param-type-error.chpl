module ParamType {
  class G1 {
    param x;
  }
  proc main() {
    var g1Obj = new G1(1);
    var g2Obj = new G1(1:uint);
    var g1 = g1Obj.borrow();
    var g2 = g2Obj.borrow();
    g1 = g2;
  }
}
