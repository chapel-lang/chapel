module M {
  @edition(last="2.0")
  proc myParam() param {
    return 17;
  }
  @edition(first="preview")
  proc myParam() param {
    return 42;
  }
  param x = myParam();
  compilerWarning("M.x=" + x:string);

  record T1 {}
  record T2 {}
  @edition(last="2.0")
  proc myType() type {
    return T1;
  }
  @edition(first="preview")
  proc myType() type {
    return T2;
  }

  type t = myType();
  compilerWarning("M.t=" + t:string);
}

import this.M;

param x = M.myParam();
compilerWarning("x=" + x:string);
type t = M.myType();
compilerWarning("t=" + t:string);
