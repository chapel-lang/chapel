use definesMultiUse;

b();
// Should fail, Inner1.b() is not visible because the use of Inner1 in
// definesMultiUse is private
