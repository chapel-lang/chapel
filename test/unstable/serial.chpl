serial {
  coforall i in 1..100 {
    writeln("hello ", i);
  }
}

proc main() {
  serial {
    forall i in 1..100 {
      writeln("goodbye ", i);
    }
  }
}
