use rectDomainAPItest;


var D = {2..4, 2..4};

testRectArrayAPI2D("plain-old domain", D, {1..3, 3..}, (3..5, ..3), {2..3, 3..4});
