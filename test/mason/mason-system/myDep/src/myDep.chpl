/* Documentation for myDep */
module myDep {
  config param MyOtherChapelReal: real;
  config param MyOtherChapelInt: int;

  proc get() {
    return (MyOtherChapelReal, MyOtherChapelInt);
  }
}
