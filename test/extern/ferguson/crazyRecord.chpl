/* This test checks that the compiler can correctly
   work with C functions taking in or returning
   structure arguments with different construction
   and different amounts of padding. The reason that
   is a bit complex is that clang, when generating
   the code for extern C functions, will pass and
   return structure arguments in registers of different
   type as part of "expandIndirectArgument".
   */
extern record A_BCde {
}
extern proc return_A_BCde():A_BCde;
extern proc print_A_BCde(in r:A_BCde);

extern record iAbc {
}
extern proc return_iAbc():iAbc;
extern proc print_iAbc(in r:iAbc);

extern record iaBc {
}
extern proc return_iaBc():iaBc;
extern proc print_iaBc(in r:iaBc);

extern record iabC {
}
extern proc return_iabC():iabC;
extern proc print_iabC(in r:iabC);

extern record iABc {
}
extern proc return_iABc():iABc;
extern proc print_iABc(in r:iABc);

extern record iaBC {
}
extern proc return_iaBC():iaBC;
extern proc print_iaBC(in r:iaBC);

extern record iaBCd {
}
extern proc return_iaBCd():iaBCd;
extern proc print_iaBCd(in r:iaBCd);

{ var r:A_BCde; r = return_A_BCde(); print_A_BCde(r); }
{ var r:iAbc; r = return_iAbc(); print_iAbc(r); }
{ var r:iaBc; r = return_iaBc(); print_iaBc(r); }
{ var r:iabC; r = return_iabC(); print_iabC(r); }
{ var r:iABc; r = return_iABc(); print_iABc(r); }
{ var r:iaBC; r = return_iaBC(); print_iaBC(r); }
{ var r:iaBCd; r = return_iaBCd(); print_iaBCd(r); }

