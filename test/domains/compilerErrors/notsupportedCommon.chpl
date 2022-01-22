// Introduce the various types of domains
// to be tested.

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
