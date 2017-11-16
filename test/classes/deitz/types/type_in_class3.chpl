class node {
  type element_type = int;
  var element : element_type;
}

var n : node = new node();

n.element = 12;

writeln(n.element);

var k : node.element_type;

k = 2;

writeln(k);

delete n;
