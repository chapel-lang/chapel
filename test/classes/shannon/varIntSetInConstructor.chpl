class setConst {
  var thisShouldBeSix      =  5;
  var thisShouldBeEleven   = 10;
  var thisShouldBeSixteen  = 15;

}



const const_testSix     : setConst = new setConst(thisShouldBeSix     =  6);
const const_testEleven  : setConst = new setConst(thisShouldBeEleven  = 11);
const const_testSixteen : setConst = new setConst(thisShouldBeSixteen = 16);

writeln("const_testSix.thisShouldBeSix         = ",
         const_testSix.thisShouldBeSix);
writeln("const_testEleven.thisShouldBeEleven   = ",
         const_testEleven.thisShouldBeEleven);
writeln("const_testSixteen.thisShouldBeSixteen = ",
         const_testSixteen.thisShouldBeSixteen);
writeln();

delete const_testSixteen;
delete const_testEleven;
delete const_testSix;





var var_testSix     : setConst = new setConst(thisShouldBeSix     =  6);
var var_testEleven  : setConst = new setConst(thisShouldBeEleven  = 11);
var var_testSixteen : setConst = new setConst(thisShouldBeSixteen = 16);

writeln("var_testSix.thisShouldBeSix         = ",
         var_testSix.thisShouldBeSix);
writeln("var_testEleven.thisShouldBeEleven   = ",
         var_testEleven.thisShouldBeEleven);
writeln("var_testSixteen.thisShouldBeSixteen = ",
         var_testSixteen.thisShouldBeSixteen);



delete var_testSixteen;
delete var_testEleven;
delete var_testSix;
