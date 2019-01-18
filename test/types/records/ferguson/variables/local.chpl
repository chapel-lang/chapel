use myrecord;

proc myfunction() {

  var local1: R;
  local1.verify();

  var local2: R;
  local2.setup(x = 20);
  local2.verify();
  assert(local2.x == 20);

}

myfunction();

verify();

