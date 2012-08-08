use BlockDist;

proc main() {
  var myDomYo = {1..10} dmapped Block(boundingBox={1..10});
  var myArrayYo: [myDomYo] real;
  var myBogusSyncYo$: [myDomYo] sync bool;

  on Locales[numLocales-1] {
    local {
      myArrayYo[10] = 1.1;
    }
  }

  on Locales[numLocales-1] {
    const myLocalBogusSyncYo$ = true;
    local {
      myArrayYo[10] = 1.1;
    }
  }

  on Locales[numLocales-1] {
    myBogusSyncYo$[10] = true;
    local {
      myArrayYo[10] = 1.1;
    }
    myBogusSyncYo$[10];
  }

  on Locales[numLocales-1] {
    local {
      myBogusSyncYo$[10] = true;
      myArrayYo[10] = 1.1;
      myBogusSyncYo$[10];
    }
  }

  on Locales[numLocales-1] {
    const myValYo = 1.1;
    myBogusSyncYo$[10] = true;
    local {
      myArrayYo[10] = myValYo;
    }
    myBogusSyncYo$[10];
  }

  writeln("Done!");
}

