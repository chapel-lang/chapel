// Verifies that PRIM_TO_NILABLE_CLASS and PRIM_TO_NON_NILABLE_CLASS
// produce the expected results when invoked on class types.

// concrete
class C {
  var x: int;
}

type co_b = owned C;
type co_q = owned C?;
compilerWarning("owned C -> nil  ", _to_nilable(co_b):string);
compilerWarning("owned C -> non  ", _to_nonnil(co_b):string);
compilerWarning("owned C? -> nil  ", _to_nilable(co_q):string);
compilerWarning("owned C? -> non  ", _to_nonnil(co_q):string);

type cs_b = shared C;
type cs_q = shared C?;
compilerWarning("shared C -> nil  ", _to_nilable(cs_b):string);
compilerWarning("shared C -> non  ", _to_nonnil(cs_b):string);
compilerWarning("shared C? -> nil  ", _to_nilable(cs_q):string);
compilerWarning("shared C? -> non  ", _to_nonnil(cs_q):string);

type cb_b = borrowed C;
type cb_q = borrowed C?;
compilerWarning("borrowed C -> nil  ", _to_nilable(cb_b):string);
compilerWarning("borrowed C -> non  ", _to_nonnil(cb_b):string);
compilerWarning("borrowed C? -> nil  ", _to_nilable(cb_q):string);
compilerWarning("borrowed C? -> non  ", _to_nonnil(cb_q):string);

type cu_b = unmanaged C;
type cu_q = unmanaged C?;
compilerWarning("unmanaged C -> nil  ", _to_nilable(cu_b):string);
compilerWarning("unmanaged C -> non  ", _to_nonnil(cu_b):string);
compilerWarning("unmanaged C? -> nil  ", _to_nilable(cu_q):string);
compilerWarning("unmanaged C? -> non  ", _to_nonnil(cu_q):string);

// generic
class D {
  var y;
}

type do_b = owned D;
type do_q = owned D?;
compilerWarning("owned D -> nil  ", _to_nilable(do_b):string);
compilerWarning("owned D -> non  ", _to_nonnil(do_b):string);
compilerWarning("owned D? -> nil  ", _to_nilable(do_q):string);
compilerWarning("owned D? -> non  ", _to_nonnil(do_q):string);

type ds_b = shared D;
type ds_q = shared D?;
compilerWarning("shared D -> nil  ", _to_nilable(ds_b):string);
compilerWarning("shared D -> non  ", _to_nonnil(ds_b):string);
compilerWarning("shared D? -> nil  ", _to_nilable(ds_q):string);
compilerWarning("shared D? -> non  ", _to_nonnil(ds_q):string);

type db_b = borrowed D;
type db_q = borrowed D?;
compilerWarning("borrowed D -> nil  ", _to_nilable(db_b):string);
compilerWarning("borrowed D -> non  ", _to_nonnil(db_b):string);
compilerWarning("borrowed D? -> nil  ", _to_nilable(db_q):string);
compilerWarning("borrowed D? -> non  ", _to_nonnil(db_q):string);

type du_b = unmanaged D;
type du_q = unmanaged D?;
compilerWarning("unmanaged D -> nil  ", _to_nilable(du_b):string);
compilerWarning("unmanaged D -> non  ", _to_nonnil(du_b):string);
compilerWarning("unmanaged D? -> nil  ", _to_nilable(du_q):string);
compilerWarning("unmanaged D? -> non  ", _to_nonnil(du_q):string);

compilerError("done");
