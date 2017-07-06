module LCALSParams {
  use LCALSEnums;

  config param realSize = 64;

  config param outputFormat: OutputStyle = OutputStyle.REFERENCE;
  type Real_type = real(realSize);
  type Complex_type = complex(2*realSize);

  const LCALS_DATA_ALIGN = 32;

}
