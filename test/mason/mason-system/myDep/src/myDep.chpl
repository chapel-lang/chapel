/* Documentation for myDep */
module myDep {
  config param MyOtherChapelReal: real;
  config param MyOtherChapelInt: int;

  proc get() {
    return (MyOtherChapelReal, MyOtherChapelInt);
  }

  require "myHeader.h";
  proc get2() {
    extern proc getMyExternalInt(): int;
    return getMyExternalInt();
  }
}
