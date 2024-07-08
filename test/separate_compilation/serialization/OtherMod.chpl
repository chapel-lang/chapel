
module OtherMod {
  proc bar() {
    writeln("MYPREFIX: OtherMod OtherMod OtherMod");
  }

  // work around the fact that 'bar' will not be resolved unless called
  export proc keepBar() {
    bar();
  }
}
