class node {
  type element_type : integer;
  var element : element_type;
}

var n : node = node();

n.element = 12;

writeln(n.element);
