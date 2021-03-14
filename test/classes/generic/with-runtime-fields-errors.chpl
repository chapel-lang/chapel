// Use non-prototype module to ensure strict nilability checking.
module w {

  config param opt = 0;

  class CC {
    type tField;
  }

  // a runtime type
  type RT = [Locales.domain] int;

  // instance type
  type IT =
    if      opt == 1 then CC
    else if opt == 2 then CC?
    else if opt == 3 then owned CC
    else if opt == 4 then owned CC?
    else if opt == 5 then shared CC
    else if opt == 6 then shared CC?
    else if opt == 7 then borrowed CC
    else if opt == 8 then borrowed CC?
    else if opt == 9 then unmanaged CC
    else                  unmanaged CC?
    ;

  var INST = new IT(RT);
  //compilerWarning(INST.type:string);

  var ARR: INST.tField;
  assert(ARR.dom == Locales.dom);
  if isUnmanagedClass(INST) then
    delete INST;
  writeln("done");
}
