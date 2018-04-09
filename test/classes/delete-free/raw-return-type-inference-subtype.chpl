class Domain {
}
class MyDomain : Domain {
  var x;
}

config const which = true;
proc makeit() {
  if which then
    return new raw MyDomain(1);
  else
    return new raw Domain();
}

proc test() {
  var x = makeit();
  writeln(x.type:string);
}
test();
