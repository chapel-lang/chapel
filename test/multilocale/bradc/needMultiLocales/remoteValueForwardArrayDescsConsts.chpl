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
    myBogusSyncYo$[10].writeEF(true);
    local {
      myArrayYo[10] = 1.1;
    }
    myBogusSyncYo$[10].readFE();
  }

  on Locales[numLocales-1] {
    local {
      myBogusSyncYo$[10].writeEF(true);
      myArrayYo[10] = 1.1;
      myBogusSyncYo$[10].readFE();
    }
  }

  on Locales[numLocales-1] {
    const myValYo = 1.1;
    myBogusSyncYo$[10].writeEF(true);
    local {
      myArrayYo[10] = myValYo;
    }
    myBogusSyncYo$[10].readFE();
  }

  //
  // Ultimately, this case shouldn't cause a remote value assertion
  // once we are remote value forwarding myValYo as we ought to be.
  // Brad's assertion is that we should never put such variables on
  // the heap to begin with as a means of ensuring that they are
  // remote value forwarded (i.e., make it break if we fail to do
  // so rather than be so expensive, as it is now).
  //
  const myValYo = 1.1;
  on Locales[numLocales-1] {
    myBogusSyncYo$[10].writeEF(true);
    local {
      myArrayYo[10] = myValYo;
    }
    myBogusSyncYo$[10].readFE();
  }

  writeln("Done!");
}

