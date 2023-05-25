param N : int = 3;
var parentDom : domain(N);
var spsDom: sparse subdomain(parentDom);
var idxBuf = spsDom.makeIndexBuffer(size=N);
