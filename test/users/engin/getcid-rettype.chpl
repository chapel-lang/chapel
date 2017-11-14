class foo {}
var obj = new foo();
var cid = __primitive("getcid", obj);
assert(cid.type==int(32));
delete obj;
