/* Documentation for myDep */
module myDep {
  config param myDepConfig: int;

  use CTypes;
  extern proc getFromMyDep() : c_int;

  proc getAll() {
    return (getFromMyDep(), myDepConfig);
  }
}
