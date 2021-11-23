module foo {

config param p = true;

module M {
  public import (if p then super.a else super.b) as A;
}

module a {
  var x = 7;
}

module b {
  var x = 8;
}

import this.M;
writeln(M.A.x);

}