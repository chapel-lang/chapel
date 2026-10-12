/* Documentation for myDep */
module myDep {
  config param SetCompoptFromToml: string;
  config param SetCompoptFromPrereq: string;

  proc getStrings() {
    return (SetCompoptFromToml, SetCompoptFromPrereq);
  }
  proc getMagic() {
    use CTypes;
    extern proc getMagicValue(): c_int;
    return getMagicValue();
  }
}
