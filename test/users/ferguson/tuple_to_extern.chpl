param size = 5;

extern proc printem(const ref x:size*uint(8), n:int(32));

var t:size*uint(8);
for param i in 0..size-1 {
  t[i] = i+1;
}

//printem(t[0], size);
test(t);

var a = t;
var b = a;

//writeln(a);
//printem(a[0], size);
//printem(b[0], size);
//test(b);

proc test(t:size*uint(8)) {
  writeln(t);
  //printem(t[0], size);
  var d = t;
  printem(d, size);
  /*var e:size*uint(8);
  for param i in 0..size-1 {
    e[i] = t[i];
  }
  printem(e[0], size);*/
}
