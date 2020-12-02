use CPtr;
class C {
  var x:int;
}


proc main() {
  var obj = new unmanaged C(1);

  {
    var node = __primitive("_wide_get_node", obj);
    var loc = __primitive("_wide_get_locale", obj);
    var raddr = __primitive("_wide_get_addr", obj);
    var node2 = chpl_nodeFromLocaleID(loc);

    var c_void_nil:c_void_ptr = c_nil;
    assert(node == 0);
    assert(node2 == 0);
    assert(raddr != c_void_nil);
  }

  if numLocales > 1 {
    on Locales[1] {
      var node = __primitive("_wide_get_node", obj);
      var loc = __primitive("_wide_get_locale", obj);
      var raddr = __primitive("_wide_get_addr", obj);
      var node2 = chpl_nodeFromLocaleID(loc);

      var c_void_nil:c_void_ptr = c_nil;
      assert(node == 0);
      assert(node2 == 0);
      assert(raddr != c_void_nil);
    }
  }

  delete obj;
}

