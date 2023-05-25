// This test is to track code that triggered an error at one point in CHAMPS
// code since our testing system didn't track it otherwise.  The error has since
// been fixed.
class Foo {
  var numGlobalNodes_: int = 0;
  var connectivitySize_ : int = 0;
  var connectivityDom_ : domain(1) = {0..#connectivitySize_};
  var facet2NodeList_ : [connectivityDom_] int;

  proc init(numGlobalNodes: int, connectivitySize : int, facet2NodeList : [?D] int) {
    if D.size != connectivitySize then halt("Size of connectivity does not match length of facet2NodeList array: ", D.size, " != ", connectivitySize);
    if (max reduce facet2NodeList) > numGlobalNodes then halt ("blah blah blah");

    this.init(numGlobalNodes, connectivitySize);

    this.facet2NodeList_ = facet2NodeList;
  }

  proc init(numGlobalNodes: int = 0, connectivitySize : int = 0) {
    this.numGlobalNodes_ = numGlobalNodes;
    this.connectivitySize_ = connectivitySize;
  }
}

proc main() {
  var nodeList: [0..<15] int = [2, 9, 3, 6, 2, 4, 3, 3, 6, 9, 2, 1, 4, 3, 7];
  var x = new owned Foo(10, 15, nodeList);

  writeln(x);

}
