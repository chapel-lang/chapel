class C
{
  var ref_count = 0;
}

record MyRecord
{
  var c:C;
}

proc MyRecord.chpl__initCopy(r: MyRecord)
{
  if c != nil then
    halt("This record is already initialized.");

  if r.c == nil then return;

  attach(r.c);
}

proc MyRecord.attach(c:C)
{
  this.detach();
  this.c = c;
  c.ref_count += 1;
}

proc MyRecord.detach()
{
  if c == nil then return;

  c.ref_count -= 1;
  if c.ref_count == 0 then delete c;
  c = nil;	// Just for safety.
}
  
var r:MyRecord;
r.c = new C();

var s = r;

writeln(s.c);
writeln(s.c.ref_count);
