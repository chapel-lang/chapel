use ChapelLocale;
use ChapelBaseLocale;

pragma "initialize prelocale" class subLocale : locale {
  proc subLocale(/*r: realm = nil,*/ id = -2/*-1*/, uid = -2/*-1*/,
             numCores = 1/*__primitive("chpl_coresPerLocale")*/) {
    if doneCreatingLocales {
      halt("subLocales cannot be created");
    }
    //myRealm = r;
    chpl_id = id;
    chpl_uid = uid;
    myNumCores = numCores;
  }

  proc getChild() : locale {
    writeln("Cannot get sublocale from SubLocale");
    return nil;
  }

  proc writeThis(f: Writer) {
    //if (numRealms == 1) {
      f.write("SUBLOCALE", id);
    //} else {
    //  f.write("LOCALE", myRealm.id, "-", id);
    //}
  }
}

proc chpl_setupSubLocale(uid) {
  var tmp: subLocale;
  on __primitive("chpl_on_locale_num", uid) {
    tmp = new subLocale(uid=uid);
    if (defaultDist._value == nil) {
      defaultDist = new dmap(new DefaultDist());
    }
  }
  return tmp;
}

proc subLocale.alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string):opaque{ 
  _extern proc printf(str: string);
  printf("S");
  return chpl_mem_allocMany(1, nbytes, mem_type, lineno, filename);
}

proc subLocale.realloc(ptr: opaque, nbytes:int(32), size:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  chpl_mem_realloc(ptr, nbytes, size, mem_type, lineno, filename);
}

proc subLocale.free(ptr: opaque, lineno:int(32), filename:string) { 
  chpl_mem_free(ptr, lineno, filename);
}


// Wrapper functions for locale-aware allocation/deallocation.
// This is used by the runtime to call into Chapel code, so we can have locale-
// aware allocation.
//
// TODO: Better type checking for mem_type.
//pragma "export" proc here_sub_alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string): opaque
//  return here.alloc(nbytes, mem_type, lineno, filename);
