//
// TODO: Check to make sure 'const' storage kind is immutable.
//

use TrackingRecord;

proc r.enterContext() {
  writeln('entering');
}

proc r.exitContext(in err: owned Error?) {
  writeln('leaving');
  if err then try! { throw err; }
}
r implements contextManager;

record res {
  proc doSomething() {
    writeln('doing something with resource');
  }
}

var globalRes = new res();

// Multiple overloads of 'enterContext()' can exist, that just means that the
// resource storage cannot be inferred.
record man : contextManager {
  var x = new r();

  proc enterContext() ref {
    writeln('proc man.enterContext() ref: res');
    return globalRes;
  }

  proc enterContext() const ref {
    writeln('proc man.enterContext() const ref: res');
    return globalRes;
  }

  proc enterContext() {
    writeln('proc man.enterContext(): res');
    return globalRes;
  }

  proc exitContext(in err: owned Error?) {
    writeln('leaving');
    if err then try! { throw err; }
  }
}

proc doSomething() {
  writeln('doing something');
}

proc test1() {
  writeln('T1');
  // Calls the value overload of 'enterContext'.
  manage new man() do doSomething();
}

proc test2() {
  writeln('T2');
  // Calls the value overload of 'enterContext'.
  manage new man() {
    doSomething();
  }
}

// Make sure 'manager' stays alive till end of block.
proc test3() {
  writeln('T3');
  var myManager = new man();
  manage myManager do doSomething();
}

// Make sure 'manager' stays alive till end of block, nested.
proc test4() {
  writeln('T4');
  var myManager = new man();
  manage myManager, new man() {
    doSomething();
  }
}

// Make sure both managers stay alive till end of block.
proc test5() {
  writeln('T5');
  var myManager1 = new man();
  var myManager2 = new man();
  manage myManager1, myManager2 do
    doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test6() {
  writeln('T6: resource is explicitly var');
  manage new man() as var myResource do
    myResource.doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test7() {
  writeln('T7: resource is explicitly ref');
  manage new man() as ref myResource do
    myResource.doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test8() {
  writeln('T8: resource is explicitly const ref');
  manage new man() as const ref myResource do
    myResource.doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
// Make sure manager survives to end of block.
proc test9() {
  writeln('T9: resource is explicitly var');
  var myManager = new man();
  manage myManager as var myResource do
    myResource.doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
// Make sure manager survives to end of block.
proc test10() {
  writeln('T10: resource is explicitly ref');
  var myManager = new man();
  manage myManager as ref myResource do
    myResource.doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
// Make sure manager survives to end of block.
proc test11() {
  writeln('T11: resource is explicitly const ref');
  var myManager = new man();
  manage myManager as const ref myResource do
    myResource.doSomething();
}

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test12() {
  writeln('T12: nested managers, mixed resource storage types');
  manage new man() as var res1, new man() as ref res2,
         new man() as const ref res3 {
    res1.doSomething();
    res2.doSomething();
    res3.doSomething();
  }
}

// TODO: This may be dropped if we decide we don't want explicit storage.
// All managers should live till end of block.
proc test13() {
  writeln('T13: nested managers, mixed resource storage types');
  var myManager1 = new man();
  var myManager2 = new man();
  var myManager3 = new man();
  manage myManager1 as var res1, myManager2 as ref res2,
         myManager3 as const ref res3 {
    res1.doSomething();
    res2.doSomething();
    res3.doSomething();
  }
}

// TODO: This may be dropped if we decide we don't want explicit storage.
proc test14() {
  writeln('T14: same manager nested, mixed resource storage types');

  var myManager = new man();

  manage myManager as var res1, myManager as ref res2,
         myManager as const ref res3 {
    res1.doSomething();
    res2.doSomething();
    res3.doSomething();
  }
}

// This context manager is used to test throwing manage statements.
record rx : contextManager {
  var _userInterceptMsg: string;
  var _userReplacementMsg: string;
  var _throwOnExit: bool;
  var _numInterceptions = 0;

  proc numInterceptions do return _numInterceptions;

  proc init(interceptMsg: string='', replacementMsg: string='',
            throwOnExit: bool=false) {
    this._userInterceptMsg = interceptMsg;
    this._userReplacementMsg = replacementMsg;
    this._throwOnExit = throwOnExit;
  }

  proc type defaultReplacementMsg(num: int, msg: string) {
    var ret = 'Intercepted! (' + num:string + '): \'' + msg + '\'';
    return ret;
  }

  proc ref _replacementMsg(e: borrowed Error?) {
    var ret = if _userReplacementMsg.isEmpty()
        then this.type.defaultReplacementMsg(_numInterceptions, e!.message())
        else _userReplacementMsg;
    _numInterceptions += 1;
    return ret;
  }

  proc doSomething(msg='Something!') do writeln(msg);

  proc doSomethingThrowing(msg='Something throwing!') throws {
    throw new Error(msg);
  }

  proc ref enterContext() ref do return this;

  proc ref exitContext(in e: owned Error?) throws {
    if e {
      if !_userInterceptMsg.isEmpty() && e!.message() == _userInterceptMsg {
        const msg = _replacementMsg(e);
        throw new Error(msg);
      } else {
        throw e;
      }
    } else if _throwOnExit {
      const msg = 'Error thrown regardless!';
      throw new Error(msg);
    }
  }
}

proc test15() {
  writeln('T15: throwing method called in manage statement');

  var foo: rx;

  try {
    manage foo as x do x.doSomethingThrowing();
  } catch e {
    writeln(e);
  }
}

proc test16() {
  writeln('T16: \'throws\' occurs in manage statement');

  var foo: rx;

  try {
    manage foo as x do throw new Error('Uh oh!');
  } catch e {
    writeln(e);
  }
}

proc test17() {
  writeln('T17: \'throws\' occurs in manage statement and is intercepted');

  const msg = 'Hidden message!';
  var foo = new rx(msg);

  try {
    manage foo as x do throw new Error(msg);
  } catch e {
    writeln(e);
  }
}

proc test18() {
  writeln('T18: different managers nested, handling thrown errors');

  const msgToIntercept1 = 'Nested hidden message!';
  const replacementMsg1 = 'Passing on another message!';
  const msgToIntercept2 = replacementMsg1;

  var man1 = new rx(msgToIntercept2);
  var man2 = new rx(msgToIntercept1, replacementMsg1);
  var man3 = new rx();

  try {
    // m3: propagate -> m2: intecept -> m1: intercept
    manage man1 as m1, man2 as m2, man3 as m3 {
      m1.doSomethingThrowing(msgToIntercept1);
    }
  } catch e {
    assert(man1.numInterceptions == 1);
    assert(man2.numInterceptions == 1);
    assert(man3.numInterceptions == 0);
    writeln(e);
  }
}

proc test19() {
  writeln('T19: same manager nested, handling thrown errors');

  const msgToIntercept1 = 'Hidden message!';

  // Effectively propagates the same message upwards through replacements.
  var man = new rx(msgToIntercept1, msgToIntercept1);

  try {
    manage man as m1, man as m2, man as m3 {
      m1.doSomethingThrowing(msgToIntercept1);
    }
  } catch e {
    writeln(e);
    const n = man.numInterceptions;
    assert(n == 3);
    writeln(n);
  }
}

proc test20() {
  writeln('T20: manage statement does not throw but \'exitContext()\' does');

  var man = new rx(throwOnExit=true);

  try {
    manage man as m do m.doSomething();
  } catch e {
    assert(man.numInterceptions == 0);
    writeln(e);
  }
}

proc test21() {
  writeln('T21: nested managers, starts with \'exitContext()\' throwing');

  const msgToIntercept1 = 'Hidden message!';
  const replacementMsg1 = 'Passing on another message!';
  const msgToIntercept2 = 'Error thrown regardless!';
  const replacementMsg2 = 'Gotcha!';

  var man1 = new rx(msgToIntercept2, replacementMsg2);  // Ok
  var man2 = new rx(msgToIntercept1, replacementMsg1);  // Skipped
  var man3 = new rx(throwOnExit=true);                  // Start

  try {
    manage man1 as m1, man2 as m2, man3 as m3 {
      // No throwing occurs in the body!
      m1.doSomething();
      m2.doSomething();
      m3.doSomething();
    }
  } catch e {
    writeln(e);
    assert(man1.numInterceptions == 1);
    assert(man2.numInterceptions == 0);
    assert(man3.numInterceptions == 0);
  }
}

// Will always clean up on all paths.
proc test22() {
  writeln('T22: clean up occurs when \'return\' is in a manage statement');

  proc nestedThrows() throws {
    manage new rx(throwOnExit=true) as m {
      m.doSomething();
      return;
    }

    writeln('Never reaches here!');
  }

  try {
    nestedThrows();
  } catch e {
    writeln(e);
  }
}

// TODO: Manage statement is not currently considered as terminating control.
proc test23() {
  writeln('T23: cannot currently terminate control through a manage');

  proc nestedReturn8() {
    manage new rx() as m {
      m.doSomething();
      return 8;
    }
    // TODO: Remove this and you get a compiler error.
    return 0;
  }

  const n = nestedReturn8();
  writeln(n);
}

proc main() {
  const tests = [ test1, test2, test3, test4, test5, test6, test7, test8,
                  test9, test10, test11, test12, test13, test14, test15,
                  test16, test17, test18, test19, test20, test21, test22,
                  test23 ];
  for test in tests {
    test();
    if test != tests.last then writeln();
  }
}
