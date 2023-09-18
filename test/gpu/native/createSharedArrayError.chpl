module Test {
  use GPU;

  record myRecord {
    var x: real(32);
    var y: real(32);
  }

  proc main() {
    on here.gpus[0] {
      foreach i in 0..<1024 {
        ref A = createSharedArray(myRecord, 16);
        A[i].x = A[i].y;
      }
    }
  }
}
