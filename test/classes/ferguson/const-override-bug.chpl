class myLocale {
  
  proc name return chpl_name() : string;

  proc chpl_name() : string {
    use HaltWrappers only ;
    HaltWrappers.pureVirtualMethodHalt();
    return "";
  }

}

class myAbstractLocaleModel : myLocale {
}

class myNumaDomain : myAbstractLocaleModel {
  const name: string;
  proc chpl_name() return name;

  proc init() {
    name = "test";
  }
}


var c = new unmanaged myNumaDomain();
writeln(c.name);
delete c;
