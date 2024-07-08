interface Basic {

}

interface Advanced {
    Self implements Basic;
}

proc needsBasic(x: Basic) {}
proc needsAdvanced(x: Advanced) {
    needsBasic(x);
}
