class myclass {
  var x: integer;
  var y: float;
}

function myclass.write(): void {
  write(x, " ", y);
}


var a: myclass = myclass();
var b: myclass = myclass();

write("a is: ", a, "b is: ", b);

a.x = 1;
a.y = 2.3;

b.x = 4;
b.y = 5.6;

write("a is: ", a, "b is: ", b);

a = b;

a.x = 7;
b.y = 8.9;

write("a is: ", a, "b is: ", b);
