// DefaultArchitecture.chpl
//
// Provides a default architectural description.
// This architectural description is backward compatible with
// the architecture implicitly provided by releases 1.6 and preceding.
//
pragma "no use ChapelStandard"
module DefaultArchitecture {

  use ChapelLocale;
  use DefaultRectangular;
  use ChapelNumLocales;

  // We would really like a class-static storage class.(C++ nomenclature)
  var doneCreatingLocales: bool = false;

  //
  // A concrete class representing the nodes in this architecture.
  //
  class DefaultNode : locale {
    const callStackSize: int;

    // This constructor must be invoked "on" the node
    // that it is intended to represent.  This trick is used
    // to establish the equivalence the "locale" field of the locale object
    // and the node ID portion of any wide pointer referring to it.
    proc DefaultNode() {
      if doneCreatingLocales {
        halt("Cannot create additional DefaultNode instances");
      }

      // chpl_nodeID is the node ID associated with the running image.
      chpl_id = __primitive("chpl_nodeID");

      // chpl_nodeName is defined in chplsys.c.
      // It supplies a node name obtained by running uname(3) on the current node.
      // For this reason as well, this constructor must be run on the node 
      // it is intende to describe.
      extern proc chpl_nodeName() : string;
      name = chpl_nodeName();

      extern proc chpl_task_getCallStackSize(): int;
      callStackSize = chpl_task_getCallStackSize();

      extern proc chpl_numCoresOnThisLocale(): int;
      numCores = chpl_numCoresOnThisLocale();
    }

    proc readWriteThis(f) {
      f <~> new ioLiteral("LOCALE") <~> id;
    }
  
    proc getChildSpace() return emptyLocaleSpace;
  
    proc getChildCount() return 0;
  
    iter getChildIndices() : int {
      for idx in emptyLocaleSpace do
        yield idx;
    }
  
    proc getChild(idx:int) : locale {
      if boundsChecking then
        halt("requesting a child from a DefaultNode locale");
      return nil;
    }
  
    iter getChldren() : locale  {
      for loc in emptyLocales do
        yield loc;
    }

    proc getChildArray() {
      return emptyLocales;
    }
  }
  
  //
  // An instance of this class is the default contents 'rootLocale'.
  // 
  // In the current implementation a platform-specific architectural description
  // may overwrite this instance or any of its children to establish a more customized
  // representation of the system resources.
  //
  class DefaultRootLocale : locale {

    // Would like to make myLocaleSpace distributed with one index per node.
    const myLocaleSpace: domain(1) = {0..numLocales-1};
    const myLocales: [myLocaleSpace] locale;
  
    proc DefaultRootLocale()
    {
      chpl_id = -1; // TODO: what should it be?
      name = "rootLocale";
      numCores = 0;

      // We cannot use a forall here because the default leader iterator will
      // access 'Locales' and 'here', which are not yet initialized.
      for locIdx in myLocaleSpace do
        // Would like to call addChild here, but for some reason it does not work. <hilde>
        on __primitive("chpl_on_locale_num", locIdx)
        {
          const node = new DefaultNode();
          _here = node;
          myLocales[locIdx] = node;
          numCores += node.numCores;
        }
      doneCreatingLocales = true;
    }

    proc readWriteThis(f) {
      f <~> new ioLiteral("rootLocale");
    }
  
    proc getChildSpace() return this.myLocaleSpace;
  
    proc getChildCount() return this.myLocaleSpace.numIndices;
  
    iter getChildIndices() : int {
      for idx in this.myLocaleSpace do
        yield idx;
    }
  
    proc getChild(idx:int) return this.myLocales[idx];
  
    iter getChldren() : locale  {
      for loc in this.myLocales do
        yield loc;
    }

    proc getChildArray() {
      return myLocales;
    }
  }
  
}
