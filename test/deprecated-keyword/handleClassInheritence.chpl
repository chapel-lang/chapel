@deprecated(notes="'Dep' is deprecated, please use 'Undep' instead")
class Dep: Undep { }

class Undep { }

class UseDep: Dep { }

proc main {
  var d: owned UseDep?;
}
