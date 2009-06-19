config const message = "Hello, world!",
             printLocaleName = true;
             
coforall r in Realms {
  on r {
    coforall loc in r.Locales {
      on loc {
        var myMessage = message + " (from realm " + thisRealm.id
                      + " of " + numRealms + ", locale " + here.id
                      + " of " + thisRealm.numLocales;
        if (printLocaleName) then myMessage += " named " + here.name;
        myMessage += ")";
      
        writeln(myMessage);
      }
    }
  }
}


writeln("Realms is: ", Realms);

