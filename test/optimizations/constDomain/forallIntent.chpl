const colWidth = 12;
const fmt = "%-"+colWidth:string+"s|%-"+colWidth:string+"s\n";

{
  writeln("Passing var domain");
  writef(fmt, "intent", "definedConst");
  writef(fmt, "-"*colWidth, "-"*colWidth);

  var d = {1..10};
  {
    var isConst: bool;
    forall i in 1..2 with (ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "(const ref)", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (in d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "in", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (ref d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "ref", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (const d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "const (ref)", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (const in d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "const in", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (const ref d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "const ref", isConst);
  }
}

writeln();

{
  writeln("Passing const domain");
  writef(fmt, "intent", "definedConst");
  writef(fmt, "-"*colWidth, "-"*colWidth);

  const d = {1..10};
  {
    var isConst: bool;
    forall i in 1..2 with (ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "(const ref)", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (in d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "in", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (ref d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "ref", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (const d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "const (ref)", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (const in d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "const in", isConst);
  }

  {
    var isConst: bool;
    forall i in 1..2 with (const ref d, ref isConst) {
      if i == 1 then isConst = d.definedConst;
    }
    writef(fmt, "const ref", isConst);
  }
}
