// This test is meant to verify that the runtime
// correctly propagates task-local data across
// begin/cobegin/coforall and the on variations
// of those.

config const setSerial=false;

record stuff {
  var oldE: _remoteEndCountType?;
  var oldS: bool;
  var e: _remoteEndCountType?;
  var s: bool;

  proc deinit() {
    if e then
      _endCountFree(e!);
  }
}

proc saveStuff() {
  var ret:stuff;
  ret.oldE = chpl_task_getDynamicEndCount();
  ret.oldS = chpl_task_getSerial();
  ret.e = _endCountAlloc(false);
  ret.s = setSerial;
  chpl_task_setDynamicEndCount(ret.e!);
  chpl_task_setSerial(ret.s);
  return ret;
}

proc checkStuff(st:stuff, name:string) {
  var e = chpl_task_getDynamicEndCount();
  var s = chpl_task_getSerial();
  if e == st.e && s == st.s then
    writeln(name, ": OK");
  else
    writeln(name, " --- error");
}

proc test() {
  var st = saveStuff();
  checkStuff(st, "basic");
}

test();

/* Doesn't work because sync modifies end count
   task-local storage that we are testing.
   If other task-local storage is available, it might
   be testable in this way.

proc testBegin() {
  var st = saveStuff();

  sync {
    begin {
      checkStuff(st, "begin");
    }
  }
}

testBegin();

proc testCoBegin() {
  var st = saveStuff();

  cobegin {
    {
      checkStuff(st, "cobegin");
    }
    {
      checkStuff(st, "cobegin");
    }
  }
}

testCoBegin();
*/

proc testCoForall() {
  var st = saveStuff();

  coforall i in 1..2 {
    checkStuff(st, "coforall");
  }
}

testCoForall();

proc testOn() {
  var st = saveStuff();
  on Locales[numLocales-1] {
    checkStuff(st, "on");
  }
}

testOn();

/* see comment above

proc testBeginOn() {
  var st = saveStuff();

  sync {
    begin on Locales[numLocales-1] {
      checkStuff(st, "begin on");
    }
  }
}

testBeginOn();


proc testOnBegin() {
  var st = saveStuff();

  sync {
    on Locales[numLocales-1] do begin {
      checkStuff(st, "on begin");
    }
  }
}

testOnBegin();
*/


proc testCoBeginOn() {
  var st = saveStuff();

  cobegin {
    on Locales[numLocales-1] {
      checkStuff(st, "cobegin on");
    }
    on Locales[numLocales-1] {
      checkStuff(st, "cobegin on");
    }
  }
}

testCoBeginOn();

proc testOnCobegin() {
  var st = saveStuff();

  on Locales[numLocales-1] {
    cobegin {
      checkStuff(st, "on cobegin");
      checkStuff(st, "on cobegin");
    }
  }
}

testOnCobegin();


proc testCoForallOn() {
  var st = saveStuff();

  coforall i in 1..2 {
    on Locales[numLocales-1] {
      checkStuff(st, "coforall on");
    }
  }
}

testCoForallOn();

proc testOnCoforall() {
  var st = saveStuff();

  on Locales[numLocales-1] {
    coforall i in 1..2 {
      checkStuff(st, "on coforall");
    }
  }
}

testOnCoforall();
