use StencilDist;

class C {
}

const Dom = stencilDist.createDomain(1..10);
var A: [Dom] borrowed C?;
var B: [Dom] owned C?;
