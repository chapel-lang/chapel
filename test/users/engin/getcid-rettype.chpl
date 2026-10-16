class foo {}
var obj = new unmanaged foo();
var cid = __primitive("getcid", obj);
assert(cid.type==uint(32));
delete obj;
