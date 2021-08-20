// Check that a formal of an IC function whose type is an associated type
// can be passed to another IC function when doing so is legal due to an
// associated constraint.
//
// This test is simplified for --minimal-modules
// Once this functionality is implemented, we should beef up this code.

interface IFC {
  type AT;
  AT implements I2;
}

interface I2 {
  proc writeln(arg: Self);
}

proc ic(x: ?Q, y: Q.AT) where Q implements IFC {
  ic2(y);  // this call should be legal
}

proc ic2(z: ?R) where R implements I2 {
}
