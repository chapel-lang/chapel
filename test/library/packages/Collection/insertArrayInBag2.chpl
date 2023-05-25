use DistributedBag;

record Node {
  var x: int;
  var y: [0..4] int;
}

var n = new Node(0, [0, 1, 2, 3, 4]);
var bag2 = new DistBag(Node, targetLocales=Locales);
bag2.add(n);
