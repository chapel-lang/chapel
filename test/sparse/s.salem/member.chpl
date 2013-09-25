//1 Dimension Array
var OneDimensionDomain = {1..3};
writeln(OneDimensionDomain.member(2)       );        //[TC1] Expected: True                   (Passed)
writeln(OneDimensionDomain.member( (2) )   );        //[TC2] Expected: True                   (Passed)
writeln(OneDimensionDomain.member(5)       );        //[TC3] Expected: False                  (Passed)
//writeln(OneDimensionDomain.member(1,5)     );      //[TC4] Expected: Compilation Error      (Passed, generated error: "index rank and type must match the domain rank and type")
//writeln(OneDimensionDomain.member( (1,5) ) );      //[TC5] Expected: Compilation Error      (Passed, generated error: "index rank and type must match the domain rank and type")

//2 Dimension Array
var TwoDimensionDomain = {1..3, 1..3};
writeln(TwoDimensionDomain.member(1,2)     );        //[TC6] Expected: True                   (Passed)
writeln(TwoDimensionDomain.member( (1,2) ) );        //[TC7] Expected: True                   (Passed)
writeln(TwoDimensionDomain.member(1,5)     );        //[TC8] Expected: False                  (Passed)
writeln(TwoDimensionDomain.member( (1,5) ) );        //[TC9] Expected: False                  (Passed)
//writeln(TwoDimensionDomain.member(1)       );      //[TC10] Expected: Compilation Error     (Passed, generated error: "index rank and type must match the domain rank and type")
//writeln(TwoDimensionDomain.member( (1) )   );      //[TC11] Expected: Compilation Error     (Passed, generated error: "index rank and type must match the domain rank and type")

//Sparse Domain
var spsDom: sparse subdomain(TwoDimensionDomain);
spsDom += (1,1);
spsDom += (1,2);
writeln(spsDom.member(1,1)       );                  //[TC12] Expected: True                  (Passed)
writeln(spsDom.member( (1,2) )   );                  //[TC13] Expected: True                  (Passed)
writeln(spsDom.member(5,2)       );                  //[TC14] Expected: False                 (Passed)
writeln(spsDom.member( (5,2) )   );                  //[TC15] Expected: False                 (Passed)
//writeln(spsDom.member(1,2,3 )    );                //[TC16] Expected: Compilation Error     (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(spsDom.member( (1,2,3) ) );                //[TC17] Expected: Compilation Error     (Passed: generated error "index rank and type must match the domain rank and type") 

//Associative Array
var AD_OneDim: domain(string);
AD_OneDim += "John";
writeln(AD_OneDim.member("John"));                   //[TC18] Expected: True                  (Passed)
writeln(AD_OneDim.member("Lara"));                   //[TC19] Expected: False                 (Passed)
//writeln(AD_OneDim.member("John","Lara"));          //[TC20] Expected: Compilation Error     (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(AD_OneDim.member( ("John","Lara") ));      //[TC21] Expected: Compilation Error     (Passed: generated error "index rank and type must match the domain rank and type") 

var AD_TwoDim: domain((string,string));
AD_TwoDim += ("A","B");
AD_TwoDim += ("C","D");
writeln(AD_TwoDim.member(("A","B")));               //[TC22] Expected: True                 (Passed)
writeln(AD_TwoDim.member( ("A","C") ));             //[TC23] Expected: False                (Passed)
//writeln(AD_TwoDim.member("A"));                     //[TC24] Expected: Compilation Error    (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(AD_TwoDim.member( ("A","B", "C") ));        //[TC25] Expected: Compilation Error    (Passed: generated error "index rank and type must match the domain rank and type") 

//Sparse Domain One Dimension
var spsDom_OneD: sparse subdomain(OneDimensionDomain);
spsDom_OneD += 1;
spsDom_OneD += 2;
writeln(spsDom_OneD.member(1)       );             //[TC26] Expected: True                  (Passed)
writeln(spsDom_OneD.member( (1) )   );             //[TC27] Expected: True                  (Passed)
writeln(spsDom_OneD.member(5)       );             //[TC28] Expected: False                 (Passed)
writeln(spsDom_OneD.member( (5) )   );             //[TC29] Expected: False                 (Passed)
//writeln(spsDom_OneD.member(1,2 )    );             //[TC30] Expected: Compilation Error     (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(spsDom_OneD.member( (1,2) ) );             //[TC31] Expected: Compilation Error     (Passed: generated error "index rank and type must match the domain rank and type") 

//Index Type Tests
//writeln(OneDimensionDomain.member(2.2)       );        //[TC32] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(OneDimensionDomain.member("2")       );        //[TC32] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(OneDimensionDomain.member("1","5")     );      //[TC33] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(TwoDimensionDomain.member("1","2")     );      //[TC34] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(TwoDimensionDomain.member( ("1","2") ) );      //[TC35] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(spsDom.member(1.5,1.4)       );                //[TC36] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(AD_OneDim.member(55));                         //[TC37] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(AD_TwoDim.member( (1,2) ));                    //[TC38] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
//writeln(spsDom_OneD.member( (5.5) )   );               //[TC39] Expected: Compilation Error   (Passed: generated error "index rank and type must match the domain rank and type") 
