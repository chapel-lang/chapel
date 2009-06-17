config const message = "Hello, world!",
             printLocaleName = true;
             
coforall r in Realms {
  on r {
    coforall loc in r.Locales {
      on loc {
        var myMessage = message + " (from realm " + /* thisRealm.id */ r.id
                      + " of " + numRealms + ", locale " + /* here.id */ loc.id
                      + " of " + r.numLocales;
        if (printLocaleName) then myMessage += " named " + loc.name;
        myMessage += ")";
      
        writeln(myMessage);
      }
    }
  }
}


writeln("Realms is: ", Realms);

