
record R {
  var x : int;
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
  var low : unmanaged Lower;

  proc replicate(valToReplicate) {
    coforall i in low do on Locales[i] {
      var x = valToReplicate;
      writeln(x.locale.id, " vs ", here);
    }
  }

  override proc myDestroy() {
    coforall i in low do on Locales[i] {
      writeln("Destroying ", here);
    }
  }
}

proc main() {
  var low = new unmanaged Lower();
  var repl = new unmanaged Replicate(low);

  var r = new R();
  repl.replicate(r);
  repl.myDestroy();

  delete repl;
  delete low;
}
