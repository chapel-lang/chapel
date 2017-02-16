private var sID          : int  = 0;

private var sAllocated   : int  = 0;
private var sDeallocated : int  = 0;

private var sDebug       : bool = false;

record Rec
{
  var canary : int;
  var id     : int;
  var copy   : bool;
  var freed  : bool;

  proc Rec()
  {
    sID          = sID + 1;
    sAllocated   = sAllocated + 1;

    canary       = 0xBABEBABE;
    id           = sID;
    copy         = false;
    freed        = false;

    if (sDebug == true) then
      writeln("Constructing Rec       id:  ", id);
  }

  proc deinit()
  {
    if (sDebug == true) then
      writeln("Destroying   Rec       id:  ", id);

    sDeallocated = sDeallocated + 1;

    if (canary != 0xBABEBABE) then
      writeln("A record is being freed that was not allocated");

    if (freed == true) then
      writeln("A record is being freed more than once");

    freed = true;
  }
}

proc = (ref lhs: Rec, rhs : Rec)
{
  if (sDebug == true) then
    writeln("Assigning    Rec       lhs: ", lhs.id, " rhs: ", rhs.id);

  if lhs.canary != 0xBABEBABE then
    writeln("= operator: LHS was not initialized for assignment");

  if rhs.canary != 0xBABEBABE then
    writeln("= operator: RHS was not initialized for assignment");

  if rhs.freed  == true then
    writeln("= operator: RHS has been freed");
}

//
// Need to override the default initCopy as it does not
// invoked the constructor.

pragma "init copy fn"
proc chpl__initCopy(arg : Rec) {
  if (sDebug == true) then
    writeln("initCopying  Rec       arg: ", arg.id);

  if arg.canary != 0xBABEBABE then
    writeln("autoCopy with uninitialized record!");

  if arg.freed  == true then
    writeln("autoCopy with a record that has been freed!");

  var ret : Rec;

  return ret;
}

pragma "donor fn"
pragma "auto copy fn"
proc chpl__autoCopy(arg : Rec) {
  if (sDebug == true) then
    writeln("autoCopying  Rec       arg: ", arg.id);

  pragma "no auto destroy"
  var ret : Rec;

  if arg.canary != 0xBABEBABE then
    writeln("autoCopy with uninitialized record!");

  if arg.freed  == true then
    writeln("autoCopy with a record that has been freed!");

  ret.copy   = true;

  return ret;
}





proc main()
{
  run();
}

proc run()
{
  testReturnByValue();
  verify("ReturnByValue:");

  testOutIntent();
  verify("OutIntent:    ");
}

proc verify(tag) : void
{
  if (sAllocated == sDeallocated)
  {
    writeln(tag, " Passed");
  }
  else
  {
    writeln(tag, " Failed");
    writeln("  Allocated:       ", sAllocated);
    writeln("  Deallocated:     ", sDeallocated);
  }

  sAllocated   = 0;
  sDeallocated = 0;
}


/*
   Simple tests for record value return
*/

proc testReturnByValue() : void
{
  var a : Rec;
  var b : Rec;

       simpleReturn1();    // Do not use the result
  a =  simpleReturn2();    // Assign the result
  wrap(simpleReturn3());   // Use the result in a call

       multipleReturn1(0);
  a =  multipleReturn2(0);
  wrap(multipleReturn3(0));

       multipleReturn4(0);
  a =  multipleReturn5(0);
  wrap(multipleReturn6(0));

       multipleReturn7(0, b);
  a =  multipleReturn8(0, b);
  wrap(multipleReturn9(0, b));

       multipleReturn10(0);
  a =  multipleReturn11(0);
  wrap(multipleReturn12(0));
}

proc wrap(x : Rec) : void
{

}

//
// Return a record by value
//
proc simpleReturn1() : Rec
{
  var retval : Rec;

  return retval;
}

proc simpleReturn2() : Rec
{
  var retval : Rec;

  return retval;
}

proc simpleReturn3() : Rec
{
  var retval : Rec;

  return retval;
}



//
// Return a record by value from within a nested scope
//

proc multipleReturn1(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0) then
    return retval1;
  else
    return retval2;
}

proc multipleReturn2(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0) then
    return retval1;
  else
    return retval2;
}

proc multipleReturn3(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0) then
    return retval1;
  else
    return retval2;
}

proc multipleReturn4(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0) then
    return retval1;

  return retval2;
}

proc multipleReturn5(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0) then
    return retval1;

  return retval2;
}

proc multipleReturn6(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0) then
    return retval1;

  return retval2;
}

proc multipleReturn7(sel : int, x : Rec) : Rec
{
  var retval1 = x;

  if (sel == 0) then
    return retval1;

  var retval2 = x;

  return retval2;
}

proc multipleReturn8(sel : int, x : Rec) : Rec
{
  var retval1 = x;

  if (sel == 0) then
    return retval1;

  var retval2 = x;

  return retval2;
}

proc multipleReturn9(sel : int, x : Rec) : Rec
{
  var retval1 = x;

  if (sel == 0) then
    return retval1;

  var retval2 = x;

  return retval2;
}

proc multipleReturn10(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel >= 0)
  {
    if (sel == 0) then
      return retval1;
    else
      return retval2;
  }

  var retval3 = retval1;

  return retval3;
}


proc multipleReturn11(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel >= 0)
  {
    if (sel == 0) then
      return retval1;
    else
      return retval2;
  }

  var retval3 = retval1;

  return retval3;
}


proc multipleReturn12(sel : int) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel >= 0)
  {
    if (sel == 0) then
      return retval1;
    else
      return retval2;
  }

  var retval3 = retval1;

  return retval3;
}


/*
   Simple tests for a formal with out intent
*/

proc testOutIntent() : void
{
  var a : Rec;
  var b : Rec;

       simpleOutReturn1(b);         // Do not use the result
  a =  simpleOutReturn2(b);         // Assign the result
  wrap(simpleOutReturn3(b));        // Use the result in a call


       multipleOutReturn1(0, b);    // Do not use the result
  a =  multipleOutReturn2(0, b);    // Assign the result
  wrap(multipleOutReturn3(0, b));   // Use the result in a call
}

proc simpleOutReturn1(out x : Rec) : Rec
{
  var retval : Rec;

  x = retval;

  return retval;
}

proc simpleOutReturn2(out x : Rec) : Rec
{
  var retval : Rec;

  x = retval;

  return retval;
}

proc simpleOutReturn3(out x : Rec) : Rec
{
  var retval : Rec;

  x = retval;

  return retval;
}

proc multipleOutReturn1(sel : int, out x : Rec) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0)
  {
    x = retval1;
    return retval2;
  }

  var retval3 : Rec;

  x = retval3;
  return retval1;
}

proc multipleOutReturn2(sel : int, out x : Rec) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0)
  {
    x = retval1;
    return retval2;
  }

  var retval3 : Rec;

  x = retval3;
  return retval1;
}

proc multipleOutReturn3(sel : int, out x : Rec) : Rec
{
  var retval1 : Rec;
  var retval2 : Rec;

  if (sel == 0)
  {
    x = retval1;
    return retval2;
  }

  var retval3 : Rec;

  x = retval3;
  return retval1;
}
