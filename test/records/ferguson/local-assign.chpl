use myrecord;

proc myfunction() {

  var local2: R;
  local2.init(x = 20);
  local2.verify();
  assert(local2.x == 20);

  var local1: R;
  local1.verify();

  local1 = local2;
  local1.verify();
  assert(local1.x == 20);

}

myfunction();

