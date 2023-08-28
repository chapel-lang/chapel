#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl__init_ChapelIO(int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (chpl__init_ChapelIO_p) /* ZLINE: 134 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _exit_chpl__init_ChapelIO_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  printModuleInit_chpl("%*s\n", "ChapelIO", INT64(8), _ln_chpl, _fn_chpl);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl__init_ChapelIO_p = UINT8(true);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl__init_ChapelBase(_ln_chpl, _fn_chpl);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl__init_ChapelLocale(_ln_chpl, _fn_chpl);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl__init_IO(_ln_chpl, _fn_chpl);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _exit_chpl__init_ChapelIO_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:693 */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeThis_chpl2(_tuple_3_star_int64_t_chpl * this_chpl31,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            fileWriter_F_nothing_chpl * f_chpl,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            Error_chpl * error_out_chpl,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            int64_t _ln_chpl,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            int32_t _fn_chpl) {
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _readWriteHelper_chpl(this_chpl31, f_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 694 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endwriteThis_chpl;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endwriteThis_chpl:;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:693 */
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeThis_chpl(_tuple_4_star_int64_t_chpl * this_chpl31,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           fileWriter_F_nothing_chpl * f_chpl,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           Error_chpl * error_out_chpl,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           int64_t _ln_chpl,
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           int32_t _fn_chpl) {
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _readWriteHelper_chpl2(this_chpl31, f_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 694 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endwriteThis_chpl;
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 694 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endwriteThis_chpl:;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 693 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:699 */
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void _readWriteHelper_chpl(_tuple_3_star_int64_t_chpl * this_chpl31,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                  fileWriter_F_nothing_chpl * f_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                  Error_chpl * error_out_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                  int64_t _ln_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                  int32_t _fn_chpl) {
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int64_t call_tmp_chpl6;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_bool call_tmp_chpl7;
#line 702 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_bool call_tmp_chpl8;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl call_tmp_chpl9;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_tmp_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x0_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x1_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x2_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x3_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl10 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl start_chpl2;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl11 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl comma_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl12 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl comma1tup_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl13 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl end_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl3 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl4 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl14 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl2 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl5 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl6 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl7 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl8 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl3 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl9 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl10 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl11 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl12 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl15 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl4 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl13 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl14 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl15 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl16 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl5 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl20 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl17 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl18 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl19 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl20 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl16 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl6 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl21 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl22 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl23 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl24 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl21 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl22 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl23 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl24 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl7 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl25 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl26 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl27 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl28 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl25 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl26 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl27 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl28 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl29 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl30 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl31 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl32 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl29 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl30 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl31 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl32 = NULL;
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = styleElement_chpl(f_chpl, QIO_STYLE_ELEMENT_TUPLE, _ln_chpl, _fn_chpl);
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 == QIO_TUPLE_FORMAT_JSON);
#line 702 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl8 = binary_chpl(f_chpl, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  getLiterals_chpl2(call_tmp_chpl6, call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(3)) = *(ret_tmp_chpl + INT64(3));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x0_chpl = *(call_tmp_chpl9 + INT64(0));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x1_chpl = *(call_tmp_chpl9 + INT64(1));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x2_chpl = *(call_tmp_chpl9 + INT64(2));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x3_chpl = *(call_tmp_chpl9 + INT64(3));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl10 = &tmp_x0_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&start_chpl2, call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl11 = &tmp_x1_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&comma_chpl, call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl12 = &tmp_x2_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&comma1tup_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl13 = &tmp_x3_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&end_chpl, call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 728 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl(&start_chpl2, f_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl != nil) /* ZLINE: 729 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl = &end_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl2 = &comma1tup_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl3 = &comma_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl4 = &start_chpl2;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl = &tmp_x3_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl2 = &tmp_x2_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl3 = &tmp_x1_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl3, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl4 = &tmp_x0_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl4, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl14 = (*(this_chpl31) + INT64(0));
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl2 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl(call_tmp_chpl14, f_chpl, &error_chpl2, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 732 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl2;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl5 = &end_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl6 = &comma1tup_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl7 = &comma_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl8 = &start_chpl2;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl5 = &tmp_x3_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl5, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl6 = &tmp_x2_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl6, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl7 = &tmp_x1_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl7, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl8 = &tmp_x0_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl8, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 734 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl3 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl(&comma_chpl, f_chpl, &error_chpl3, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl3 != nil) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl3;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl9 = &end_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl10 = &comma1tup_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl11 = &comma_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl12 = &start_chpl2;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl9 = &tmp_x3_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl9, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl10 = &tmp_x2_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl10, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl11 = &tmp_x1_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl11, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl12 = &tmp_x0_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl12, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl15 = (*(this_chpl31) + INT64(1));
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl4 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl(call_tmp_chpl15, f_chpl, &error_chpl4, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl4 != nil) /* ZLINE: 737 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl4;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl13 = &end_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl14 = &comma1tup_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl15 = &comma_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl16 = &start_chpl2;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl13 = &tmp_x3_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl13, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl14 = &tmp_x2_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl14, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl15 = &tmp_x1_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl15, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl16 = &tmp_x0_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl16, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 734 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl5 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl(&comma_chpl, f_chpl, &error_chpl5, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl5 != nil) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl5;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl17 = &end_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl18 = &comma1tup_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl19 = &comma_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl20 = &start_chpl2;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl20, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl17 = &tmp_x3_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl17, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl18 = &tmp_x2_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl18, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl19 = &tmp_x1_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl19, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl20 = &tmp_x0_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl20, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl16 = (*(this_chpl31) + INT64(2));
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl6 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl(call_tmp_chpl16, f_chpl, &error_chpl6, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl6 != nil) /* ZLINE: 737 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl6;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl21 = &end_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl21, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl22 = &comma1tup_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl22, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl23 = &comma_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl23, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl24 = &start_chpl2;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl24, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl21 = &tmp_x3_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl21, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl22 = &tmp_x2_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl22, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl23 = &tmp_x1_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl23, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl24 = &tmp_x0_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl24, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 746 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 746 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 746 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl7 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl(&end_chpl, f_chpl, &error_chpl7, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl7 != nil) /* ZLINE: 747 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl7;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl25 = &end_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl25, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl26 = &comma1tup_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl26, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl27 = &comma_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl27, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl28 = &start_chpl2;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl28, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl25 = &tmp_x3_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl25, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl26 = &tmp_x2_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl26, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl27 = &tmp_x1_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl27, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl28 = &tmp_x0_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl28, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl29 = &end_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl29, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl30 = &comma1tup_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl30, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl31 = &comma_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl31, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl32 = &start_chpl2;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl32, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl29 = &tmp_x3_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl29, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl30 = &tmp_x2_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl30, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl31 = &tmp_x1_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl31, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl32 = &tmp_x0_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl32, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _end_readWriteHelper_chpl:;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:699 */
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void _readWriteHelper_chpl2(_tuple_4_star_int64_t_chpl * this_chpl31,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   fileWriter_F_nothing_chpl * f_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   Error_chpl * error_out_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   int64_t _ln_chpl,
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   int32_t _fn_chpl) {
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int64_t call_tmp_chpl6;
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_bool call_tmp_chpl7;
#line 702 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_bool call_tmp_chpl8;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl call_tmp_chpl9;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_tmp_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x0_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x1_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x2_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_x3_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl10 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl start_chpl2;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl11 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl comma_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl12 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl comma1tup_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl call_tmp_chpl13 = NULL;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl end_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl4 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl3 = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl4 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl14 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl2 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl5 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl6 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl7 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl8 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl5 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl6 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl7 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl8 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl3 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl9 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl10 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl11 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl12 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl9 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl10 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl11 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl12 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl15 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl4 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl13 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl14 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl15 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl16 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl13 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl14 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl15 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl16 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl5 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl17 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl18 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl19 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl20 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl17 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl18 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl19 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl20 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl16 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl6 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl21 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl22 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl23 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl24 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl21 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl22 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl23 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl24 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl7 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl25 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl26 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl27 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl28 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl25 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl26 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl27 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl28 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_int64_t_chpl call_tmp_chpl17 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl8 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl29 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl30 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl31 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl32 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl29 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl30 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl31 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl32 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl9 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl33 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl34 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl35 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl36 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl33 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl34 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl35 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl36 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl37 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl38 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl39 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i_x_chpl40 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl37 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl38 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl39 = NULL;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl40 = NULL;
#line 700 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = styleElement_chpl(f_chpl, QIO_STYLE_ELEMENT_TUPLE, _ln_chpl, _fn_chpl);
#line 701 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl7 = (call_tmp_chpl6 == QIO_TUPLE_FORMAT_JSON);
#line 702 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl8 = binary_chpl(f_chpl, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  getLiterals_chpl(call_tmp_chpl6, call_tmp_chpl7, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(0)) = *(ret_tmp_chpl + INT64(0));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(1)) = *(ret_tmp_chpl + INT64(1));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(2)) = *(ret_tmp_chpl + INT64(2));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(call_tmp_chpl9 + INT64(3)) = *(ret_tmp_chpl + INT64(3));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x0_chpl = *(call_tmp_chpl9 + INT64(0));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x1_chpl = *(call_tmp_chpl9 + INT64(1));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x2_chpl = *(call_tmp_chpl9 + INT64(2));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  tmp_x3_chpl = *(call_tmp_chpl9 + INT64(3));
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl10 = &tmp_x0_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&start_chpl2, call_tmp_chpl10, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl11 = &tmp_x1_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&comma_chpl, call_tmp_chpl11, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl12 = &tmp_x2_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&comma1tup_chpl, call_tmp_chpl12, _ln_chpl, _fn_chpl);
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl13 = &tmp_x3_chpl;
#line 719 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&end_chpl, call_tmp_chpl13, _ln_chpl, _fn_chpl);
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 728 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 728 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = NULL;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl2(&start_chpl2, f_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl != nil) /* ZLINE: 729 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl = &end_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl2 = &comma1tup_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl3 = &comma_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl4 = &start_chpl2;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl4, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl = &tmp_x3_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl2 = &tmp_x2_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl3 = &tmp_x1_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl3, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl4 = &tmp_x0_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl4, _ln_chpl, _fn_chpl);
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 729 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl14 = (*(this_chpl31) + INT64(0));
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl2 = NULL;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl2(call_tmp_chpl14, f_chpl, &error_chpl2, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 732 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl2;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl5 = &end_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl5, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl6 = &comma1tup_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl6, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl7 = &comma_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl7, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl8 = &start_chpl2;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl8, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl5 = &tmp_x3_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl5, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl6 = &tmp_x2_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl6, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl7 = &tmp_x1_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl7, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl8 = &tmp_x0_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl8, _ln_chpl, _fn_chpl);
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 732 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 734 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl3 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl2(&comma_chpl, f_chpl, &error_chpl3, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl3 != nil) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl3;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl9 = &end_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl9, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl10 = &comma1tup_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl10, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl11 = &comma_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl11, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl12 = &start_chpl2;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl12, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl9 = &tmp_x3_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl9, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl10 = &tmp_x2_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl10, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl11 = &tmp_x1_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl11, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl12 = &tmp_x0_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl12, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl15 = (*(this_chpl31) + INT64(1));
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl4 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl2(call_tmp_chpl15, f_chpl, &error_chpl4, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl4 != nil) /* ZLINE: 737 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl4;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl13 = &end_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl13, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl14 = &comma1tup_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl14, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl15 = &comma_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl15, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl16 = &start_chpl2;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl16, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl13 = &tmp_x3_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl13, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl14 = &tmp_x2_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl14, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl15 = &tmp_x1_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl15, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl16 = &tmp_x0_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl16, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 734 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl5 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl2(&comma_chpl, f_chpl, &error_chpl5, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl5 != nil) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl5;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl17 = &end_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl17, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl18 = &comma1tup_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl18, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl19 = &comma_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl19, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl20 = &start_chpl2;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl20, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl17 = &tmp_x3_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl17, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl18 = &tmp_x2_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl18, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl19 = &tmp_x1_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl19, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl20 = &tmp_x0_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl20, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl16 = (*(this_chpl31) + INT64(2));
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl6 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl2(call_tmp_chpl16, f_chpl, &error_chpl6, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl6 != nil) /* ZLINE: 737 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl6;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl21 = &end_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl21, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl22 = &comma1tup_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl22, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl23 = &comma_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl23, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl24 = &start_chpl2;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl24, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl21 = &tmp_x3_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl21, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl22 = &tmp_x2_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl22, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl23 = &tmp_x1_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl23, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl24 = &tmp_x0_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl24, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 734 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 734 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl7 = NULL;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl2(&comma_chpl, f_chpl, &error_chpl7, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl7 != nil) /* ZLINE: 735 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl7;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl25 = &end_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl25, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl26 = &comma1tup_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl26, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl27 = &comma_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl27, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl28 = &start_chpl2;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl28, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl25 = &tmp_x3_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl25, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl26 = &tmp_x2_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl26, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl27 = &tmp_x1_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl27, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl28 = &tmp_x0_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl28, _ln_chpl, _fn_chpl);
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 735 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl17 = (*(this_chpl31) + INT64(3));
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl8 = NULL;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  helper_chpl2(call_tmp_chpl17, f_chpl, &error_chpl8, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl8 != nil) /* ZLINE: 737 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl8;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl29 = &end_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl29, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl30 = &comma1tup_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl30, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl31 = &comma_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl31, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl32 = &start_chpl2;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(i_x_chpl32, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl29 = &tmp_x3_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl29, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl30 = &tmp_x2_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl30, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl31 = &tmp_x1_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl31, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl32 = &tmp_x0_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl32, _ln_chpl, _fn_chpl);
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_readWriteHelper_chpl;
#line 737 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 746 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (! call_tmp_chpl8) /* ZLINE: 746 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 746 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl9 = NULL;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    rwLiteral_chpl2(&end_chpl, f_chpl, &error_chpl9, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl9 != nil) /* ZLINE: 747 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      *(error_out_chpl) = error_chpl9;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl33 = &end_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl33, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl34 = &comma1tup_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl34, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl35 = &comma_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl35, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      i_x_chpl36 = &start_chpl2;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(i_x_chpl36, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl33 = &tmp_x3_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl33, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl34 = &tmp_x2_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl34, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl35 = &tmp_x1_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl35, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      _field_destructor_tmp__chpl36 = &tmp_x0_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      deinit_chpl95(_field_destructor_tmp__chpl36, _ln_chpl, _fn_chpl);
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _end_readWriteHelper_chpl;
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 747 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl37 = &end_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl37, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl38 = &comma1tup_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl38, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl39 = &comma_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl39, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl40 = &start_chpl2;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(i_x_chpl40, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl37 = &tmp_x3_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl37, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl38 = &tmp_x2_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl38, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl39 = &tmp_x1_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl39, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl40 = &tmp_x0_chpl;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl40, _ln_chpl, _fn_chpl);
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _end_readWriteHelper_chpl:;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 699 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:709 */
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void getLiterals_chpl(int64_t st_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                             chpl_bool isJson_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                             _tuple_4_star_string_chpl * _retArg_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                             int64_t _ln_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                             int32_t _fn_chpl) {
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e0_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e1_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e2_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e3_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl5;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl6;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl4;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e0_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e1_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e2_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e3_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl9;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl10;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl11;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl8;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e0_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e1_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e2_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e3_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl13;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl14;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl15;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl16;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl12;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (st_chpl == QIO_TUPLE_FORMAT_SPACE) /* ZLINE: 710 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl = _str_literal_163_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e0_x_chpl = &inlineImm_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl2 = _str_literal_401_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e1_x_chpl = &inlineImm_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl3 = _str_literal_163_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e2_x_chpl = &inlineImm_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl4 = _str_literal_163_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e3_x_chpl = &inlineImm_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl, i__e0_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl3 = tmp_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl2, i__e1_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl4 = tmp_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl3, i__e2_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl5 = tmp_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl4, i__e3_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl6 = tmp_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(0)) = ret_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(1)) = ret_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(2)) = ret_chpl5;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(3)) = ret_chpl6;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(0)) = *(ret_chpl2 + INT64(0));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(1)) = *(ret_chpl2 + INT64(1));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(2)) = *(ret_chpl2 + INT64(2));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(3)) = *(ret_chpl2 + INT64(3));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_getLiterals_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  } else /* ZLINE: 712 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (isJson_chpl) /* ZLINE: 712 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl5 = _str_literal_848_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e0_x_chpl2 = &inlineImm_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl6 = _str_literal_825_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e1_x_chpl2 = &inlineImm_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl7 = _str_literal_163_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e2_x_chpl2 = &inlineImm_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl8 = _str_literal_851_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e3_x_chpl2 = &inlineImm_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl5, i__e0_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl8 = tmp_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl6, i__e1_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl9 = tmp_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl7, i__e2_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl10 = tmp_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl8, i__e3_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl11 = tmp_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(0)) = ret_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(1)) = ret_chpl9;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(2)) = ret_chpl10;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(3)) = ret_chpl11;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(0)) = *(ret_chpl7 + INT64(0));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(1)) = *(ret_chpl7 + INT64(1));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(2)) = *(ret_chpl7 + INT64(2));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(3)) = *(ret_chpl7 + INT64(3));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_getLiterals_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  } else /* ZLINE: 714 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl9 = _str_literal_1379_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e0_x_chpl3 = &inlineImm_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl10 = _str_literal_825_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e1_x_chpl3 = &inlineImm_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl11 = _str_literal_472_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e2_x_chpl3 = &inlineImm_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl12 = _str_literal_415_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e3_x_chpl3 = &inlineImm_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl9, i__e0_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl13 = tmp_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl10, i__e1_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl14 = tmp_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl11, i__e2_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl15 = tmp_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl12, i__e3_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl16 = tmp_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(0)) = ret_chpl13;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(1)) = ret_chpl14;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(2)) = ret_chpl15;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(3)) = ret_chpl16;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(0)) = *(ret_chpl12 + INT64(0));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(1)) = *(ret_chpl12 + INT64(1));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(2)) = *(ret_chpl12 + INT64(2));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(3)) = *(ret_chpl12 + INT64(3));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_getLiterals_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _end_getLiterals_chpl:;
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(2)) = *(ret_chpl + INT64(2));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(3)) = *(ret_chpl + INT64(3));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:709 */
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void getLiterals_chpl2(int64_t st_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                              chpl_bool isJson_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                              _tuple_4_star_string_chpl * _retArg_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                              int64_t _ln_chpl,
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                              int32_t _fn_chpl) {
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e0_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e1_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e2_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e3_x_chpl = NULL;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl5;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl6;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl4;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e0_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e1_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e2_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e3_x_chpl2 = NULL;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl9;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl10;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl11;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl8;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e0_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e1_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e2_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl inlineImm_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl i__e3_x_chpl3 = NULL;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _tuple_4_star_string_chpl ret_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl13;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl14;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl15;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl16;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl12;
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (st_chpl == QIO_TUPLE_FORMAT_SPACE) /* ZLINE: 710 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 710 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl = _str_literal_163_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e0_x_chpl = &inlineImm_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl2 = _str_literal_401_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e1_x_chpl = &inlineImm_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl3 = _str_literal_163_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e2_x_chpl = &inlineImm_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl4 = _str_literal_163_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e3_x_chpl = &inlineImm_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl, i__e0_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl3 = tmp_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl2, i__e1_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl4 = tmp_chpl2;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl3, i__e2_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl5 = tmp_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl4, i__e3_x_chpl, _ln_chpl, _fn_chpl);
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl6 = tmp_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(0)) = ret_chpl3;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(1)) = ret_chpl4;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(2)) = ret_chpl5;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl2 + INT64(3)) = ret_chpl6;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(0)) = *(ret_chpl2 + INT64(0));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(1)) = *(ret_chpl2 + INT64(1));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(2)) = *(ret_chpl2 + INT64(2));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(3)) = *(ret_chpl2 + INT64(3));
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_getLiterals_chpl;
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  } else /* ZLINE: 712 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (isJson_chpl) /* ZLINE: 712 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 711 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl5 = _str_literal_848_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e0_x_chpl2 = &inlineImm_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl6 = _str_literal_825_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e1_x_chpl2 = &inlineImm_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl7 = _str_literal_163_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e2_x_chpl2 = &inlineImm_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl8 = _str_literal_851_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e3_x_chpl2 = &inlineImm_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl5, i__e0_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl8 = tmp_chpl5;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl6, i__e1_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl9 = tmp_chpl6;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl7, i__e2_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl10 = tmp_chpl7;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl8, i__e3_x_chpl2, _ln_chpl, _fn_chpl);
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl11 = tmp_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(0)) = ret_chpl8;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(1)) = ret_chpl9;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(2)) = ret_chpl10;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl7 + INT64(3)) = ret_chpl11;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(0)) = *(ret_chpl7 + INT64(0));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(1)) = *(ret_chpl7 + INT64(1));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(2)) = *(ret_chpl7 + INT64(2));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(3)) = *(ret_chpl7 + INT64(3));
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_getLiterals_chpl;
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  } else /* ZLINE: 714 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 713 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl9 = _str_literal_1379_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e0_x_chpl3 = &inlineImm_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl10 = _str_literal_825_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e1_x_chpl3 = &inlineImm_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl11 = _str_literal_472_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e2_x_chpl3 = &inlineImm_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    inlineImm_chpl12 = _str_literal_415_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__e3_x_chpl3 = &inlineImm_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl9, i__e0_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl13 = tmp_chpl9;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl10, i__e1_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl14 = tmp_chpl10;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl11, i__e2_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl15 = tmp_chpl11;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_ASSIGN__chpl25(&tmp_chpl12, i__e3_x_chpl3, _ln_chpl, _fn_chpl);
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl16 = tmp_chpl12;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(0)) = ret_chpl13;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(1)) = ret_chpl14;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(2)) = ret_chpl15;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl12 + INT64(3)) = ret_chpl16;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(0)) = *(ret_chpl12 + INT64(0));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(1)) = *(ret_chpl12 + INT64(1));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(2)) = *(ret_chpl12 + INT64(2));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(ret_chpl + INT64(3)) = *(ret_chpl12 + INT64(3));
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_getLiterals_chpl;
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 715 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _end_getLiterals_chpl:;
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(0)) = *(ret_chpl + INT64(0));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(1)) = *(ret_chpl + INT64(1));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(2)) = *(ret_chpl + INT64(2));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(*(_retArg_chpl) + INT64(3)) = *(ret_chpl + INT64(3));
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 709 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:721 */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void helper_chpl2(int64_t * arg_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         fileWriter_F_nothing_chpl * f_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         Error_chpl * error_out_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int64_t _ln_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int32_t _fn_chpl) {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl local_nilLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int64_t coerce_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl ret_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl call_tmp_chpl6 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl ret_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_bool call_tmp_chpl7;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl call_tmp_chpl8;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int32_t call_tmp_chpl10;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_localeID_t call_tmp_chpl11;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl call_tmp_chpl12;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl ret_tmp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl call_tmp_chpl13 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  syserr call_tmp_chpl14;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_SystemError_chpl call_tmp_chpl15;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_SystemError_chpl ret_tmp_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl call_tmp_chpl16 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl ret_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_SystemError_chpl i_x_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_SystemError_chpl _formal_tmp_x_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl ret_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_SystemError_chpl i_this_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl call_tmp_chpl17 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_SystemError_chpl i_this_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl call_tmp_chpl18;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl initTemp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int32_t call_tmp_chpl19;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl3 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl4 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_nilLocale_chpl = nilLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  coerce_tmp_chpl3 = *(arg_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = &((f_chpl)->_readWriteThisFromLocale);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl(&ret_chpl2, call_tmp_chpl6);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl7 = chpl___EQUALS_(&ret_chpl2, &local_nilLocale_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (call_tmp_chpl7) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl10 = get_chpl_nodeID();
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl11 = chpl_rt_buildLocaleID(call_tmp_chpl10, c_sublocid_any);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl9 = call_tmp_chpl11;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl9, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl8 = call_tmp_chpl12;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl___ASSIGN_9(&ret_chpl2, &call_tmp_chpl8);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl62(i_x_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  ret_chpl = ret_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  origLocale_chpl = ret_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl13 = &((f_chpl)->_home);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (((RootClass_chpl)((call_tmp_chpl13)->_instance)) == nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl14 = qio_int_to_err(((int32_t)(INT64(0))));
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if ((f_chpl)->_channel_internal == nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    createSystemError_chpl2(((int64_t)(EINVAL)), &_str_literal_1920_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl15 = ret_tmp_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl2 = &call_tmp_chpl15;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__retArg_chpl = &ret_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl2);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl17 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    (i_this_chpl2)->chpl_p = ((SystemError_chpl)(nil));
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (((RootClass_chpl)(call_tmp_chpl17)) == nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_chpl188(&initTemp_chpl, call_tmp_chpl17);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl18 = initTemp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl3 = call_tmp_chpl18;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl__autoDestroy54(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(i__retArg_chpl) = ret_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    coerce_tmp_chpl4 = ret_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl4;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl16 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl2 = call_tmp_chpl16;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endlock_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl19 = qio_err_iserr(call_tmp_chpl14);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (call_tmp_chpl19 != INT32(0)) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl3 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _ch_ioerror_chpl4(f_chpl, call_tmp_chpl14, &_str_literal_1919_chpl, &error_chpl3, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl3 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto handler_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    handler_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl3 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      error_chpl2 = error_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _endlock_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endlock_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl2:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = error_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl3 = &origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl62(i_x_chpl3);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endwrite_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl4 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _writeOne_chpl7(f_chpl, coerce_tmp_chpl3, &origLocale_chpl, &error_chpl4, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl4 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl3:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl4 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = error_chpl4;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl4 = &origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl62(i_x_chpl4);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endwrite_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl5 = &origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl62(i_x_chpl5);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endwrite_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endhelper_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endhelper_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:721 */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void helper_chpl(int64_t * arg_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                        fileWriter_F_nothing_chpl * f_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                        Error_chpl * error_out_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                        int64_t _ln_chpl,
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                        int32_t _fn_chpl) {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl local_nilLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int64_t coerce_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl ret_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl call_tmp_chpl6 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl ret_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_bool call_tmp_chpl7;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl call_tmp_chpl8;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_localeID_t call_tmp_chpl9;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int32_t call_tmp_chpl10;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_localeID_t call_tmp_chpl11;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl call_tmp_chpl12;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl ret_tmp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  locale_chpl origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl call_tmp_chpl13 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  syserr call_tmp_chpl14;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_SystemError_chpl call_tmp_chpl15;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_SystemError_chpl ret_tmp_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl call_tmp_chpl16 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl coerce_tmp_chpl4;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl ret_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_SystemError_chpl i_x_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_Error_chpl2 i__retArg_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_SystemError_chpl _formal_tmp_x_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl ret_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_SystemError_chpl i_this_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl call_tmp_chpl17 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref__owned_SystemError_chpl i_this_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl call_tmp_chpl18;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl initTemp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _owned_Error_chpl _formal_tmp_in_err_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  int32_t call_tmp_chpl19;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl3 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl3 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl4 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl4 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_locale_chpl i_x_chpl5 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_nilLocale_chpl = nilLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  coerce_tmp_chpl3 = *(arg_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = &((f_chpl)->_readWriteThisFromLocale);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl(&ret_chpl2, call_tmp_chpl6);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl7 = chpl___EQUALS_(&ret_chpl2, &local_nilLocale_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (call_tmp_chpl7) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl10 = get_chpl_nodeID();
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl11 = chpl_rt_buildLocaleID(call_tmp_chpl10, c_sublocid_any);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl9 = call_tmp_chpl11;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_localeID_to_locale(&call_tmp_chpl9, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl12 = ret_tmp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl8 = call_tmp_chpl12;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl___ASSIGN_9(&ret_chpl2, &call_tmp_chpl8);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl = &call_tmp_chpl8;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl62(i_x_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  ret_chpl = ret_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  origLocale_chpl = ret_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl13 = &((f_chpl)->_home);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (((RootClass_chpl)((call_tmp_chpl13)->_instance)) == nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl2 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl14 = qio_int_to_err(((int32_t)(INT64(0))));
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if ((f_chpl)->_channel_internal == nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    createSystemError_chpl2(((int64_t)(EINVAL)), &_str_literal_1920_chpl, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl15 = ret_tmp_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl2 = &call_tmp_chpl15;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i__retArg_chpl = &ret_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _formal_tmp_x_chpl = *(i_x_chpl2);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_this_chpl = &_formal_tmp_x_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl17 = ((Error_chpl)((i_this_chpl)->chpl_p));
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_this_chpl2 = &_formal_tmp_x_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    (i_this_chpl2)->chpl_p = ((SystemError_chpl)(nil));
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (((RootClass_chpl)(call_tmp_chpl17)) == nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    init_chpl188(&initTemp_chpl, call_tmp_chpl17);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl18 = initTemp_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    ret_chpl3 = call_tmp_chpl18;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl__autoDestroy54(&_formal_tmp_x_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(i__retArg_chpl) = ret_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    coerce_tmp_chpl4 = ret_tmp_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _formal_tmp_in_err_chpl = coerce_tmp_chpl4;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    call_tmp_chpl16 = chpl_fix_thrown_error(&_formal_tmp_in_err_chpl, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl2 = call_tmp_chpl16;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endlock_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl19 = qio_err_iserr(call_tmp_chpl14);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (call_tmp_chpl19 != INT32(0)) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl3 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _ch_ioerror_chpl4(f_chpl, call_tmp_chpl14, &_str_literal_1919_chpl, &error_chpl3, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl3 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto handler_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    handler_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    if (error_chpl3 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      error_chpl2 = error_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
      goto _endlock_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endlock_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl2:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = error_chpl2;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl3 = &origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl62(i_x_chpl3);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endwrite_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl4 = NULL;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _writeOne_chpl7(f_chpl, coerce_tmp_chpl3, &origLocale_chpl, &error_chpl4, _ln_chpl, _fn_chpl);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl4 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl3;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl3:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl4 != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = error_chpl4;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    i_x_chpl4 = &origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl62(i_x_chpl4);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endwrite_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  i_x_chpl5 = &origLocale_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl62(i_x_chpl5);
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endwrite_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 721 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endhelper_chpl;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endhelper_chpl:;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 721 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:724 */
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void rwLiteral_chpl(string_chpl * lit_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           fileWriter_F_nothing_chpl * f_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           Error_chpl * error_out_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           int64_t _ln_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                           int32_t _fn_chpl) {
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_ioLiteral call_tmp_chpl6;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_ioLiteral initTemp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl _formal_tmp_in_val_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_ioLiteral iolit_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl2 = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, lit_chpl, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  ret_chpl = tmp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _formal_tmp_in_val_chpl = ret_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_chpl219(&initTemp_chpl, &_formal_tmp_in_val_chpl, UINT8(true));
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  iolit_chpl = call_tmp_chpl6;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl2 = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeIt_chpl(f_chpl, &iolit_chpl, &error_chpl2, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 725 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = error_chpl2;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl = &((&iolit_chpl)->val);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_writeLiteral_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl2 = &((&iolit_chpl)->val);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _end_writeLiteral_chpl:;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 725 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endrwLiteral_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endrwLiteral_chpl:;
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:724 */
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void rwLiteral_chpl2(string_chpl * lit_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            fileWriter_F_nothing_chpl * f_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            Error_chpl * error_out_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            int64_t _ln_chpl,
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                            int32_t _fn_chpl) {
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_ioLiteral call_tmp_chpl6;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_ioLiteral initTemp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl _formal_tmp_in_val_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl tmp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_ioLiteral iolit_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl2 = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _ref_string_chpl _field_destructor_tmp__chpl2 = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_ASSIGN__chpl25(&tmp_chpl, lit_chpl, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  ret_chpl = tmp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _formal_tmp_in_val_chpl = ret_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  init_chpl219(&initTemp_chpl, &_formal_tmp_in_val_chpl, UINT8(true));
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = initTemp_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  iolit_chpl = call_tmp_chpl6;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl2 = NULL;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeIt_chpl(f_chpl, &iolit_chpl, &error_chpl2, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl2 != nil) /* ZLINE: 725 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    error_chpl = error_chpl2;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    _field_destructor_tmp__chpl = &((&iolit_chpl)->val);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    deinit_chpl95(_field_destructor_tmp__chpl, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _end_writeLiteral_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _field_destructor_tmp__chpl2 = &((&iolit_chpl)->val);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  deinit_chpl95(_field_destructor_tmp__chpl2, _ln_chpl, _fn_chpl);
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _end_writeLiteral_chpl:;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 725 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    *(error_out_chpl) = error_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto _endrwLiteral_chpl;
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 725 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  _endrwLiteral_chpl:;
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 724 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:887 */
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeln_chpl3(string_chpl * _e0_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int64_t _e1_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int64_t _ln_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int32_t _fn_chpl) {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  fileWriter_T_nothing_chpl local_stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_stdout_chpl = stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeln_chpl10(&local_stdout_chpl, _e0_args_chpl, _e1_args_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl:;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:887 */
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeln_chpl(string_chpl * _e0_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int64_t _e1_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         string_chpl * _e2_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         _real64 _e3_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         string_chpl * _e4_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int64_t _e5_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         string_chpl * _e6_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int64_t _e7_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int64_t _ln_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                         int32_t _fn_chpl) {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  fileWriter_T_nothing_chpl local_stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_stdout_chpl = stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeln_chpl8(&local_stdout_chpl, _e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, _e5_args_chpl, _e6_args_chpl, _e7_args_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl:;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:887 */
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeln_chpl5(string_chpl * _e0_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          _tuple_4_star_int64_t_chpl * _e1_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int64_t _ln_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int32_t _fn_chpl) {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  fileWriter_T_nothing_chpl local_stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_stdout_chpl = stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeln_chpl6(&local_stdout_chpl, _e0_args_chpl, _e1_args_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl:;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:887 */
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeln_chpl2(string_chpl * _e0_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          _tuple_3_star_int64_t_chpl * _e1_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int64_t _ln_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int32_t _fn_chpl) {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  fileWriter_T_nothing_chpl local_stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_stdout_chpl = stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeln_chpl7(&local_stdout_chpl, _e0_args_chpl, _e1_args_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl:;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:887 */
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void writeln_chpl4(string_chpl * _e0_args_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int64_t _ln_chpl,
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                          int32_t _fn_chpl) {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  fileWriter_T_nothing_chpl local_stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  Error_chpl error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  local_stdout_chpl = stdout_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  error_chpl = NULL;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  writeln_chpl9(&local_stdout_chpl, _e0_args_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    goto handler_chpl;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  handler_chpl:;
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  if (error_chpl != nil) /* ZLINE: 888 /Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl */
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  {
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 888 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  }
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 887 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper7(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify3(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper9(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    ArrayInit_chpl _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify5(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper17(int8_t * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify16(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper11(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify2(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper6(int64_t _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify22(_e0_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper16(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e4_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify15(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper12(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_int64_t_both_one_chpl _e4_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify20(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper10(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_int64_t_both_one_chpl _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_int64_t_both_one_chpl _e4_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify8(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper13(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_int64_t_both_one_chpl _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify6(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper8(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify14(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper15(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_int64_t_low_one_chpl _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify17(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper19(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_byteIndex_low_one_chpl _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify18(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper20(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_byteIndex_high_one_chpl _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify19(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   string_chpl * _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   string_chpl * _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                   int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify21(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper14(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     uint64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     uint64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify7(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper3(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    range_int64_t_neither_one_chpl _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper2(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    range_int64_t_both_one_chpl _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify11(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper21(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     uint64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify10(_e0_args_chpl, _e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper4(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify13(_e0_args_chpl, _e1_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper18(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e4_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e5_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e6_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e7_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e8_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify9(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, _e5_args_chpl, _e6_args_chpl, _e7_args_chpl, _e8_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper5(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    uint64_t _e4_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _e5_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    uint64_t _e6_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                    int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify4(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, _e4_args_chpl, _e5_args_chpl, _e6_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
/* ChapelIO.chpl:913 */
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
static void chpl_stringify_wrapper22(string_chpl * _e0_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     range_byteIndex_both_one_chpl _e1_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _e2_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _e3_args_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     string_chpl * _retArg_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int64_t _ln_chpl,
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
                                     int32_t _fn_chpl) {
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl call_tmp_chpl6;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  string_chpl ret_tmp_chpl;
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  chpl_stringify12(_e0_args_chpl, _e1_args_chpl, _e2_args_chpl, _e3_args_chpl, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 915 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  call_tmp_chpl6 = ret_tmp_chpl;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  *(_retArg_chpl) = call_tmp_chpl6;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
  return;
#line 913 "/Users/iainmoncrief/Documents/chapel/modules/standard/ChapelIO.chpl"
}

