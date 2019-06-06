class C {}
class D: C {}

proc type (owned C).mOwned() { compilerWarning("mOwned",1); }
proc type (shared C).mShared() { compilerWarning("mShared",1); }

(owned D).mOwned();
(shared D).mShared();

compilerError("done"); // no need to continue compiling
