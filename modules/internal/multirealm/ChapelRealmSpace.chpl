const chpl_RealmBox: domain(1) = [0..numRealms-1];

const RealmSpace: sparse subdomain(chpl_RealmBox) = chpl_genNonEmptyRealms();

iter chpl_genNonEmptyRealms() {
  for i in chpl_RealmBox do
    if chpl_numLocales(i) > 0 then
    yield i;
}

