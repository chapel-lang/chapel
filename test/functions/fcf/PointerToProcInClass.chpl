//
// This sample from a user was a crash, but should work now.
// See issue: #17622
//
use M1 only Integrator;

var cc = new owned Integrator(R);
cc();

record R {}

module M1 {

  use CTypes;

  class Integrator {
    type T;

    proc this() {
      proc _helper() : real {
        return 0;
      }
      var gf = c_ptrTo(_helper);
    }
  }
}
