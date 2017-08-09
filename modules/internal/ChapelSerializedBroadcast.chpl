module ChapelSerializedBroadcast {
  use ChapelLocale;
  use CPtr;

  config param chpl__enableSerializedGlobals = true;

  proc chpl__broadcastGlobal(ref localeZeroGlobal : ?T, id : int)
  where chpl__enableSerializedGlobals {
    const data = localeZeroGlobal.chpl__serialize();
    const root = here.id;
    coforall loc in Locales do on loc {
      if here.id != root {
        pragma "no copy"
        pragma "no auto destroy"
        var temp = T.chpl__deserialize(data);

        extern var chpl_global_serialize_table : c_ptr(c_void_ptr);
        const destVoidPtr = (chpl_global_serialize_table + id).deref();
        const dest = destVoidPtr:c_ptr(localeZeroGlobal.type);

        __primitive("=", dest.deref(), temp);
      }
    }
  }

  proc chpl__destroyBroadcastedGlobal(ref localeZeroGlobal, id : int)
  where chpl__enableSerializedGlobals {
    type globalType = localeZeroGlobal.type;
    const root = here.id;
    coforall loc in Locales do on loc {
      if here.id != root {
        extern var chpl_global_serialize_table : c_ptr(c_void_ptr);
        const voidPtr = (chpl_global_serialize_table + id).deref();
        var ptr = voidPtr:c_ptr(globalType);

        pragma "no copy"
        pragma "no auto destroy"
        var temp = ptr.deref();

        chpl__autoDestroy(temp);
      }
    }
  }
}
