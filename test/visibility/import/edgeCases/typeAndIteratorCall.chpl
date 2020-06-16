module TestImportTheseFollower {
  // Import the type `set` from the submodule `MySet`.
  import this.MySet.set;

  proc test() {
    var s1 = new set();
    var s2 = new set();

    forall (x, y) in zip(s1, s2) do
      var tmp = x + y;

    return;
  }
  test();

  module MySet {

    // Define a simple "set" type.
    record set {
      iter these() { for i in 0..#100 do yield i; }

      iter these(param tag) where tag == iterKind.leader {
        var space = 0..#100;
        for followThis in space.these(tag) do yield followThis;
      }

      iter these(param tag, followThis) where tag == iterKind.follower {
        for idx in followThis(0) do yield idx;
      }
    }
  }
}
