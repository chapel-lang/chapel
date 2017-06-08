module LCALSParams {
  config param realSize = 64;

  // outputFormat == 0 -> no timing/checksum output
  // outputFormat == 1 -> output matching the reference code
  // outputFormat == 2 -> output formatted for perf testing
  config param outputFormat = 1;
  type Real_type = real(realSize);
  type Complex_type = complex(2*realSize);

  const LCALS_DATA_ALIGN = 32;

}
