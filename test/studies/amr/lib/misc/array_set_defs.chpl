use domain_set_defs;

class ArraySet {

  const domain_set: DomainSet;
  var arrays: [domain_set.indices] IsolatedArray;

  def ArraySet(Dset: DomainSet) {
    for i in domain_set.indices do
      arrays(i) = new IsolatedArray(domain_set.domains(i));
  }

  def this(D: domain) {
    for i in domain_set.indices do
      if domain_set.domains(i) == D then return arrays(i);
    halt("ArraySet: Invalid domain provided.");
  }

}