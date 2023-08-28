#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:25 */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static void chpl__init_ChapelUtil(int64_t _ln_chpl,
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                  int32_t _fn_chpl) {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_int32_t_chpl refIndentLevel_chpl = NULL;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (chpl__init_ChapelUtil_p) /* ZLINE: 25 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _exit_chpl__init_ChapelUtil_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  printModuleInit_chpl("%*s\n", "ChapelUtil", INT64(10), _ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  refIndentLevel_chpl = &moduleInitLevel_chpl;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  *(refIndentLevel_chpl) += INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__init_ChapelUtil_p = UINT8(true);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl__init_CTypes(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl__init_Regex(_ln_chpl, _fn_chpl);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  *(refIndentLevel_chpl) -= INT64(1);
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _exit_chpl__init_ChapelUtil_chpl:;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 25 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:32 */
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool safeAdd_chpl(int64_t a_chpl,
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                              int64_t b_chpl) {
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (a_chpl < INT64(0)) /* ZLINE: 35 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 35 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (b_chpl >= INT64(0)) /* ZLINE: 36 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 36 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      ret_chpl = UINT8(true);
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      goto _end_safeAdd_chpl;
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    } else /* ZLINE: 39 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (b_chpl < ((int64_t)((-INT64(9223372036854775807) - INT64(1) - a_chpl)))) /* ZLINE: 39 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 37 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      ret_chpl = UINT8(false);
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      goto _end_safeAdd_chpl;
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    } else /* ZLINE: 41 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 40 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      ret_chpl = UINT8(true);
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      goto _end_safeAdd_chpl;
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    }
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  } else /* ZLINE: 46 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (b_chpl <= INT64(0)) /* ZLINE: 46 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 42 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ret_chpl = UINT8(true);
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _end_safeAdd_chpl;
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  } else /* ZLINE: 49 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (b_chpl > ((int64_t)((INT64(9223372036854775807) - a_chpl)))) /* ZLINE: 49 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 47 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ret_chpl = UINT8(false);
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _end_safeAdd_chpl;
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  } else /* ZLINE: 51 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 50 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ret_chpl = UINT8(true);
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _end_safeAdd_chpl;
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 52 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end_safeAdd_chpl:;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 32 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:105 */
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool safeMul_chpl(uint64_t a_chpl,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                              uint64_t b_chpl,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                              int64_t _ln_chpl,
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                              int32_t _fn_chpl) {
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl;
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl2;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl3;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t call_tmp_chpl6;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl4;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t call_tmp_chpl7;
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl5;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl6;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t call_tmp_chpl8;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl7;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl8;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl9;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t call_tmp_chpl9;
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (INT64(0) < INT64(0)) /* ZLINE: 108 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    tmp_chpl = UINT8(true);
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  } else /* ZLINE: 108 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    tmp_chpl = (a_chpl > ((uint64_t)(INT64(0))));
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (tmp_chpl) /* ZLINE: 108 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 108 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (INT64(0) < INT64(0)) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      tmp_chpl2 = UINT8(true);
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    } else /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      tmp_chpl2 = (b_chpl > ((uint64_t)(INT64(0))));
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    }
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (tmp_chpl2) /* ZLINE: 109 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 109 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 110 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl3 = (b_chpl == ((uint64_t)(INT64(0))));
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      } else /* ZLINE: 110 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl3 = UINT8(false);
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (tmp_chpl3) /* ZLINE: 110 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      call_tmp_chpl6 = (UINT64(18446744073709551615) / b_chpl);
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (a_chpl > call_tmp_chpl6) /* ZLINE: 110 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        ret_chpl = UINT8(false);
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        goto _end_safeMul_chpl;
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    } else /* ZLINE: 112 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 110 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 113 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl4 = (a_chpl == ((uint64_t)(INT64(0))));
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      } else /* ZLINE: 113 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl4 = UINT8(false);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (tmp_chpl4) /* ZLINE: 113 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      call_tmp_chpl7 = (UINT64(0) / a_chpl);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (b_chpl < call_tmp_chpl7) /* ZLINE: 113 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        ret_chpl = UINT8(false);
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        goto _end_safeMul_chpl;
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    }
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  } else /* ZLINE: 116 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 113 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (INT64(0) < INT64(0)) /* ZLINE: 117 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      tmp_chpl5 = UINT8(true);
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    } else /* ZLINE: 117 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      tmp_chpl5 = (b_chpl > ((uint64_t)(INT64(0))));
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    }
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (tmp_chpl5) /* ZLINE: 117 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 117 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (! (INT64(0) < INT64(0))) /* ZLINE: 118 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl6 = (b_chpl == ((uint64_t)(INT64(0))));
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      } else /* ZLINE: 118 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl6 = UINT8(false);
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (tmp_chpl6) /* ZLINE: 118 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      call_tmp_chpl8 = (UINT64(0) / b_chpl);
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (a_chpl < call_tmp_chpl8) /* ZLINE: 118 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        ret_chpl = UINT8(false);
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        goto _end_safeMul_chpl;
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    } else /* ZLINE: 120 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 118 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (INT64(0) < INT64(0)) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl7 = UINT8(true);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      } else /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl7 = (a_chpl != ((uint64_t)(INT64(0))));
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (tmp_chpl7) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        if (! (INT64(0) < INT64(0))) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
          tmp_chpl9 = (a_chpl == ((uint64_t)(INT64(0))));
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        } else /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
          tmp_chpl9 = UINT8(false);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        }
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        if (tmp_chpl9) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
          halt_chpl(&_str_literal_30_chpl, _ln_chpl, _fn_chpl);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        }
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        call_tmp_chpl9 = (UINT64(18446744073709551615) / a_chpl);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl8 = (b_chpl < call_tmp_chpl9);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      } else /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        tmp_chpl8 = UINT8(false);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      if (tmp_chpl8) /* ZLINE: 121 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      {
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        ret_chpl = UINT8(false);
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
        goto _end_safeMul_chpl;
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      }
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    }
#line 121 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = UINT8(true);
#line 125 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end_safeMul_chpl:;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 105 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static _real64 _command_line_cast_chpl17(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _real64 ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _real64 call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_28(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl19(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl24(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl6(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl9(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl21(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl23(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl22(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl20(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static uint64_t _command_line_cast_chpl18(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_26(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static uint64_t _command_line_cast_chpl16(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  uint64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_26(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static void _command_line_cast_chpl12(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      string_chpl * _retArg_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = ret_chpl2;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static void _command_line_cast_chpl11(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      string_chpl * _retArg_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = ret_chpl2;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static void _command_line_cast_chpl10(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      string_chpl * _retArg_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = ret_chpl2;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl8(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl4(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                          int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static void _command_line_cast_chpl14(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      string_chpl * _retArg_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                      int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = ret_chpl2;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 145 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  *(_retArg_chpl) = ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl13(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                         int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl7(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static _real64 _command_line_cast_chpl5(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _real64 ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _real64 call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_28(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl3(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static int64_t _command_line_cast_chpl2(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                        int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_27(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:134 */
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static chpl_bool _command_line_cast_chpl15(int8_t * x_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int64_t _ln_chpl,
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                                           int32_t _fn_chpl) {
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool ret_chpl;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _owned_IllegalArgumentError_chpl2 ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref__owned_IllegalArgumentError_chpl i_arg_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  IllegalArgumentError_chpl call_tmp_chpl8 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl coerce_tmp_chpl3 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  int64_t call_tmp_chpl10;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  Error_chpl error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  string_chpl str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl2 = NULL;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _ref_string_chpl i_x_chpl3 = NULL;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl6 = string_length_bytes(x_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_checkValue2(call_tmp_chpl6, &ret_tmp_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl7 = ret_tmp_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_arg_chpl = &call_tmp_chpl7;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl8 = borrow_chpl2(i_arg_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (((RootClass_chpl)(call_tmp_chpl8)) != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = ((Error_chpl)(call_tmp_chpl8));
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    ((void(*)(Error_chpl,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       string_chpl *,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int64_t,
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
       int32_t))chpl_vmtable[((INT64(6) * ((RootClass_chpl)(coerce_tmp_chpl3))->chpl__cid) + INT64(1))])(coerce_tmp_chpl3, &ret_tmp_chpl2, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    call_tmp_chpl9 = ret_tmp_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    safeCastCheckHalt_chpl(&call_tmp_chpl9, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl = &call_tmp_chpl9;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl__autoDestroy62(&call_tmp_chpl7, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl10 = call_tmp_chpl6;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  error_chpl2 = NULL;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  createCopyingBuffer_chpl(((c_ptr_uint8_t_chpl)(x_chpl)), call_tmp_chpl10, ((int64_t)((call_tmp_chpl10 + INT64(1)))), decodePolicy_chpl_strict_chpl, &error_chpl2, &ret_tmp_chpl3, _ln_chpl, _fn_chpl);
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl11 = ret_tmp_chpl3;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl2 != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    error_chpl = error_chpl2;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto _endcreateCopyingBuffer_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl2 = call_tmp_chpl11;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _endcreateCopyingBuffer_chpl:;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 143 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 143 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  str_chpl = ret_chpl2;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  call_tmp_chpl12 = chpl___COLON_19(&str_chpl, &error_chpl, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 151 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    i_x_chpl2 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    deinit_chpl95(i_x_chpl2, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    goto handler_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  ret_chpl = call_tmp_chpl12;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  i_x_chpl3 = &str_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deinit_chpl95(i_x_chpl3, _ln_chpl, _fn_chpl);
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  goto _end__command_line_cast_chpl;
#line 151 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  handler_chpl:;
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  if (error_chpl != nil) /* ZLINE: 142 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  {
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_uncaught_error(error_chpl, _ln_chpl, _fn_chpl);
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 142 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  _end__command_line_cast_chpl:;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return ret_chpl;
#line 134 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:211 */
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
static void chpl_addModule(c_string_rehook moduleName_chpl,
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                           c_fn_ptr deinitFun_chpl,
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                           int64_t _ln_chpl,
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
                           int32_t _fn_chpl) {
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_ModuleDeinit new_temp_chpl = NULL;
#line 213 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  new_temp_chpl = _new_chpl18(((c_ptrConst_int8_t_chpl)(moduleName_chpl)), deinitFun_chpl, chpl_moduleDeinitFuns, _ln_chpl, _fn_chpl);
#line 212 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_moduleDeinitFuns = new_temp_chpl;
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 211 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
/* ChapelUtil.chpl:216 */
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
void chpl_deinitModules(void) {
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_ModuleDeinit prev_chpl = NULL;
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_bool tmp_chpl;
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_ModuleDeinit curr_chpl = NULL;
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_ModuleDeinit coerce_tmp_chpl3 = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_ModuleDeinit coerce_tmp_chpl4 = NULL;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_ModuleDeinit coerce_tmp_chpl5 = NULL;
#line 223 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  prev_chpl = chpl_moduleDeinitFuns;
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  tmp_chpl = (((RootClass_chpl)(prev_chpl)) != nil);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  while (tmp_chpl) {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    if (((RootClass_chpl)(prev_chpl)) == nil) /* ZLINE: 225 /Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl */
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    {
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
      nilCheckHalt_chpl(&_str_literal_52_chpl, INT64(225), INT32(39));
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    }
#line 225 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    curr_chpl = prev_chpl;
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl3 = prev_chpl;
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl3, INT64(228), INT32(39));
#line 228 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_execute_module_deinit((coerce_tmp_chpl3)->deinitFun);
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl4 = prev_chpl;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl_check_nil(coerce_tmp_chpl4, INT64(229), INT32(39));
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    coerce_tmp_chpl5 = (coerce_tmp_chpl4)->prevModule;
#line 229 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    prev_chpl = coerce_tmp_chpl5;
#line 230 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    chpl__delete47(curr_chpl, INT64(230), INT32(39));
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
    tmp_chpl = (((RootClass_chpl)(coerce_tmp_chpl5)) != nil);
#line 224 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  }
#line 233 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  chpl_moduleDeinitFuns = ((chpl_ModuleDeinit)(nil));
#line 236 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  deallocate_string_literals_buf();
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
  return;
#line 216 "/Users/iainmoncrief/Documents/chapel/modules/internal/ChapelUtil.chpl"
}

