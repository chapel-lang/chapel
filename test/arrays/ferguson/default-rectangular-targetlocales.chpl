use List;

config const verbose = false;

class C {
  var loc:locale;
  var A:[1..10] int;
}

var lst: list(owned C);

proc populateList() {
  if verbose then
    writeln("Adding ", here);

  lst.pushBack(new C(here)); 

  for i in 0..#here._getChildCount() {
    var subloc = here._getChild(i);
    on subloc {
      populateList();
    }
  }
}

for loc in Locales {
  on loc {
    populateList();
  }
}

for elt in lst {
  if verbose then
    writeln("elt.loc=", elt.loc, " elt!.locale=", elt!.locale,
            " elt.A.targetLocales()= ", elt.A.targetLocales());

  assert(elt.loc == elt!.locale);
  assert(elt.A.targetLocales().size == 1);
  assert(elt.A.targetLocales()[0] == elt.loc);
}
