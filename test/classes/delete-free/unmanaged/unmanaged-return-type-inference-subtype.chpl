class Domain {
}
class MyDomain : Domain {
  var x;
}

config const which = true;
proc makeit() {
  if which then
    return new unmanaged MyDomain(1);
  else
    return new unmanaged Domain();
}

proc test() {
  var x = makeit();
  writeln(x.type:string);
  delete x;
}
test();
