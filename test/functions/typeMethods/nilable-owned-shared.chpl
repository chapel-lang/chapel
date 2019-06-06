
class C {
  proc type typeme() {
    compilerWarning(this:string);
  }
}

(owned C?).typeme();
(shared C?).typeme();

compilerError("done"); // no need to continue compiling
