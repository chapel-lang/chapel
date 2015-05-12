#include "qio_formatted.h"
#include <assert.h>
#include <math.h>
#include <locale.h>
#include <langinfo.h>

int verbose = 0;

void test_endian(void)
{
  // We write (hex) 00 0102 03040506 0708091011121314
  // as big endian and little endian, and then we check
  // that the data is what we expected.
  
  qioerr err;
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  int64_t offset;
  int len = 15;
  uint8_t n0 = 0;
  uint16_t n1 = 0x0102;
  uint32_t n2 = 0x03040506;
  uint64_t n3 = 0x0708091011121314LL;
  const char* expect_le = "\x00\x02\x01\x06\x05\x04\x03"
                          "\x14\x13\x12\x11\x10\x09\x08\x07";
  const char* expect_be = "\x00\x01\x02\x03\x04\x05\x06"
                          "\x07\x08\x09\x10\x11\x12\x13\x14";
  char got[16];
  int i;
  int b_order;
  const char* expect;

  if( verbose ) printf("Testing endian functions\n");

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  for( i = 0; i < 2; i++ ) {
    if( i == 0 ) {
      b_order = QIO_BIG;
      expect = expect_be;
    } else {
      b_order = QIO_LITTLE;
      expect = expect_le;
    }

    // Create a "write to file" channel.
    err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, NULL);
    assert(!err);

    err = qio_channel_write_uint8(true, writing, n0);
    assert(!err);
    err = qio_channel_write_uint16(true, b_order, writing, n1);
    assert(!err);
    err = qio_channel_write_uint32(true, b_order, writing, n2);
    assert(!err);
    err = qio_channel_write_uint64(true, b_order, writing, n3);
    assert(!err);

    err = qio_channel_offset(true, writing, &offset);
    assert(!err);

    assert( offset == len );

    qio_channel_release(writing);
    writing = NULL;

    // Create a "read from file" channel.
    err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
    assert(!err);

    err = qio_channel_read_amt(true, reading, got, len);
    assert(!err);

    // Check that we read back what we wrote.
    assert( 0 == memcmp(got, expect, len) );

    qio_channel_release(reading);
    reading = NULL;
  }

  // Close the file.
  qio_file_release(f);
  f = NULL;


}

void test_readwriteint(void)
{
  qioerr err;
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;
  const char* testdata[] = {"\x00\x00\x00\x00\x00\x00\x00\x00",
                            "\xff\xff\xff\xff\xff\xff\xff\xff",
                            "\x7f\x7f\x7f\x7f\x7f\x7f\x7f\x7f",
                            "\x01\x02\x03\x04\x05\x06\x07\x08",
                            "\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8",
                            NULL };
  int sizes[] = {1, -1, 2, -2, 4, -4, 8, -8, 0};
  int byteorder[] = {QIO_NATIVE, QIO_BIG, QIO_LITTLE, 0};
  char got[16];
  int i,j,k;

  if( verbose ) printf("Testing binary integer I/O\n");

  // Open a temporary file.
  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  for( i = 0; testdata[i]; i++ ) {
    for( j = 0; sizes[j]; j++ ) {
      for( k = 0; byteorder[k]; k++ ) {
        const char* data = testdata[i];
        int sz = sizes[j];
        int b_order = byteorder[k];
        int len;

        len = sz;
        if( len < 0 ) len = - sz;


        memset(got, 0, sizeof(got));

        // Create a "write to file" channel.
        err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, NULL);
        assert(!err);

        err = qio_channel_write_int(true, b_order, writing, data, len, sz < 0);
        assert(!err);

        qio_channel_release(writing);
        writing = NULL;

        // Create a "read from file" channel.
        err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
        assert(!err);


        err = qio_channel_read_int(true, b_order, reading, got, len, sz < 0);
        assert(!err);

        // Check that we read back what we wrote.
        assert( 0 == memcmp(got, data, len) );

        qio_channel_release(reading);
        reading = NULL;
      }
    }
  }

  // Close the file.
  qio_file_release(f);
  f = NULL;

}

void test_printscan_int(void)
{
  qioerr err;
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;

#define NSTYLES 12
  qio_style_t styles[NSTYLES];

  const char* zero[] = { // writing 0
                        "0", // default style
                        "  0", // min_width = 3, pad = ' '
                        "000", // min_width = 3, pad = 0
                        "0  ", // min_width = 3, pad = ' ', leftjustify
                        "0", // hex
                        "0x0", // hex, showbase
                        "   0", // binary, min_width = 4
                        "0b0", // binary, showbase
                        " 0b0", // binary, showbase, showplus=pad
                        "+0X0", // hex, showbase, showplus=+, uppercase
                        "0", // octal
                        "0o0" // octal, showbase
                       };

  const char* one[] = { // writing 1
                        "1", // default style
                        "  1", // min_width = 3, pad = ' '
                        "001", // min_width = 3, pad = 0
                        "1  ", // min_width = 3, pad = ' ', leftjustify
                        "1", // hex
                        "0x1", // hex, showbase
                        "   1", // binary, min_width = 4
                        "0b1", // binary, showbase
                        " 0b1", // binary, showbase, showplus=pad
                        "+0X1", // hex, showbase, showplus=+, uppercase
                        "1", // octal
                        "0o1" // octal, showbase
                       };
  const char* hundred[] = { // writing 100 = 0x64 = 0b1100100
                        "100", // default style
                        "100", // min_width = 3, pad = ' '
                        "100", // min_width = 3, pad = 0
                        "100", // min_width = 3, pad = ' ', leftjustify
                        "64", // hex
                        "0x64", // hex, showbase
                        "1100100", // binary, min_width = 4
                        "0b1100100", // binary, showbase
                        " 0b1100100", // binary, showbase, showplus=pad
                        "+0X64", // hex, showbase, showplus=+, uppercase
                        "144", // octal
                        "0o144" // octal, showbase
                          };
  const char* big[] = { // writing 2^63-1 = 9223372036854775807 = 0x7FFFFFFFFFFFFFFF = 0b111111111111111111111111111111111111111111111111111111111111111
                        "9223372036854775807", // default style
                        "9223372036854775807", // min_width = 3, pad = ' '
                        "9223372036854775807", // min_width = 3, pad = 0
                        "9223372036854775807", // min_width = 3, pad = ' ', leftjustify
                        "7fffffffffffffff", // hex
                        "0x7fffffffffffffff", // hex, showbase
                        "111111111111111111111111111111111111111111111111111111111111111", // binary, min_width = 4
                        "0b111111111111111111111111111111111111111111111111111111111111111", // binary, showbase
                        " 0b111111111111111111111111111111111111111111111111111111111111111", // binary, showbase, showplus=pad
                        "+0X7FFFFFFFFFFFFFFF", // hex, showbase, showplus=+, uppercase
                        "777777777777777777777", // octal
                        "0o777777777777777777777" // octal, showbase
                          };
  const char* small[] = { // writing -2^63 = -9223372036854775808 = -0x8000000000000000 = -0b1000000000000000000000000000000000000000000000000000000000000000
                        "-9223372036854775808", // default style
                        "-9223372036854775808", // min_width = 3, pad = ' '
                        "-9223372036854775808", // min_width = 3, pad = 0
                        "-9223372036854775808", // min_width = 3, pad = ' ', leftjustify
                        "-8000000000000000", // hex
                        "-0x8000000000000000", // hex, showbase
                        "-1000000000000000000000000000000000000000000000000000000000000000", // binary, min_width = 4
                        "-0b1000000000000000000000000000000000000000000000000000000000000000", // binary, showbase
                        "-0b1000000000000000000000000000000000000000000000000000000000000000", // binary, showbase, showplus=pad
                        "-0X8000000000000000", // hex, showbase, showplus=+, uppercase
                        "-1000000000000000000000", // octal
                        "-0o1000000000000000000000" // octal, showbase
                          };

  int64_t nums[] = {0, 1, 100, INT64_MAX, INT64_MIN, 0};
  const char** expect_arr[] = { zero, one, hundred, big, small, NULL};
  char got[500];
  char sep[4] = {0,0,0,0};
  ssize_t amt_read;
  int i,j;

  if( verbose ) printf("Testing text integer I/O\n");

  for( i = 0; i < NSTYLES; i++ ) {
    qio_style_init_default(&styles[i]);
  }
  // Set up the styles
  // 0 is default.

  // 1 has minwidth 3, pad ' '
  styles[1].min_width_columns = 3;
  styles[1].pad_char = ' ';

  // 2 has minwidth 3, pad 0
  styles[2].min_width_columns = 3;
  styles[2].pad_char = '0';

  // 3 has minwidth 3, pad ' ', leftjustify
  styles[3].min_width_columns = 3;
  styles[3].pad_char = ' ';
  styles[3].leftjustify = 1;

  // 4 has defaults, hex
  styles[4].base = 16;
  styles[4].prefix_base = 0;

  // 5 has defaults, hex, showbase
  styles[5].base = 16;
  styles[5].prefix_base = 1;

  // 6 has defaults, binary, min_width=4
  styles[6].base = 2;
  styles[6].min_width_columns = 4;
  styles[6].prefix_base = 0;

  // 7 has defaults, binary, showbase
  styles[7].base = 2;
  styles[7].prefix_base = 1;

  // 8 has showbase, showplus = pad, binary
  styles[8].base = 2;
  styles[8].prefix_base = 1;
  styles[8].showplus = 2;

  // 9 has showbase, showplus, hex, uppercase
  styles[9].base = 16;
  styles[9].prefix_base = 1;
  styles[9].showplus = 1;
  styles[9].uppercase = 1;

  // 10 has defaults, octal
  styles[10].base = 8;
  styles[10].prefix_base = 0;

  // 11 has defaults, octal, showbase
  styles[11].base = 8;
  styles[11].prefix_base = 1;


  // Open a temporary file.
  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  for( i = 0; expect_arr[i]; i++ ) {
    for( j = 0; j < NSTYLES; j++ ) {
      const char* expect = expect_arr[i][j];
      int64_t num = nums[i];
      qio_style_t* style = &styles[j];
      int64_t got_num;

      //printf("Expect '%s' testing 0x%" PRIx64 " style %i\n", expect, num, j);

      memset(got, 0, sizeof(got));

      // Create a "write to file" channel.
      err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, style);
      assert(!err);

      err = qio_channel_print_int(true, writing, &num, 8, true);
      assert(!err);

      // write a separator so we can use the same file..
      err = qio_channel_write_amt(true, writing, sep, sizeof(sep));
      assert(!err);

      qio_channel_release(writing);
      writing = NULL;

      // Create a "read from file" channel.
      err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
      assert(!err);

      err = qio_channel_read(true, reading, got, sizeof(got), &amt_read);
      assert(qio_err_to_int(err) == EEOF);

      //printf("Got    '%s' expect '%s'\n", got, expect);
      //
      assert( 0 == strcmp(got, expect) );

      qio_channel_release(reading);
      reading = NULL;


      // Try scanning our number.
      // Create a "read from file" channel.
      err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, style);
      assert(!err);

      got_num = 0;
      err = qio_channel_scan_int(true, reading, &got_num, 8, 1);
      assert(!err);

      //printf("Got    '%s'         0x%" PRIx64 "\n", got, got_num);
      assert( got_num == num );

      qio_channel_release(reading);
      reading = NULL;
    }
  }

  // Close the file.
  qio_file_release(f);
  f = NULL;
#undef NSTYLES
}

// Sortof like (0==strcmp(got,expect)), but if expect has '*'
// then they match any character.
// returns 1 if OK match, 0 if not.
int float_result_ok(const char* got, const char* expect)
{
  int i;
  i = 0;
  for( i = 0; got[i] && expect[i]; i++ ) {
    if( got[i] == expect[i] || expect[i] == '*' ) {
      // ok
    } else return 0;
  }
  // make sure they both got to '\0' at end of string.
  if( got[i] != expect[i] ) return 0;
  return 1;
}

void test_printscan_float(void)
{
  qioerr err;
  qio_file_t* f;
  qio_channel_t* writing;
  qio_channel_t* reading;

#define NSTYLES 9
  qio_style_t styles[NSTYLES];

  const char* zero[] = { // writing 0
                        "0.0", // default style
                        "0.000", // %f precision 3
                        "0.000e+00", // %e precision 3
                        "+0.", // showpoint, showplus
                        "0x0p+0", // hex
                        "0X0.000P+0", // hex, uppercase, showpoint, prec 3
                        "0", // %g, 4 significant digits
                        "0.0000", // %f, showpoint, precision 4
                        "0.0000e+00", // %e, showpoint, precision 4
                       };

  const char* one[] = { // writing 1
                        "1.0", // default style
                        "1.000", // %f precision 3
                        "1.000e+00", // %e precision 3
                        "+1.", // showpoint, showplus
                        "0x1p+0", // hex
                        "0X1.000P+0", // hex, uppercase, showpoint, prec 3
                        "1", // %g, 4 significant digits
                        "1.0000", // %f, showpoint, precision 4
                        "1.0000e+00", // %e, showpoint, precision 4
                       };

  const char* pos[] = { // writing 11.25
                        "11.25", // default style
                        "11.250", // %f precision 3
                        "1.125e+01", // %e precision 3
                        "+11.25", // showpoint, showplus
                        "0x1.68p+3", // hex
                        "0X1.680P+3", // hex, uppercase, showpoint, prec 3
                        "11.25", // %g, 4 significant digits
                        "11.2500", // %f, showpoint, precision 4
                        "1.1250e+01", // %e, showpoint, precision 4
                       };

  const char* neg[] = { // writing -11.25
                        "-11.25", // default style
                        "-11.250", // %f precision 3
                        "-1.125e+01", // %e precision 3
                        "-11.25", // showpoint, showplus
                        "-0x1.68p+3", // hex
                        "-0X1.680P+3", // hex, uppercase, showpoint, prec 3
                        "-11.25", // %g, 4 significant digits
                        "-11.2500", // %f, showpoint, precision 4
                        "-1.1250e+01", // %e, showpoint, precision 4
                       };

  const char* plusinf[] = { // writing +infinity
                        "inf", // default style
                        "inf", // %f precision 3
                        "inf", // %e precision 3
                        "+inf", // showpoint, showplus
                        "inf", // hex
                        "INF", // hex, uppercase, showpoint, prec 3
                        "inf", // %g, 4 significant digits
                        "inf", // %f, showpoint, precision 4
                        "inf", // %e, showpoint, precision 4
                       };
  const char* minusinf[] = { // writing +infinity
                        "-inf", // default style
                        "-inf", // %f precision 3
                        "-inf", // %e precision 3
                        "-inf", // showpoint, showplus
                        "-inf", // hex
                        "-INF", // hex, uppercase, showpoint, prec 3
                        "-inf", // %g, 4 significant digits
                        "-inf", // %f, showpoint, precision 4
                        "-inf", // %e, showpoint, precision 4
                       };
  const char* nan[] = { // writing nan
                        "nan", // default style
                        "nan", // %f precision 3
                        "nan", // %e precision 3
                        "+nan", // showpoint, showplus
                        "nan", // hex
                        "NAN", // hex, uppercase, showpoint, prec 3
                        "nan", // %g, 4 significant digits
                        "nan", // %f, showpoint, precision 4
                        "nan", // %e, showpoint, precision 4
                       };
  const char* nnan[] = { // writing nan
                        "nan", // default style
                        "nan", // %f precision 3
                        "nan", // %e precision 3
                        "-nan", // showpoint, showplus
                        "nan", // hex
                        "NAN", // hex, uppercase, showpoint, prec 3
                        "nan", // %g, 4 significant digits
                        "nan", // %f, showpoint, precision 4
                        "nan", // %e, showpoint, precision 4
                       };
  const char* x[] = { // writing 1.125e+300
                        "1.125e+300", // default style
                        "1124999999999999984717009863215819639889402******************************************************************************************************************************************************************************************************************************************************************.000", // %f precision 3
                        // Note that the Cygwin result
                     // 1124999999999999984717009863215819639889402000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.000000
                        // and the Linux/Mac OS X result
                     // 1124999999999999984717009863215819639889402246251651042717325796981054812448928754462634938945285169790751774504176833459124149130131831874871128930639759966162906545922666490056516990646142904182469580674455306426256469801266735686696548991733655898546719119989659797590624855449025294035374384873472.000
                        // are actually the same (double) number
                        "1.125e+300", // %e precision 3
                        "+1.125e+300", // showpoint, showplus
                        "0x1.ae0c41900844fp+996", // hex
                        "0X1.AE1P+996", // hex, uppercase, showpoint, prec 3
                        "1.125e+300", // %g, 4 significant digits
                        "1124999999999999984717009863215819639889402******************************************************************************************************************************************************************************************************************************************************************.0000", // %f, showpoint, precision 4
                        "1.1250e+300", // %e, showpoint, precision 4
                       };
  const char* y[] = { // writing 6.125e-300,
                        "6.125e-300", // default style
                        "0.000", // %f precision 3
                        "6.125e-300", // %e precision 3
                        "+6.125e-300", // showpoint, showplus
                        "0x1.0685051469a5p-994", // hex
                        "0X1.068P-994", // hex, uppercase, showpoint, prec 3
                        "6.125e-300", // %g, 4 significant digits
                        "0.0000", // %f, showpoint, precision 4
                        "6.1250e-300", // %e, showpoint, precision 4
                       };
  const char* large[] = { // writing 1.7206679531457315e+308
                        "1.72067e+308", // default style
                        "1720667953145731543457459681945095582860664**************************************************************************************************************************************************************************************************************************************************************************.000", // %f precision 3
                        // Note that the Cygwin result
                      // 172066795314573154345745968194509558286066400000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.000
                        // and the Linux/Mac OS X result
                      // 172066795314573154345745968194509558286066410995909100775616533636325924454368801282172479282703763495131121854551604368189828377600559568197898797683089581352692291163003339166097827106946755086868842980633031878406821667478584291249430295495272835661018598744016150896290408946067667584552736715085306658816.000
                        // are actually the same (double) number.
                        "1.721e+308", // %e precision 3
                        "+1.72067e+308", // showpoint, showplus
                        "0x1.ea100001p+1023", // hex
                        "0X1.EA1P+1023", // hex, uppercase, showpoint, prec 3
                        "1.721e+308", // %g, 4 significant digits
                        "1720667953145731543457459681945095582860664**************************************************************************************************************************************************************************************************************************************************************************.0000", // %f, showpoint, precision 4
                        "1.7207e+308", // %e, showpoint, precision 4
                       };
  const char* small[] = { // writing 4.2594736637394926356874e-308
                        "4.25947e-308", // default style
                        "0.000", // %f precision 3
                        "4.259e-308", // %e precision 3
                        "+4.25947e-308", // showpoint, showplus
                        "0x1.ea100001p-1022", // hex
                        "0X1.EA1P-1022", // hex, uppercase, showpoint, prec 3
                        "4.259e-308", // %g, 4 significant digits
                        "0.0000", // %f, showpoint, precision 4
                        "4.2595e-308", // %e, showpoint, precision 4
                       };
  double mynan = NAN; // 0.0*(1.0/0.0);
  double posnan = copysign(mynan, 1.0);
  double negnan = copysign(mynan, -1.0);
  double nums[] = {0.0, 1.0, 11.25,  -11.25,
                   1.0/0.0 /*+inf*/, -1.0/0.0 /*-inf*/,
                   posnan,
                   negnan,
                   1.125e+300, 6.125e-300,
                   1.7206679531457315e+308, 4.2594736637394926356874e-308 };

  const char** expect_arr[] = { zero, one, pos, neg,
                                plusinf, minusinf, nan, nnan,
                                x,y,large,small, NULL };
  char got[500];
  char sep[4] = {0,0,0,0};
  ssize_t amt_read;
  int i,j;

  //printf("Testing text float I/O\n");

  for( i = 0; i < NSTYLES; i++ ) {
    qio_style_init_default(&styles[i]);
    styles[i].showpointzero = 0;
  }
  // Set up the styles
  // 0 is default.
  styles[0].showpointzero = 1;

  // 1 has %f, precision 3
  styles[1].precision = 3;
  styles[1].realfmt = 1;

  // 2 has %e, precision 3
  styles[2].precision = 3;
  styles[2].realfmt = 2;

  // 3 has showpoint, showplus
  styles[3].showpoint = 1;
  styles[3].showplus = 1;

  // 4 has base 16
  styles[4].base = 16;

  // 5 has base 16, uppercase
  styles[5].base = 16;
  styles[5].uppercase = 1;
  styles[5].showpoint = 1;
  styles[5].precision = 3;

  // 6 %g, 4 significant digits
  styles[6].precision = 4;
  styles[6].realfmt = 0;

  // 7 has %f showpoint, precision 4
  styles[7].showpoint = 1;
  styles[7].precision = 4;
  styles[7].realfmt = 1;

  // 7 has %e showpoint, precision 4
  styles[8].showpoint = 1;
  styles[8].precision = 4;
  styles[8].realfmt = 2;


  // Open a temporary file.
  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  for( i = 0; expect_arr[i]; i++ ) {
    for( j = 0; j < NSTYLES; j++ ) {
      const char* expect = expect_arr[i][j];
      double num = nums[i];
      qio_style_t* style = &styles[j];
      double got_num;

      //printf("Expect '%s' testing %e style %i\n", expect, num, j);

      memset(got, 0, sizeof(got));

      // Create a "write to file" channel.
      err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, style);
      assert(!err);

      err = qio_channel_print_float(true, writing, &num, 8);
      assert(!err);

      // write a separator so we can use the same file..
      err = qio_channel_write_amt(true, writing, sep, sizeof(sep));
      assert(!err);

      qio_channel_release(writing);
      writing = NULL;

      // Create a "read from file" channel.
      err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, style);
      assert(!err);

      err = qio_channel_read(true, reading, got, sizeof(got), &amt_read);
      assert(qio_err_to_int(err) == EEOF);

      if( !float_result_ok(got, expect) ) {
        fprintf(stderr, "match failed for i=%i j=%i\n", i, j);
        fprintf(stderr, "Got    '%s'\n", got);
        fprintf(stderr, "Expect '%s'\n", expect);

        assert( float_result_ok(got, expect) );
      }

      qio_channel_release(reading);
      reading = NULL;


      // Try scanning our number.
      // Create a "read from file" channel.
      {
        err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
        assert(!err);

        got_num = 0;
        err = qio_channel_scan_float(true, reading, &got_num, 8);
        assert(!err);

        if( !isfinite(num) ||
            (0.001 <= fabs(num) && fabs(num) <= 1000 ) ) {
          // Verify that the read number matches only if all of
          // our styles would print it correctly.
          //printf("Got    '%s'         %e\n", got, got_num);
          int ok = 0;
          if( got_num == num ) ok = 1;
          if( isnan(got_num) && isnan(num) ) ok = 1;
          if( isinf(got_num) && isinf(num) ) ok = 1;
          if( ! ok ) {
            fprintf(stderr, "num is %g\n", num);
            fprintf(stderr, "num is %a\n", num);
            fprintf(stderr, "read   %a\n", got_num);
            fprintf(stderr, "got    is %s\n", got);
            fprintf(stderr, "expect is %s\n", expect);
            assert( got_num == num );
          }
        }

        qio_channel_release(reading);
        reading = NULL;
      }
    }
  }

  // Close the file.
  qio_file_release(f);
  f = NULL;

  if( verbose ) printf("PASS: text float I/O\n");
#undef NSTYLES
}

void test_verybasic()
{
	qio_file_t *f = NULL;
	qio_channel_t *writing = NULL;
	qio_channel_t *reading = NULL;
	qioerr err; 
	char buf[4] = {0};
	
	//open the tmp file, create a write channel, write our data, and release the channel
  	err = qio_file_open_tmp(&f, 0, NULL);
        assert(!err);
      	err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, NULL);
        assert(!err);
      	err = qio_channel_write_amt(true, writing, "\xDE\xAD\xBE\xEF", 4);
        assert(!err);
        qio_channel_release(writing);

	//open a read channel to the tmp file, and read back the data.
    	err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
        assert(!err);
      	err = qio_channel_read_amt(true, reading, buf, 4);
        assert(!err);
        qio_channel_release(reading);

	///check that what we wrote is what we read back
	if(memcmp(buf, "\xDE\xAD\xBE\xEF", 4) != 0){ assert(0);}

	//close the file
	qio_file_release(f);
	f = NULL;

	if( verbose ) printf("PASS: test_verybasic\n");
}

typedef struct string_test_s{
	char *string;
	int length;
}string_test_t;

void string_escape_tests()
{
	qio_style_t style = qio_style_default();
	qio_channel_t *reading;
	qio_channel_t *writing;
	qio_file_t *f = NULL;
	qioerr err;
        const char* inputs[2][4] = { {"a \"\\b\x01", // original string
                                      "\"a \\\"\\\\b\x01\"", // simple encoding
                                      "\"a \\\"\\\\b\\x01\"", // chapel encoding
                                      "\"a \\\"\\\\b\\u0001\"", // JSON encoding
                                     },
                                     { NULL, NULL, NULL, NULL }
                                   };
	char buf[50] = {0};
        int i,j;

	style.binary=0;
	style.string_start = '"';
	style.string_end = '"';
	
	err = qio_file_open_tmp(&f, 0, NULL);
        assert(!err);
        
        for( i = 0; inputs[i][0]; i++ ) {
          const char* input = inputs[i][0];
          ssize_t input_len = strlen(input);
          for( j = 1; j < 4; j++ ) {
            const char* expect = inputs[i][j];
            ssize_t expect_len = strlen(expect);
            style.string_format = j;

            err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, &style);
            assert(!err);
            err = qio_channel_print_string(true, writing, input, input_len);	
            assert(!err);
            qio_channel_release(writing);

            err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, &style);
            assert(!err);
            err = qio_channel_read_amt(true, reading, buf, expect_len);
            assert(!err);
     
            qio_channel_release(reading);

            //printf("Got %s expect %s\n", buf, expect);
            assert( memcmp(buf, expect, expect_len) == 0 );

            // Check that we can read it in again.
            err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, &style);
            assert(!err);

            {
              const char* got = NULL;
              int64_t got_len = 0;
              err = qio_channel_scan_string(true, reading, &got, &got_len, -1);
              assert(!err);

              //printf("Read back %s expect %s\n", got, input);
              assert( got_len == input_len );
              assert( memcmp(got, input, got_len) == 0 );

              free((void*) got);
            }
     
            qio_channel_release(reading);
          }
        }

        qio_file_release(f);

        if( verbose ) printf("PASS: simple escape\n");
}

void write_65k_test()
{
        const char *out = NULL;
	char *p = NULL;
        qioerr err;
        qio_file_t *f = NULL;
        qio_channel_t *reading = NULL;
        qio_channel_t *writing = NULL;
        qio_style_t style = qio_style_default();
	int buflen = 65535;
        int64_t out_len = 0;

        // skip this test for very small buf sizes on 32-bit platforms
        // because we might run out of memory.
        if( qbytes_iobuf_size < 8 && sizeof(void*) < 8 ) return;

        err = qio_file_open_tmp(&f, 0, NULL);
        assert(!err);

        style.binary = 1;
        style.byteorder = QIO_BIG;
        style.str_style = -2;

        err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, &style);
        assert(!err);

	p = (char *)calloc(1, buflen);
	if(!p){ assert(0); }
	memset(p, 'A', buflen);

        err = qio_channel_write_string(true, style.byteorder, style.str_style, writing, p, buflen);
        assert(!err);
        qio_channel_release(writing);
        assert(!err);

        err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
        assert(!err);

	while(1){
        	err = qio_channel_read_string(true, style.byteorder, style.str_style, reading, &out, &out_len, -1);
		if(out){
			if(memcmp(out, p, buflen)!=0){
				assert(0);
			}
			free((void*) out); out=NULL;
		}
		if(err>0){ break; }
	}

        qio_channel_release(reading);

	if( verbose ) printf("PASS: wrote 65k to file, read it back ok.\n");

        qio_file_release(f);
        f = NULL;

	free(p);
}

/**
 *   
 **/
void max_width_test()
{
        const char *out = NULL;
        int64_t out_len = 0;
        qioerr err;
        qio_file_t *f = NULL;
        qio_channel_t *reading = NULL;
        qio_channel_t *writing = NULL;
        qio_style_t style = qio_style_default();

        err = qio_file_open_tmp(&f, 0, NULL);
        assert(!err);

        style.binary = 0;
        style.string_format = 1;
        style.max_width_columns = 5;


        err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, &style);
        assert(!err);

        err = qio_channel_print_string(true, writing, "helloworld", 10);
        assert(!err);
        qio_channel_release(writing);
        assert(!err);

        err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
        assert(!err);
        err = qio_channel_scan_string(true, reading, &out, &out_len, -1);
        assert(!err);
        qio_channel_release(reading);
        assert(!err);

        if(strlen(out) != 5){
                printf("FAIL: min_width expecting 11 chars for '%s'\n", out);
                assert(0);
        }

        free((void*) out);

        qio_file_release(f);
        f = NULL;
}

/**
 * Tests the min_width option.
 * by writing 10 byte helloworld string but specifying 
 * min_width = 11 so the read should return 11 bytes where
 * the 11th byte would be a space character.
 */
void min_width_test()
{
        const char *out = NULL;
        int64_t out_len = 0;
        qioerr err;
        qio_file_t *f = NULL;
        qio_channel_t *reading = NULL;
        qio_channel_t *writing = NULL;
	qio_style_t style = qio_style_default();

        err = qio_file_open_tmp(&f, 0, NULL);
        assert(!err);

	style.binary = 0;
	style.string_format = 1;
	style.min_width_columns = 11;

        err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, &style);
        assert(!err);


        err = qio_channel_print_string(true, writing, "helloworld", 10);
        assert(!err);
        qio_channel_release(writing);

        err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
        assert(!err);
        err = qio_channel_scan_string(true, reading, &out, &out_len, -1);
        assert(!err);
        qio_channel_release(reading);

	if(strlen(out) != 11){
		printf("FAIL: min_width expecting 11 chars for '%s'\n", out);
		assert(0);
	}

        free((void*) out);

        qio_file_release(f);
        f = NULL;
}

/**
 * Test basic ascii functionality
 */
void basicstring_test()
{ 
 	const char *out = NULL;
        int64_t out_len = 0;
        qioerr err;
	int x,y=0;
        qio_file_t *f = NULL;
        qio_channel_t *reading = NULL;
        qio_channel_t *writing = NULL;

	#define NUM_STR_STYLES 12
        qio_style_t styles[NUM_STR_STYLES];

	string_test_t strings[] = { 	
				    { "", 0 },
				    { "a", 1 },
				    { "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA", 255},
				  }; 
        int NUM_STRINGS = sizeof(strings)/sizeof(string_test_t);

        if( verbose ) printf("Testing basic string writing/reading\n");

        err = qio_file_open_tmp(&f, 0, NULL);
	assert(!err);

        for(x=0;x<NUM_STR_STYLES;x++) {
          qio_style_init_default(&styles[x]);
        }
        // Set up the styles
        // 0 is default.
        styles[0].binary = 1;
        styles[0].str_style = -1;

        // binary styles.
        styles[1].binary = 1;
        styles[1].str_style = -1;
        styles[2].binary = 1;
        styles[2].str_style = -2;
        styles[3].binary = 1;
        styles[3].str_style = -4;
        styles[4].binary = 1;
        styles[4].str_style = -8;
        styles[5].binary = 1;
        styles[5].str_style = -10;
        styles[6].binary = 1;
        styles[6].str_style = -0x0100; // zero-terminated.
        styles[7].binary = 1;
        styles[7].str_style = -0x01ff;
        styles[8].binary = 1;
        styles[8].str_style = 0xffff; // updated below for STYLE 8!

        styles[9].string_format = QIO_STRING_FORMAT_BASIC;
        styles[9].string_start = '|';
        styles[9].string_end = '+';
        styles[10].string_format = QIO_STRING_FORMAT_CHPL;
        styles[11].string_format = QIO_STRING_FORMAT_JSON;

	for(x=0;x<NUM_STR_STYLES;x++){
          qio_style_t* style = &styles[x];
		for(y=0;y<NUM_STRINGS;y++){
                  string_test_t* string = &strings[y];

                        // make style 8 always use the string length!
                        if( x == 8 ) style->str_style = string->length;

        		err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, style);
			assert(!err);

                        //printf("Writing string '%s' with style %i\n", string->string, x);

                        if( style->binary ) 
                          err = qio_channel_write_string(true, style->byteorder, style->str_style, writing, string->string, string->length);
                        else
                          err = qio_channel_print_string(true, writing, string->string, string->length);

			assert(!err);
        		qio_channel_release(writing);

			err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, style);
			assert(!err);

                        //printf("Reading string '%s' with style %i\n", string->string, x);
                        if( style->binary ) 
                          err = qio_channel_read_string(true, style->byteorder, style->str_style, reading, &out, &out_len, -1);
                        else
                          err = qio_channel_scan_string(true, reading, &out, &out_len, -1);
			assert(!err);
			qio_channel_release(reading);

                        //printf("Got '%s' expect '%s'\n", out, string->string);

			if(memcmp(out, string->string, string->length) != 0){
				printf("FAIL: style %d, string='%s'\n", x, string->string);
				free((void*) out);
				assert(0);
			}

			//printf("PASS: style %d\n", x);
			if(out){ free((void*) out); out=NULL; }
		}
	}

        qio_file_release(f);
        f = NULL;
       
        if( verbose ) printf("PASS: basic string test\n");
#undef NUM_STR_STYLES
#define NUM_STRINGS 1
}


void test_readwritestring()
{
	//basicstring_test(); in main.
	write_65k_test();   //write 65k then read it back.

	//min_width_test();  //test min_width              
	//max_width_test();  //test max_width
	
        string_escape_tests();
}

void test_scanmatch()
{
  qioerr err;
  qio_file_t *f = NULL;
  qio_channel_t *reading = NULL;
  qio_channel_t *writing = NULL;

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_write_char(true, writing, ' ');
  assert(!err);
  err = qio_channel_write_char(true, writing, 'm');
  assert(!err);
  err = qio_channel_write_char(true, writing, 'a');
  assert(!err);
  err = qio_channel_write_char(true, writing, 't');
  assert(!err);
  err = qio_channel_write_char(true, writing, 'c');
  assert(!err);
  err = qio_channel_write_char(true, writing, 'h');
  assert(!err);


  qio_channel_release(writing);

  err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_scan_literal(true, reading, "test", 4, 1);
  assert(qio_err_to_int(err) == EFORMAT);
  err = qio_channel_scan_literal(true, reading, "match", 5, 1);
  assert(err == 0);
  err = qio_channel_scan_literal(true, reading, "match", 5, 1);
  assert(qio_err_to_int(err) == EEOF);

  qio_channel_release(reading);

  err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);
  err = qio_channel_scan_literal(true, reading, " match", 5, 1);
  assert(err == 0);

  qio_channel_release(reading);
 
  qio_file_release(f);
  f = NULL;

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_write_char(true, writing, 'a');
  assert(!err);
  err = qio_channel_write_char(true, writing, ' ');
  assert(!err);
  err = qio_channel_write_char(true, writing, 'b');
  assert(!err);

  qio_channel_release(writing);

  err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_scan_literal(true, reading, "ab", 2, 1);
  assert(qio_err_to_int(err) == EFORMAT);
  err = qio_channel_scan_literal(true, reading, "a", 1, 1);
  assert(err == 0);
  err = qio_channel_scan_literal(true, reading, " ", 1, 1);
  assert(err == 0);
  err = qio_channel_scan_literal(true, reading, "b", 1, 1);
  assert(err == 0);
  err = qio_channel_scan_literal(true, reading, "a", 1, 1);
  assert(qio_err_to_int(err) == EEOF);

  qio_channel_release(reading);

  err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_scan_literal(true, reading, "ab", 2, 1);
  assert(qio_err_to_int(err) == EFORMAT);
  err = qio_channel_scan_literal(true, reading, "a ", 2, 1);
  assert(err == 0);
  err = qio_channel_scan_literal(true, reading, "b", 1, 1);
  assert(err == 0);
  err = qio_channel_scan_literal(true, reading, "a", 1, 1);
  assert(qio_err_to_int(err) == EEOF);

  qio_channel_release(reading);


  qio_file_release(f);
  f = NULL;
}

void do_test_utf8(int wchar, char* utf8)
{
  qioerr err;
  qio_file_t *f = NULL;
  qio_channel_t *reading = NULL;
  qio_channel_t *writing = NULL;
  int32_t got_wchar;
  int i;
  int len;

  len = strlen(utf8);
  if( len == 0 && wchar == 0 ) len = 1; // read 1-byte null

  //printf("Testing character %x len %i \n", wchar, len);

  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_write_char(true, writing, wchar);
  assert(!err);

  qio_channel_release(writing);

  err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  for( i = 0; i < len; i++ ) {
   got_wchar = qio_channel_read_byte(true, reading);
   //printf("Read byte %x\n", got_wchar);
   assert(got_wchar == (unsigned char) utf8[i]);
  }
  assert( -EEOF == qio_channel_read_byte(true, reading) );

  qio_channel_release(reading);

  err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, NULL);
  assert(!err);

  err = qio_channel_read_char(true, reading, &got_wchar);
  assert(!err);

  assert(got_wchar == wchar);

  qio_channel_release(reading);
 
  qio_file_release(f);
  f = NULL;

}

void test_utf8(void)
{
  if( verbose )
    printf("Testing UTF-8 glocale_utf8=%i qbytes_iobuf_size=%i\n",
         qio_glocale_utf8, (int) qbytes_iobuf_size);

  do_test_utf8(0x0024, "\x24");
  do_test_utf8(0x00A2, "\xC2\xA2");
  do_test_utf8(0x20AC, "\xE2\x82\xAC");
  do_test_utf8(0x024B62, "\xF0\xA4\xAD\xA2");

  // from Unicode manual D92 UTF-8
  do_test_utf8(0x4D, "\x4D");
  do_test_utf8(0x430, "\xD0\xB0");
  do_test_utf8(0x4E8C, "\xE4\xBA\x8C");
  do_test_utf8(0x10302, "\xF0\x90\x8C\x82");

  do_test_utf8(0x0000, "\x00");
  do_test_utf8(0x007F, "\x7F");
  do_test_utf8(0x0080, "\xC2\x80");
  do_test_utf8(0x07FF, "\xDF\xBF");
  do_test_utf8(0x0800, "\xE0\xA0\x80");
  do_test_utf8(0x0FFF, "\xE0\xBF\xBF");
  do_test_utf8(0x1000, "\xE1\x80\x80");
  do_test_utf8(0xCFFF, "\xEC\xBF\xBF");
  do_test_utf8(0xD000, "\xED\x80\x80");
  do_test_utf8(0xD7FF, "\xED\x9F\xBF");
  do_test_utf8(0xE000, "\xEE\x80\x80");
  do_test_utf8(0xFFFF, "\xEF\xBF\xBF");
  do_test_utf8(0x010000, "\xF0\x90\x80\x80");
  do_test_utf8(0x03FFFF, "\xF0\xBF\xBF\xBF");
  do_test_utf8(0x040000, "\xF1\x80\x80\x80");
  do_test_utf8(0x0FFFFF, "\xF3\xBF\xBF\xBF");
  do_test_utf8(0x100000, "\xF4\x80\x80\x80");
  do_test_utf8(0x10FFFF, "\xF4\x8F\xBF\xBF");
}

void test_quoted_string_maxlength(void)
{
  qio_style_t style = qio_style_default();
  qio_channel_t *reading;
  qio_channel_t *writing;
  qio_file_t *f = NULL;
  qioerr err;
  const char* inputs[8][16] = {
    { "",     // original string
      "\"\"", // maxlen = 1
      "\"\"", // maxlen = 2
      "\"\"", // maxlen = 3
      "\"\"", // maxlen = 4
      "\"\"", // maxlen = 5
      "\"\"", // maxlen = 6
      "\"\"", // maxlen = 7
      "\"\"", // maxlen = 8
      "\"\"", // maxlen = 9
      "\"\"", // maxlen = 10
      "\"\"", // maxlen = 11
      "\"\"", // maxlen = 12
      "\"\"", // maxlen = 13
      "\"\"", // maxlen = 14
      NULL
    },
    { "a",     // original string
      "\"a\"", // maxlen = 1
      "\"a\"", // maxlen = 2
      "\"a\"", // maxlen = 3
      "\"a\"", // maxlen = 4
      "\"a\"", // maxlen = 5
      "\"a\"", // maxlen = 6
      "\"a\"", // maxlen = 7
      "\"a\"", // maxlen = 8
      "\"a\"", // maxlen = 9
      "\"a\"", // maxlen = 10
      "\"a\"", // maxlen = 11
      "\"a\"", // maxlen = 12
      "\"a\"", // maxlen = 13
      "\"a\"", // maxlen = 14
      NULL
    },
    { "ab",     // original string
      "\"ab\"", // maxlen = 1
      "\"ab\"", // maxlen = 2
      "\"ab\"", // maxlen = 3
      "\"ab\"", // maxlen = 4
      "\"ab\"", // maxlen = 5
      "\"ab\"", // maxlen = 6
      "\"ab\"", // maxlen = 7
      "\"ab\"", // maxlen = 8
      "\"ab\"", // maxlen = 9
      "\"ab\"", // maxlen = 10
      "\"ab\"", // maxlen = 11
      "\"ab\"", // maxlen = 12
      "\"ab\"", // maxlen = 13
      "\"ab\"", // maxlen = 14
      NULL
    },
    { "abc",     // original string
      "\"abc\"", // maxlen = 1
      "\"abc\"", // maxlen = 2
      "\"abc\"", // maxlen = 3
      "\"abc\"", // maxlen = 4
      "\"abc\"", // maxlen = 5
      "\"abc\"", // maxlen = 6
      "\"abc\"", // maxlen = 7
      "\"abc\"", // maxlen = 8
      "\"abc\"", // maxlen = 9
      "\"abc\"", // maxlen = 10
      "\"abc\"", // maxlen = 11
      "\"abc\"", // maxlen = 12
      "\"abc\"", // maxlen = 13
      "\"abc\"", // maxlen = 14
      NULL
    },
    { "abcd",     // original string
      "\"\"...", // maxlen = 1
      "\"\"...", // maxlen = 2
      "\"\"...", // maxlen = 3
      "\"\"...", // maxlen = 4
      "\"\"...", // maxlen = 5
      "\"abcd\"", // maxlen = 6
      "\"abcd\"", // maxlen = 7
      "\"abcd\"", // maxlen = 8
      "\"abcd\"", // maxlen = 9
      "\"abcd\"", // maxlen = 10
      "\"abcd\"", // maxlen = 11
      "\"abcd\"", // maxlen = 12
      "\"abcd\"", // maxlen = 13
      "\"abcd\"", // maxlen = 14
      NULL
    },

    { "123456789",     // original string
      "\"\"...",       // maxlen = 1
      "\"\"...",       // maxlen = 2
      "\"\"...",       // maxlen = 3
      "\"\"...",       // maxlen = 4
      "\"\"...",       // maxlen = 5
      "\"1\"...",      // maxlen = 6
      "\"12\"...",     // maxlen = 7
      "\"123\"...",    // maxlen = 8
      "\"1234\"...",   // maxlen = 9
      "\"12345\"...",  // maxlen = 10
      "\"123456789\"", // maxlen = 11
      "\"123456789\"", // maxlen = 12
      "\"123456789\"", // maxlen = 13
      "\"123456789\"", // maxlen = 14
      NULL
    },

    { "\x01X\x01Y", // original string
      "\"\"...",       // maxlen = 1
      "\"\"...",       // maxlen = 2
      "\"\"...",       // maxlen = 3
      "\"\"...",       // maxlen = 4
      "\"\"...",       // maxlen = 5
      "\"\"...",       // maxlen = 6
      "\"\"...",       // maxlen = 7
      "\"\"...",       // maxlen = 8
      "\"\\x01\"...",  // maxlen = 9
      "\"\\x01X\"...", // maxlen = 10
      "\"\\x01X\"...", // maxlen = 11
      "\"\\x01X\\x01Y\"", // maxlen = 12
      "\"\\x01X\\x01Y\"", // maxlen = 13
      "\"\\x01X\\x01Y\"", // maxlen = 14
      NULL
    },
    { NULL, NULL, NULL, NULL,
      NULL, NULL, NULL, NULL,
      NULL, NULL, NULL, NULL,
      NULL, NULL, NULL, NULL,
    }
  };

  char buf[50] = {0};
  int i,j;

  style.binary=0;
  style.string_start = '"';
  style.string_end = '"';
  
  err = qio_file_open_tmp(&f, 0, NULL);
  assert(!err);

  for( i = 0; inputs[i][0]; i++ ) {
    const char* input = inputs[i][0];
    ssize_t input_len = strlen(input);
    for( j = 1; inputs[i][j]; j++ ) {
      const char* expect = inputs[i][j];
      ssize_t expect_len = strlen(expect);
      qio_truncate_info_t ti;
      const char* got = NULL;
      ssize_t truncate_len = (j<input_len)?(j):(input_len);
      style.string_format = QIO_STRING_FORMAT_CHPL;
      style.max_width_columns = j;

      ti.max_columns = style.max_width_columns;
      ti.max_chars = SSIZE_MAX;
      ti.max_bytes = SSIZE_MAX;
      
      // check that qio_quote_string gives correct string when
      // used in a no-quote mode
      if( i != 6 ) {
        // but not for test 6 since the control characters are weird.
        err = qio_quote_string(0, 0, QIO_STRING_FORMAT_WORD, input, input_len, &got, &ti);
        assert(!err);
        assert(ti.ret_columns == truncate_len);
        assert(ti.ret_chars == truncate_len);
        assert(truncate_len == strlen(got));
        assert( 0 == memcmp(got, input, truncate_len) );
        free((void*) got);
      }

      // Now, check that qio_quote_string returns the correct string.
      err = qio_quote_string(style.string_start, style.string_end, style.string_format, input, input_len, &got, &ti);
      assert(!err);

      assert(ti.ret_columns == strlen(inputs[i][1]) || ti.ret_columns <= j);
      assert(ti.ret_bytes == expect_len);
      assert(ti.ret_bytes == strlen(got));
      assert( 0 == strcmp(got, expect) );
      free((void*) got);

      // Now, check that the quoting works correctly when writing.

      err = qio_channel_create(&writing, f, QIO_CH_BUFFERED, 0, 1, 0, INT64_MAX, &style);
      assert(!err);
      err = qio_channel_print_string(true, writing, input, input_len);	
      assert(!err);
      qio_channel_release(writing);

      err = qio_channel_create(&reading, f, QIO_CH_BUFFERED, 1, 0, 0, INT64_MAX, &style);
      assert(!err);
      err = qio_channel_read_amt(true, reading, buf, expect_len);
      assert(!err);

      qio_channel_release(reading);

      //printf("Got %s expect %s\n", buf, expect);
      assert( memcmp(buf, expect, expect_len) == 0 );
    }
  }

  qio_file_release(f);

  if( verbose ) printf("PASS: quoted max length\n");
}

int main(int argc, char** argv)
{
  int sizes[] = {qbytes_iobuf_size, 64, 1, 2, 0};

  setlocale(LC_CTYPE,"");

  if( verbose ) {
    printf("Sizeof of qio_style_t is %i\n", (int) sizeof(qio_style_t));
    printf("Sizeof of qio_channel_t is %i\n", (int) sizeof(qio_channel_t));
  }

  for( int i = 0; sizes[i] != 0; i++ ) {
    char* codeset = nl_langinfo(CODESET); 
    qbytes_iobuf_size = sizes[i];

    if( 0 == strcmp(codeset, "UTF-8") ) {
      qio_glocale_utf8 = -1;
      test_utf8();
      qio_glocale_utf8 = 1;
      test_utf8();
    }
  }

  for( int i = 0; sizes[i] != 0; i++ ) {
    qbytes_iobuf_size = sizes[i];

    if( verbose ) printf("Testing formatted I/O qbytes_iobuf_size=%i\n",
                         (int) qbytes_iobuf_size);
    basicstring_test();

    test_verybasic();
    test_readwriteint();
    test_endian();
    test_printscan_int();
    test_printscan_float();

    test_readwritestring();

    test_scanmatch();

    test_quoted_string_maxlength();
  }

  printf("qio_formatted_test PASS\n");

  return 0;
}

