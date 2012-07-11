module remoteAtomics {
  const LINENO = -1:int(32); // it's be nice if we had something like __LINENO__

  proc ratomic(type t) type {
    if t==int || t==bool then
      return ratomic_int64;
    else if t==real then
      return ratomic_real64;
    else
      compilerError("Remote atomic operations not support on "+typeToString(t));
  }

  // int(64)
  extern proc chpl_comm_atomic_get_int64(inout result:int(64),
                                         l:int(32), inout obj:int(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_put_int64(inout desired:int(64),
                                         l:int(32), inout obj:int(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_add_int64(inout op:int(64),
                                         l:int(32), inout obj:int(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_add_int64(inout op:int(64),
                                               l:int(32), inout obj:int(64),
                                               inout result:int(64),
                                               ln:int(32), fn:string);
  extern proc chpl_comm_atomic_sub_int64(inout op:int(64),
                                         l:int(32), inout obj:int(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_sub_int64(inout op:int(64),
                                               l:int(32), inout obj:int(64),
                                               inout result:int(64),
                                               ln:int(32), fn:string);
  extern proc chpl_comm_atomic_and_int64(inout op:int(64),
                                         l:int(32), inout obj:int(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_and_int64(inout op:int(64),
                                               l:int(32), inout obj:int(64),
                                               inout result:int(64),
                                               ln:int(32), fn:string);
  extern proc chpl_comm_atomic_or_int64(inout op:int(64),
                                        l:int(32), inout obj:int(64),
                                        ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_or_int64(inout op:int(64),
                                              l:int(32), inout obj:int(64),
                                              inout result:int(64),
                                              ln:int(32), fn:string);
  extern proc chpl_comm_atomic_xor_int64(inout op:int(64),
                                         l:int(32), inout obj:int(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_xor_int64(inout op:int(64),
                                               l:int(32), inout obj:int(64),
                                               inout result:int(64),
                                               ln:int(32), fn:string);
  extern proc chpl_comm_atomic_cmpxchg_int64(inout expected:int(64),
                                             inout desired:int(64),
                                             l:int(32), inout obj:int(64),
                                             inout result:bool,
                                             ln:int(32), fn:string);

  record ratomic_int64 {
    var _v: int(64);
    inline proc read() {
      var ret: int(64);
      chpl_comm_atomic_get_int64(ret, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc write(value:int(64)) {
      var v = value;
      chpl_comm_atomic_put_int64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc exchange(value:int(64)):int(64) {
      var ret:int(64);
      //
      return ret;
    }
    inline proc compareExchange(expected:int(64), desired:int(64)):int(64) {
      var ret:bool;
      var te = expected;
      var td = desired;
      chpl_comm_atomic_cmpxchg_int64(te, td, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret==expected;
    }
    inline proc compareExchangeWeak(expected:int(64), desired:int(64)):int(64) {
      return compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:int(64), desired:int(64)):int(64) {
      return compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:int(64)):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_add_int64(v, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc add(value:int(64)):int(64) {
      var v = value;
      chpl_comm_atomic_add_int64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc fetchSub(value:int(64)):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_sub_int64(v, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc sub(value:int(64)):int(64) {
      var v = value;
      chpl_comm_atomic_sub_int64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc fetchOr(value:int(64)):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_or_int64(v, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc or(value:int(64)):int(64) {
      var v = value;
      chpl_comm_atomic_or_int64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc fetchAnd(value:int(64)):int(64) {
      var v = value;
      var ret:int(64);
      chpl_comm_atomic_fetch_and_int64(v, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc and(value:int(64)):int(64) {
      var v = value;
      chpl_comm_atomic_and_int64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc waitFor(val:int(64)) {
      on this do while (read() != val) do chpl_task_yield();
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(a:ratomic_int64, b:ratomic_int64) {
    a.write(b.read());
    return a;
  }
  inline proc =(a:ratomic_int64, b) {
    compilerError("Cannot directly assign remote atomic variables");
    return a;
  }
  inline proc +(a:ratomic_int64, b) {
    compilerError("Cannot directly add remote atomic variables");
    return a;
  }
  inline proc -(a:ratomic_int64, b) {
    compilerError("Cannot directly subtract remote atomic variables");
    return a;
  }
  inline proc *(a:ratomic_int64, b) {
    compilerError("Cannot directly multiply remote atomic variables");
    return a;
  }
  inline proc /(a:ratomic_int64, b) {
    compilerError("Cannot directly divide remote atomic variables");
    return a;
  }
  inline proc %(a:ratomic_int64, b) {
    compilerError("Cannot directly divide remote atomic variables");
    return a;
  }

  // real(64)
  extern proc chpl_comm_atomic_get_real64(inout result:real(64),
                                         l:int(32), inout obj:real(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_put_real64(inout desired:real(64),
                                         l:int(32), inout obj:real(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_add_real64(inout op:real(64),
                                         l:int(32), inout obj:real(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_add_real64(inout op:real(64),
                                               l:int(32), inout obj:real(64),
                                               inout result:real(64),
                                               ln:int(32), fn:string);
  extern proc chpl_comm_atomic_sub_real64(inout op:real(64),
                                         l:int(32), inout obj:real(64),
                                         ln:int(32), fn:string);
  extern proc chpl_comm_atomic_fetch_sub_real64(inout op:real(64),
                                               l:int(32), inout obj:real(64),
                                               inout result:real(64),
                                               ln:int(32), fn:string);
  extern proc chpl_comm_atomic_cmpxchg_real64(inout expected:real(64),
                                             inout desired:real(64),
                                             l:int(32), inout obj:real(64),
                                             inout result:bool,
                                             ln:int(32), fn:string);

  record ratomic_real64 {
    var _v: real(64);
    inline proc read() {
      var ret: real(64);
      chpl_comm_atomic_get_real64(ret, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc write(value:real(64)) {
      var v = value;
      chpl_comm_atomic_put_real64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc exchange(value:real(64)):real(64) {
      var ret:real(64);
      //
      return ret;
    }
    inline proc compareExchange(expected:real(64), desired:real(64)):real(64) {
      var ret:bool;
      chpl_comm_atomic_cmpxchg_real64(expected, desired, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc compareExchangeWeak(expected:real(64), desired:real(64)):real(64) {
      return compareExchange(expected, desired);
    }
    inline proc compareExchangeStrong(expected:real(64), desired:real(64)):real(64) {
      return compareExchange(expected, desired);
    }

    inline proc fetchAdd(value:real(64)):real(64) {
      var v = value;
      var ret:real(64);
      chpl_comm_atomic_fetch_add_real64(v, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc add(value:real(64)):real(64) {
      var v = value;
      chpl_comm_atomic_add_real64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc fetchSub(value:real(64)):real(64) {
      var v = value;
      var ret:real(64);
      chpl_comm_atomic_fetch_sub_real64(v, this.locale.id:int(32), this._v, ret, LINENO, "remoteAtomics.chpl");
      return ret;
    }
    inline proc sub(value:real(64)):real(64) {
      var v = value;
      chpl_comm_atomic_sub_real64(v, this.locale.id:int(32), this._v, LINENO, "remoteAtomics.chpl");
    }

    inline proc fetchOr(value:real(64)):real(64) {
      compilerError("or not definted for remote atomic real");
    }
    inline proc or(value:real(64)):real(64) {
      compilerError("or not definted for remote atomic real");
    }

    inline proc fetchAnd(value:real(64)):real(64) {
      compilerError("and not definted for remote atomic real");
    }
    inline proc and(value:real(64)):real(64) {
      compilerError("and not definted for remote atomic real");
    }

    inline proc waitFor(val:real(64)) {
      on this do while (read() != val) do chpl_task_yield();
    }

    proc writeThis(x: Writer) {
      x.write(read());
    }
  }

  inline proc =(a:ratomic_real64, b:ratomic_real64) {
    a.write(b.read());
    return a;
  }
  inline proc =(a:ratomic_real64, b) {
    compilerError("Cannot directly assign remote atomic variables");
    return a;
  }
  inline proc +(a:ratomic_real64, b) {
    compilerError("Cannot directly add remote atomic variables");
    return a;
  }
  inline proc -(a:ratomic_real64, b) {
    compilerError("Cannot directly subtract remote atomic variables");
    return a;
  }
  inline proc *(a:ratomic_real64, b) {
    compilerError("Cannot directly multiply remote atomic variables");
    return a;
  }
  inline proc /(a:ratomic_real64, b) {
    compilerError("Cannot directly divide remote atomic variables");
    return a;
  }
  inline proc %(a:ratomic_real64, b) {
    compilerError("Cannot directly divide remote atomic variables");
    return a;
  }

}

use remoteAtomics;
proc nativeAtomic(type t) type {
  if CHPL_COMM=="ugni" {
    if t==int(64) || t==bool || t==real then
      return ratomic(t);
    else if t!=int(64) && t!=bool && t!=real {
      compilerWarning("Using processor atomics for type "+typeToString(t));
      return atomic t;
    }
  } else return atomic t;
}
/*

writeln("CHPL_COMM: ", CHPL_COMM);
var rai: nativeAtomic(int);
writeln("rai: ", typeToString(rai.type));
writeln(rai);
resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] do rai.write(7);
stopCommDiagnostics();
writeln(getCommDiagnostics());
writeln(rai);
resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] do rai.add(1);
stopCommDiagnostics();
writeln(getCommDiagnostics());
writeln(rai);

var rar: nativeAtomic(real);
writeln("rar: ", typeToString(rar.type));
writeln(rar);
resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] do rar.write(7.0);
stopCommDiagnostics();
writeln(getCommDiagnostics());
writeln(rar);
resetCommDiagnostics();
startCommDiagnostics();
on Locales[numLocales-1] do rar.add(1.0);
stopCommDiagnostics();
writeln(getCommDiagnostics());
writeln(rar);

*/
