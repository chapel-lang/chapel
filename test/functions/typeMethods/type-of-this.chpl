
class C {
  proc type typeme() {
    compilerWarning(this:string);
  }
}

(C).typeme();
(owned C).typeme();
(shared C).typeme();
(borrowed C).typeme();
(unmanaged C).typeme();

(C?).typeme();
(owned C?).typeme();
(shared C?).typeme();
(borrowed C?).typeme();
(unmanaged C?).typeme();

compilerError("done"); // no need to continue compiling
