class myLocale {
  
  proc name do return chpl_name() : string;

  proc chpl_name() : string {
    use HaltWrappers only ;
    HaltWrappers.pureVirtualMethodHalt();
    return "";
  }

}

class myAbstractLocaleModel : myLocale {
}

class myNumaDomain : myAbstractLocaleModel {
  const myname: string;

  override proc name do return myname;
  proc chpl_name() do return name;

  proc init() {
    myname = "test";
  }
}


var c = new unmanaged myNumaDomain();
writeln(c.name);
delete c;
