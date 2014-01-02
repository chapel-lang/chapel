extern proc printem(ref x:uint(8), n:int(32));

param size = 5;
var t:size*uint(8);
for param i in 1..size {
  t[i] = i;
}

//printem(t[1], size);
test(t);

var a = t;
var b = a;

//writeln(a);
//printem(a[1], size);
//printem(b[1], size);
//test(b);

proc test(t:size*uint(8)) {
  writeln(t);
  //printem(t[1], size);
  var d = t;
  printem(d[1], size);
  /*var e:size*uint(8);
  for param i in 1..size {
    e[i] = t[i];
  }
  printem(e[1], size);*/
}

