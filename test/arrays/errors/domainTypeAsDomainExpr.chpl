class Base {}
class Child : Base {
  var s: string;
}

proc fn(vec: [domain(1,int(64),false)] _owned(Child)) {
  return vec;
}

var x: [0..-1] owned Child;
x.push_back(new owned Child("C"));
writeln(x.type:string);
var y = fn(x);
writeln(x);
writeln(y);
