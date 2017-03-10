class myLocale {
  
  proc name return chpl_name() : string;

  proc chpl_name() : string {
    _throwPVFCError();
    return "";
  }

}

class myAbstractLocaleModel : myLocale {
}

class myNumaDomain : myAbstractLocaleModel {
  const name: string;
  proc chpl_name() return name;

  proc myNumaDomain() {
    name = "test";
  }
}


var c = new myNumaDomain();
writeln(c.name);

