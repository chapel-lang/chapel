
forall loc in Locales with (
                            var tpv1 = 5,
                            const tpv2 = tpv1 * 3.0,  // would be an error to reference tp3 or tp4
                            var tpv3: tpv1.type = 77,
                            var tpv4: tpv2.type = 88,
                            ref tpv5 = tpv1
) {
  tpv5 -= 4;
  writeln((tpv1, tpv2, tpv3, tpv4, tpv5));
}
