config const message = "Hello, world!",
             printLocaleName = true;
             

coforall loc in Locales {
  on loc {
    var myMessage = message + " (from locale " + here.id + " of " + numLocales;
    if (printLocaleName) then myMessage += " named " + loc.name;
    myMessage += ")";

    writeln(myMessage);
  }
}
