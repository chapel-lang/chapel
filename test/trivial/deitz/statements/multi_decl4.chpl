//var x, y: sync int;

var x: sync int;
var y: x.type;

/* Works:
     var x: sync int;
     var y: sync int;
*/

// Works:
//   var x: sync int, y: sync int;


x.writeEF(1);
y.writeEF(2);

writeln(x.readFE());
writeln(y.readFF());

