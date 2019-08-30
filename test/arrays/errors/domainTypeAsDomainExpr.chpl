class Base {}
class Child : Base {
  var s: string;
}

proc fn(vec: [domain(1,int(64),false)] owned Child) {
  return vec;
}

var x: [0..0] owned Child = new owned Child("C");
writeln(x.type:string);
var y = fn(x);
writeln(x);
writeln(y);
