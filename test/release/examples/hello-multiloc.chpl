config const printLocaleName = true;
             

coforall loc in Locales {
  on loc {
    var message = "Hello, world! (from locale " + here.id + " of " + numLocales;
    if (printLocaleName) then message += " named " + loc.name;
    message += ")";

    writeln(message);
  }
}
