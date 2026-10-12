/* Documentation for prereq2 */
module prereq2 {
  extern proc hello();
  config param myParam = 1;
  proc getMyParam() {
    return myParam;
  }
}
