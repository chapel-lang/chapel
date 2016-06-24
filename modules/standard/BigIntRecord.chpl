module BigIntRecord {
  use GMP;
  //
  // BigInt Record Implementation
  //
  pragma "ignore noinit"
  record BigIntRecord{
    /*var mpz : mpz_t; //the big int

    // initializing integers (constructors)
    proc BigIntRecord(init2:bool, nbits:uint) {
      mpz_init2(this.mpz, nbits.safeCast(c_ulong));
    }
    proc BigIntRecord(num:int) {
      mpz_init_set_si(this.mpz, num.safeCast(c_long));
    }
    proc BigIntRecord(str:string, base:int=0) {
      var e:c_int;
      e = mpz_init_set_str(this.mpz, str.localize().c_str(), base.safeCast(c_int));
      if e {
        mpz_clear(this.mpz);
        halt("Error initializing big integer: bad format");
      }
    }
     proc BigIntRecord(str:string, base:int=0, out error:syserr) {
      var e:c_int;
      error = ENOERR;
      e = mpz_init_set_str(this.mpz, str.localize().c_str(), base.safeCast(c_int));
      if e {
        mpz_clear(this.mpz);
        error = EFORMAT;
      }
    }
    proc BigIntRecord(num:BigIntRecord) {
      if num.locale == here {
        mpz_init_set(this.mpz, num.mpz);
      } else {
        mpz_init(this.mpz);
        var mpz_struct = num.mpzStruct(); 
        chpl_gmp_get_mpz(this.mpz, num.locale.id, mpz_struct);
      }
    }
    proc BigIntRecord() { mpz_init(this.mpz); }

    // destructor
    proc ~BigIntRecord() { on this do mpz_clear(this.mpz); }*/

    var bi : BigInt;
    proc BigIntRecord(init2:bool, nbits:uint){
      bi = new BigInt(init2, nbits);
    }
    proc BigIntRecord(num:int) {
      bi = new BigInt(num);
    }
    proc BigIntRecord(str:string, base:int=0) {
      bi = new BigInt(str, base);
    }
    proc BigIntRecord(num:BigInt) {
      bi = new BigInt(num);
    }
    proc BigIntRecord(num:BigIntRecord) {
      bi = new BigInt(num.bi);
    }
    proc ~BigIntRecord(){
      if (bi != nil) then
        delete bi;
    }


  }
}