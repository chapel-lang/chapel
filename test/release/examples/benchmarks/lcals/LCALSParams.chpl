module LCALSParams {
  config param realSize = 64;
  config param perfTesting = false;
  type Real_type = real(realSize);
  type Complex_type = complex(2*realSize);

  const LCALS_DATA_ALIGN = 32;

}
