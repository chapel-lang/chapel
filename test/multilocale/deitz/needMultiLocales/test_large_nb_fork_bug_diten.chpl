use Time;

proc main {
  on Locales(1) {
    cobegin {
      coforall i in 1..100 {
        sleep(3);
      }
      coforall i in 1..100 {
        sleep(3);
      }
    }
    sleep(3);
  }

  writeln("here 1");

  var tup: 300*int;

  on Locales(1) {
    begin {
      var t = tup;
    }
  }

  writeln("here 2");
}
