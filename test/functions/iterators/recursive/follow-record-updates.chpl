// Inpired by: test/release/examples/primers/iterators.chpl

/////////////////////////////////

class Tree {
  var first: bool;
  var left: owned Tree?;
}

iter postorder(tree: borrowed Tree?): borrowed Tree {
  if tree != nil {

    for child in postorder(tree!.left) do
      yield child;

    yield tree!;
  }
}

var GlobalTree = new owned Tree(true,
                   new owned Tree(false,
                     new owned Tree(true,
                       new owned Tree(false
                         ))));

/////////////////////////////////

record RRR {
  var FFF: int = 3;
}

var GlobalRec: RRR;

proc update() {
  GlobalRec.FFF += 10;
  writeln("updating FFF to ", GlobalRec.FFF);
}

proc report(const ref REPO: RRR) {
  writeln("FFF = ", REPO.FFF);
}

/////////////////////////////////

// The former bug was not seen if we add 'const ref' to 'ARG'.
proc displayThis(ARG: RRR) {

  for node in postorder(GlobalTree) {
    if node.first {
      report(ARG);
    } else {
      update();
    }
  }
}

proc main {
  displayThis(GlobalRec);
}
