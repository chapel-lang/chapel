module OuterModule {
  module M {
    class Q {
      var x : int;
    }
  }

  class C {
    var x : int;
  }

  class G {
    type T;
    var x : T;
  }
  type GI = G(int);

  class CheckDefaultInit {
    var c = new C;
    var g = new GI;
  }

  class UserInit {
    var c : C;
    var g : GI;

    proc init(c = new C, g = new GI) {
      this.c = c;
      this.g = g;
    }
  }

  proc main() {
    {
      var d = new C;
      var o = new owned C;
      var s = new shared C;
      var b = new borrowed C;
    }
    {
      var d = new GI;
      var o = new owned GI;
      var s = new shared GI;
      var b = new borrowed GI;
    }
    {
      var q = new M.Q;
    }
  }
}
