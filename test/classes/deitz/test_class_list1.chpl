class node {
  var data : string;
  var next : node;
}

var head : node;

head = node();
head.data = "one";
head.next = node();
head.next.data = "two";
writeln(head.data);
writeln(head.next.data);
