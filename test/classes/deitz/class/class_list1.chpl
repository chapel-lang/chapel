class node {
  var data : string;
  var next : unmanaged node?;
}

var head : unmanaged node?;

head           = new unmanaged node();
head!.data      = "one";

head!.next      = new unmanaged node();
head!.next!.data = "two";

writeln(head!.data);
writeln(head!.next!.data);

delete head!.next;
delete head;

