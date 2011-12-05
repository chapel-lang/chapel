class SubLocale : locale {

  proc writeThis(f: Writer) {
      f.write("SUBLOCALE", id);
  }
}

export SubLocale_alloc proc SubLocale.alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string):opaque{ 
  extern proc printf(str: string);
  printf("S");
  return chpl_mem_allocMany(1, nbytes, mem_type, lineno, filename);
}

proc SubLocale.realloc(ptr: opaque, nbytes:int(32), size:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  chpl_mem_realloc(ptr, nbytes, size, mem_type, lineno, filename);
}

proc SubLocale.free(ptr: opaque, lineno:int(32), filename:string) { 
  chpl_mem_free(ptr, lineno, filename);
}


// Wrapper functions for locale-aware allocation/deallocation.
// This is used by the runtime to call into Chapel code, so we can have locale-
// aware allocation.
//
// TODO: Better type checking for mem_type.
//pragma "export" proc here_sub_alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string): opaque
//  return here.alloc(nbytes, mem_type, lineno, filename);
