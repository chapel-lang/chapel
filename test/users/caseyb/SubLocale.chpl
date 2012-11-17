// SubLocale.chpl
//

class SubLocale : locale {

  proc id {
    return chpl_id;
  }

  proc readWriteThis(f) {
    f <~> new ioLiteral("SUBLOCALE") <~> id;
  }
}

extern proc printf(x...);

proc SubLocale.alloc(type x, md) {
  printf("S");
  return __primitive("chpl_mem_alloc", x, md);
}

proc SubLocale.free(x: object) { 
  __primitive("chpl_mem_free", x);
}

