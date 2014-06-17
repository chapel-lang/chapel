class node {
  var data : string;
  var next : node;
}

var head : node;

head = new node();
head.data = "one";
head.next = new node();
head.next.data = "two";
writeln(head.data);
writeln(head.next.data);
