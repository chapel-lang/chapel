
record R {
  var x : int;
  proc R() { }
}

class Base {
  proc myDestroy() {
    writeln("Base destroy!");
  }
}

class Lower : Base {
  iter these() {
    for i in 0..#numLocales do yield i;
    myDestroy();
  }
}

class Replicate : Base {
  var low : Lower;

  proc replicate(valToReplicate) {
    coforall i in low do on Locales[i] {
      var x = valToReplicate;
      writeln(x.locale.id, " vs ", here);
    }
  }

  proc myDestroy() {
    coforall i in low do on Locales[i] {
      writeln("Destroying ", here);
    }
  }
}

proc main() {
  var low = new Lower();
  var repl = new Replicate(low);

  var r = new R();
  repl.replicate(r);
  repl.myDestroy();

  delete repl;
  delete low;
}
