use LinkedList;

record myrecord {
  var x: int;
  var y: real;
}

var newrecord: myrecord;

newrecord.x = 1;
newrecord.y = 2.3;

writeln("newrecord is: ", newrecord);

var myseq: linkedList(myrecord) = makeList( newrecord );

writeln("myseq is: ", myseq);

myseq.destroy();
