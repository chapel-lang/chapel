private var sID          : int  = 0;

private var sAllocated   : int  = 0;
private var sDeallocated : int  = 0;

private var sDebug       : bool = false;

record Rec
{
  var canary : int;
  var id     : int;
  var freed  : bool;

  proc Rec()
  {
    sID          = sID + 1;
    sAllocated   = sAllocated + 1;

    canary       = 0xBABEBABE;
    id           = sID;
    freed        = false;

    if (sDebug == true) then
      writeln("Executing Rec.Rec()    id: ", id);
  }

  proc ~Rec()
  {
    if (sDebug == true) then
      writeln("Executing Rec.~Rec()   id: ", id);

    sDeallocated = sDeallocated + 1;

    if (canary != 0xBABEBABE) then
      writeln("A record is being freed that was not allocated");

    if (freed == true) then
      writeln("Rec ", id, " is being freed more than once");

    freed = true;
  }
}

proc = (ref lhs: Rec, rhs : Rec)
{
  if lhs.canary != 0xBABEBABE then
    writeln("LHS was not initialized for assignment");

  if rhs.canary != 0xBABEBABE then
    writeln("RHS was not initialized for assignment");
}

proc main()
{
  run();

  if (sAllocated != sDeallocated)
  {
    writeln("Failed");
    writeln("  Allocated:      ", sAllocated);
    writeln("  Deallocated:    ", sDeallocated);
  }
  else
    writeln("Passed");
}

proc run()
{
  var a : Rec;

  scopeFree(0, a);
}

proc scopeFree(sel : int, out x : Rec)
{
  if (sel == 0)
  {
    var tmp : Rec;

    x = tmp;
  }

  return 0;
}
