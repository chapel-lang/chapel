      program kinds
      INTEGER*4 I4
      INTEGER*8 I8
      I4 = 1
      I8 = 1234567890123_8
      write (*,'("The KIND for single precision is ",I2)') KIND(0.0)
      write (*,'("The KIND for double precision is ",I2)') KIND(0.0D0)
      write (*,'("The KIND for integer *4 is ",I2)') KIND(I4)
      write (*,'("The KIND for integer *8 is ",I2)') KIND(I8)
      write (*,'("The KIND for single complex is ",I2)') KIND((0.0,0.0))
      write (*,'("The KIND for double precision is ",I2)') KIND((0.0D0,0.0D0))
      stop
      end
