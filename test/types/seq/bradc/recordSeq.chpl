record myrecord {
  var x: int;
  var y: float;
}

var newrecord: myrecord;

newrecord.x = 1;
newrecord.y = 2.3;

writeln("newrecord is: ", newrecord);

var myseq: seq(myrecord) = (/ newrecord /);

writeln("myseq is: ", myseq);
