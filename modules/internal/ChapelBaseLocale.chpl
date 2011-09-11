use ChapelLocale;
use ChapelSubLocale;


pragma "initialize prelocale" class baseLocale : locale {
  var sub : subLocale;

  proc baseLocale(/*r: realm = nil,*/ id = -1, uid = -1,
             numCores = 1/*__primitive("chpl_coresPerLocale")*/) {
    if doneCreatingLocales {
      halt("locales cannot be created");
    }
    //myRealm = r;
    chpl_id = id;
    chpl_uid = uid;
    myNumCores = numCores;
  }

  proc id {
    return chpl_id;
  }

  proc uid {
    return chpl_uid;
  }

  proc getChild() : locale {
    return sub;
  }

  proc writeThis(f: Writer) {
    //if (numRealms == 1) {
      f.write("BASELOCALE", id);
    //} else {
    //  f.write("LOCALE", myRealm.id, "-", id);
    //}
  }
}

proc chpl_int_to_locale(in id) {
  var r = thisRealm : realm;
  //for r in Realms {
    if (id < r.numLocales) {
      return r.Locales[id];
    } else if (id < (2 * r.numLocales)) {
      return r.SubLocales[id - r.numLocales];
    }
    id -= r.numLocales;
  //}

  halt("id out of range in chpl_int_to_locale()");
  return Realms[0].Locales[0];
}

// Wrapper functions for locale-aware allocation/deallocation.
// This is used by the runtime to call into Chapel code, so we can have locale-
// aware allocation.
//
// TODO: Better type checking for mem_type.
pragma "export" proc here_alloc(nbytes:int(32), mem_type:int(32), lineno:int(32) = 42, filename:string = "foo.c"): opaque {
  return here.alloc(nbytes, mem_type, lineno, filename);
}

proc chpl_setupLocale(uid) : baseLocale {
  var tmp: baseLocale;
  on __primitive("chpl_on_locale_num", uid) {
    tmp = new baseLocale(uid=uid); //combine this primitive and the allocate primitive into one
    //writeln(tmp);
    _here = tmp;
    if (defaultDist._value == nil) {
      defaultDist = new dmap(new DefaultDist());
    }
  }

  tmp.sub = chpl_setupSubLocale(0);
  return tmp : baseLocale;
}

// This is our alloc routine, to show that we can allocate stuff through the 
// memory management interface implemented by a locale.
proc baseLocale.alloc(nbytes:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  _extern proc printf(str: string);
  printf("B");
  return chpl_mem_allocMany(1, nbytes, mem_type, lineno, filename);
}

proc baseLocale.realloc(ptr: opaque, nbytes:int(32), size:int(32), mem_type:int(32), lineno:int(32), filename:string) : opaque { 
  chpl_mem_realloc(ptr, nbytes, size, mem_type, lineno, filename);
}

proc baseLocale.free(ptr: opaque, lineno:int(32), filename:string) { 
  chpl_mem_free(ptr, lineno, filename);
}


