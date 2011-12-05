// BaseLocale.chpl
//

class BaseLocale : locale {

  proc id {
    return chpl_id;
  }

  proc writeThis(f: Writer) {
      f.write("BASELOCALE", id);
    //} else {
    //  f.write("LOCALE", id);
    //}
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

// This is our alloc routine, to show that we can allocate stuff through the 
// memory management interface implemented by a locale.
export BaseLocale_alloc proc BaseLocale.alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  extern proc printf(str: string);
  printf("B");
  return chpl_mem_allocMany(1, nbytes, mem_type, lineno, filename);
}

proc BaseLocale.realloc(ptr: opaque, nbytes:int(32), size:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  chpl_mem_realloc(ptr, nbytes, size, mem_type, lineno, filename);
}

proc BaseLocale.free(ptr: opaque, lineno:int(32), filename:string) { 
  chpl_mem_free(ptr, lineno, filename);
}


