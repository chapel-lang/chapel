// The following works because 'xx' is resolved before it is used.
param xx = 2;
enumScopeTestWorks();
proc enumScopeTestWorks() {
  enum NumbersWorks {oneWorks=xx, two, three};
  compilerWarning("NumbersWorks.oneWorks = ", NumbersWorks.oneWorks:string);
  writeln(NumbersWorks.oneWorks);
}

// The following currently breaks upon 'one=x'. Should it work?
enumScopeTest();
param x = 2;
proc enumScopeTest() {
  enum Numbers {one=x, two, three};
  writeln(Numbers.one);
}
