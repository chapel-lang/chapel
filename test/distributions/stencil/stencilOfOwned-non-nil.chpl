use StencilDist;

class C {
}

const Dom = stencilDist.createDomain(1..10);
var A: [Dom] owned C = [i in Dom] new owned C();
