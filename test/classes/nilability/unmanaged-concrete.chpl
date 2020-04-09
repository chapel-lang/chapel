// Verify with --devel that `unmanaged GEN` is concretely-non-nilable.

class GEN {
  type GT;
}

proc main {
  var ARR1: [LocaleSpace] borrowed GEN(int)?;
  var ARR2: [LocaleSpace] unmanaged GEN(int)?;
  var al = new unmanaged GEN(int);

  compilerWarning(ARR1.type:string);
  compilerWarning(ARR2.type:string);
  compilerWarning(al.type:string);

  compilerError("done");
}
