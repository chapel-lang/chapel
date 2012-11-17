// BaseLocale.chpl
//

class BaseLocale : locale {

  proc id {
    return chpl_id;
  }

  proc readWriteThis(f: Writer) {
      f <~> new ioLiteral("BASELOCALE") <~> id;
  }

}

// Wrapper functions for locale-aware allocation/deallocation.
// This is used by the runtime to call into Chapel code, so we can have locale-
// aware allocation.
//
// TODO: Better type checking for mem_type.
//pragma "export" proc here_alloc(nbytes:int(32), mem_type:int(32), lineno:int(32) = 42, filename:string = "foo.c"): opaque {
//  return here.alloc(nbytes, mem_type, lineno, filename);
//}

extern proc printf(x...);

// This is our alloc routine, to show that we can allocate stuff through the 
// memory management interface implemented by a locale.
proc BaseLocale.alloc(type x, md) {
  printf("B");
  return __primitive("chpl_mem_alloc", x, md);
}

proc BaseLocale.free(x: object) {
  __primitive("chpl_mem_free", x);
  printf("b");
}


