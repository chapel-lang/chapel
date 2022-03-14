// These definitions are copied from
//   test/domains/compilerErrors/notsupportedCommon.chpl

var dfltRectangularBase = {1..3};
var dfltAssociativeBase = {1, 2, 3};

// Test a distributed domain.
use BlockDist;
var DistributedBase = dfltRectangularBase dmapped Block({1..3});

// Simple subdomains.
var dfltRectangularSubdomain: subdomain(dfltRectangularBase) = dfltRectangularBase;
var dfltAssociativeSubdomain: subdomain(dfltAssociativeBase) = dfltAssociativeBase;
var DistributedSubdomain:     subdomain(DistributedBase)     = DistributedBase;

// Sparse subdomains.
var dfltRectangularSparseSub: sparse subdomain(dfltRectangularBase);
//var dfltAssociativeSparseSub: sparse subdomain(dfltAssociativeBase);
var DistributedSparseSub:     sparse subdomain(DistributedBase);

// Test isXXX methods on domain classes.
proc testDom(dom, param name) {
  compilerWarning(name,
                  "  ", dom.isRectangular(): string,
                  "  ", dom.isAssociative(): string,
                  "  ", dom.isSparse(): string);
}

testDom(dfltRectangularBase,      "dfltRectangularBase");
testDom(dfltAssociativeBase,      "dfltAssociativeBase");
testDom(DistributedBase,          "DistributedBase");
testDom(dfltRectangularSubdomain, "dfltRectangularSubdomain");
testDom(dfltAssociativeSubdomain, "dfltAssociativeSubdomain");
testDom(DistributedSubdomain,     "DistributedSubdomain");
testDom(dfltRectangularSparseSub, "dfltRectangularSparseSub");
//testDom(dfltAssociativeSparseSub, "dfltAssociativeSparseSub");
testDom(DistributedSparseSub,      "DistributedSparseSub");

compilerError("done");
