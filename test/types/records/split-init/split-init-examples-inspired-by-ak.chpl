module M {
  // inspired by Arkouda and its transition to non-nilable classes

  use Map;

  class Entry {
    var x: int;
  }

  class Table {
    var tab: map(string, shared Entry?);

    proc store(name: string, x: int) {
      if tab.contains(name) {
        tab[name]!.x = x;
      } else {
        tab.add(name, new shared Entry(x)?);
      }
    }
  }

  proc Table.lookup(name:string): borrowed Entry throws {

    if !tab.contains(name) {
      throw new Error("no entry for " + name);
    }

    ref r = tab[name]; // workaround for an issue solved by 14793
    return r!.borrow();
  }

  proc Table.tryLookup(name:string): borrowed Entry? {

    if !tab.contains(name) {
      return nil;
    }

    ref r = tab[name]; // workaround for an issue solved by 14793
    return r.borrow();
  }


  proc test0(t: Table, numerators:[] string, denominator:string): string {

    var denominatorEntryQ: borrowed Entry? = t.tryLookup(denominator);
    
    if denominatorEntryQ == nil {
      return "no entry for " + denominator;
    }
    var denominatorEntry = denominatorEntryQ!;

    if denominatorEntry.x == 0 then
      return "cannot divide by 0";

    var denom = denominatorEntry.x;

    for numerator in numerators {
      var numeratorEntryQ: borrowed Entry? = t.tryLookup(numerator);
      if numeratorEntryQ == nil {
        return "no entry for " + numerator;
      } else if numeratorEntryQ!.x == 0 {
        return "0 numerator not supported";
      }
    }

    var sum = 0;
    for numerator in numerators {
      var numeratorEntryQ: borrowed Entry? = t.tryLookup(numerator);
      sum += numeratorEntryQ!.x;
    }

    return "result: " + (sum / denom):string;
  }

  proc test1(t: Table, numerators:[] string, denominator:string): string {
    var denominatorEntryQ: borrowed Entry?;
    
    try {
      denominatorEntryQ = t.lookup(denominator);
    } catch e {
      return e.message();
    }

    var denominatorEntry = denominatorEntryQ!;

    if denominatorEntry.x == 0 then
      return "cannot divide by 0";

    var denom = denominatorEntry.x;

    for numerator in numerators {
      try {
        var numeratorEntry: borrowed Entry = t.lookup(numerator);
        if numeratorEntry.x == 0 then
          return "0 numerator not supported";
      } catch e {
        return e.message();
      }
    }

    var sum = 0;
    for numerator in numerators {
      try {
        var numeratorEntry: borrowed Entry = t.lookup(numerator);
        sum += numeratorEntry.x;
      } catch e {
        return e.message();
      }
    }

    return "result: " + (sum / denom):string;
  }

  proc test2(t: Table, numerators:[] string, denominator:string): string {
    var denominatorEntry: borrowed Entry;
    
    try {
      denominatorEntry = t.lookup(denominator);
    } catch e {
      return e.message();
    }

    if denominatorEntry.x == 0 then
      return "cannot divide by 0";

    var denom = denominatorEntry.x;

    for numerator in numerators {
      try {
        var numeratorEntry: borrowed Entry = t.lookup(numerator);
        if numeratorEntry.x == 0 then
          return "0 numerator not supported";
      } catch e {
        return e.message();
      }
    }

    var sum = 0;
    for numerator in numerators {
      try {
        var numeratorEntry: borrowed Entry = t.lookup(numerator);
        sum += numeratorEntry.x;
      } catch e {
        return e.message();
      }
    }

    return "result: " + (sum / denom):string;
  }

  proc doTest3(t: Table, numerators:[] string, denominator:string): string
                                                                    throws {
    var denominatorEntry: borrowed Entry;
    
    try {
      denominatorEntry = t.lookup(denominator);
    }

    if denominatorEntry.x == 0 then
      throw new Error("cannot divide by 0");

    var denom = denominatorEntry.x;

    for numerator in numerators {
      try {
        var numeratorEntry: borrowed Entry = t.lookup(numerator);
        if numeratorEntry.x == 0 then
          throw new Error("0 numerator not supported");
      }
    }

    var sum = 0;
    for numerator in numerators {
      try {
        var numeratorEntry: borrowed Entry = t.lookup(numerator);
        sum += numeratorEntry.x;
      }
    }

    return "result: " + (sum / denom):string;
  }

  proc test3(t: Table, numerators:[] string, denominator:string):string {
    try {
      return doTest3(t, numerators, denominator);
    } catch e {
      return e.message();
    }
  }

  proc main() {
    var t = new Table();
    t.store("z", 0);
    t.store("a", 1);
    t.store("b", 3);
    t.store("c", 2);

    writeln("test0");
    writeln(test0(t, ["a", "b"], "c"));
    writeln(test0(t, ["a", "b"], "z"));
    writeln(test0(t, ["z", "b"], "c"));
    writeln(test0(t, ["a", "b"], "x"));
    writeln(test0(t, ["a", "y"], "c"));
 
    writeln("test1");
    writeln(test1(t, ["a", "b"], "c"));
    writeln(test1(t, ["a", "b"], "z"));
    writeln(test1(t, ["z", "b"], "c"));
    writeln(test1(t, ["a", "b"], "x"));
    writeln(test1(t, ["a", "y"], "c"));
    
    writeln("test2");
    writeln(test2(t, ["a", "b"], "c"));
    writeln(test2(t, ["a", "b"], "z"));
    writeln(test2(t, ["z", "b"], "c"));
    writeln(test2(t, ["a", "b"], "x"));
    writeln(test2(t, ["a", "y"], "c"));

    writeln("test3");
    writeln(test3(t, ["a", "b"], "c"));
    writeln(test3(t, ["a", "b"], "z"));
    writeln(test3(t, ["z", "b"], "c"));
    writeln(test3(t, ["a", "b"], "x"));
    writeln(test3(t, ["a", "y"], "c"));
  }
}
