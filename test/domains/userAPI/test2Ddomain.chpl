use domainAPItest;

testDomainAPI2D("2D domain", {1..5, 0..4}, (3, 3), (0, 3), (3, 5), {0..6, 3..5});
testDomainAPI2D("strided 2D domain", {1..5 by 2, 0..4 by 2 align 1}, (3, 3), (0, 3), (3, 5), {0..6, 3..5});
