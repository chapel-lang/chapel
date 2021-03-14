/*
A simplification of 
  test/classes/forwarding/point-of-insn-issue.chpl

which comes from GitHub issue #10358
*/

module Application {
  import Lib.ForwardingRecord;
  proc pr(param arg) {
    compilerError("success");
  }
  proc main() {
    var rec = new ForwardingRecord();
    rec.fn(1);
  }
}

module Lib {
  record ForwardingRecord {
    forwarding var instance = new ForwardedClass();
  }
  class ForwardedClass {
    proc fn(param arg1) {
      pr(1); // requires functions visible to the call being forwarded
    }
  }
}
