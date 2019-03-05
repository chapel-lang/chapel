
class C {
  var x : int;
}

class G {
  type T;
  var x : T;
}

proc main() {
  {
    var d = new C;
    var o = new owned C;
    var s = new shared C;
    var b = new borrowed C;
  }
  {
    type GI = G(int);
    var d = new GI;
    var o = new owned GI;
    var s = new shared GI;
    var b = new borrowed GI;
  }
}
