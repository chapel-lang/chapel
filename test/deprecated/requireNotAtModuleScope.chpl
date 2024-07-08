
require "dummyFile.chpl";
class C {
  proc foo() {
    require "dummyFile.chpl"; // should warn
  }
}
record R {
  proc foo() {
    require "dummyFile.chpl"; // should warn
  }
}
proc foo() {
  require "dummyFile.chpl"; // should warn
}
if false then require "dummyFile.chpl"; // should warn
config const unknownAtCompileTime = true;
if unknownAtCompileTime then require "dummyFile.chpl"; // should warn
