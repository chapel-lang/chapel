class node {
  typedef element_type : integer;
  var element : element_type;
}

var n : node = node();

n.element = 12;

writeln(n.element);

var k : node.element_type;

k = 2;

writeln(k);
