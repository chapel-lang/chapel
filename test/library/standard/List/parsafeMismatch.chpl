// This test has a .noexec because it will encounter a race when run, due to not
// being parallel safe.
use List;

record parSafeRecord {
  var blahText: string;

  proc init(text: string) {
    this.blahText = text;
  }
}



class C {
  var parSafeList: list(parSafeRecord);

  proc init() {
    this.parSafeList = new list(parSafeRecord, parSafe=true);
    init this;
  }

  proc parSafeAppend(text: string) {
    this.parSafeList.pushBack(new parSafeRecord(text));
  }
}


proc main {
  var c: shared C = new shared C();
  var chunk: int = 65536;
  coforall i in 0..chunk*100 by chunk with (ref c) {
    var textList = ['0','1','2','3','4','5'];
    {
      for t in textList {
        c.parSafeAppend(t);
      }
    }
  }
}
