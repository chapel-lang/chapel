module OuterModule {
  use LinkedLists;
  module LibGL {
    use LinkedLists;
    // Hidden in the bowels of some huge but useful Chapel library like LibGL.
    proc LinkedList.append(x: int) {
      halt("I Got you!");
    }
  }

  proc main {
    foo();
    bar();
  }

  proc foo() {
    var li = new LinkedList(real);
    li.append(3);
    li.append(4);
    writeln(li);
  }

  proc bar() {
    use LibGL;
    var li = new LinkedList(real);
    li.append(3);
  }
}
