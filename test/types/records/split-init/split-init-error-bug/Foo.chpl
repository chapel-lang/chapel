module Foo {
  class foo {
    var size : int(64);
    param isWeighted : bool;
    var arrDom : domain(1) = {1..(if isWeighted then size else 0)};
  }
}
