module Program {
  proc main() {
    use bash only;

    var p = bash.ls();

    writeln(p.x);
  }
}

module bash {
  public use Other; // does not bring in Other
  use Other; // does bring in Other but only for local use

  proc ls(args='') {
    var p = Other.makeFoo();
    return p;
  }
}

module Other {
  record Foo {
    var x = true;
  }

  proc makeFoo() {
    return new Foo();
  }
}
