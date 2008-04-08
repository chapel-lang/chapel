        set     arg1,1*8
        set     arg2,2*8
        set     arg3,3*8
        set     arg4,4*8

        text

 #    ts1am_2me(sx, sy, sz)
 #
        global ts1am_2me
        align 4
ts1am_2me:
        using   ts1am_2me,$s33
        lds     $s35,arg1(,$s34)        # Sx : atomic data
        lds     $s36,arg2(,$s34)        # Sy : control flag
        lds     $s37,arg3(,$s34)        # Sz : pointer
        ts1am   $s35,$s36,$s37          #
        or      $s123,0,$s35            # RVAL = original data;
        be      0,0(,$s32) 

