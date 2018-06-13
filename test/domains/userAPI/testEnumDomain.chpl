use domainAPItest;

enum color {red=-1, orange, yellow, green, blue, indigo, violet};
use color;

testDomainAPI2D("2D enum domain", {orange..green, yellow..blue}, (yellow, green), (red, green), (yellow, violet), {red..violet, green..violet});
testDomainAPI2D("strided 2D enum domain", {orange..green by 2, yellow..blue by 2}, (green, yellow), (orange, green), (orange, violet), {red..violet, green..violet});
