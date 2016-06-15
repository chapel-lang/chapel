/*
  structures to improve error handling in chapel programs
 */
module stout {

   record Error {
      type eltype;
      var msg:string;

      proc Error(type eltype, const message:string) {
         msg = message;
      }

      proc this(type eltype) : Try(eltype) {
         return Try(eltype).error(eltype, msg);
      }

      proc this(type eltype) : Result(eltype) {
         return Result(eltype).error(eltype, msg);
      }
   }

   proc =(ref lval:Try(?eltype), rval:Error(eltype)) {
      lval.e = Option(Error(eltype)).some(rval);
   }

   proc =(ref lval:Result(?eltype), rval:Error(eltype)) {
      lval.t = new Result(Error(eltype), rval);
   }

   // capture the concept of void
   // record Nothing {}
 
   enum OptionState { SOME, NONE }

   record Option {
      type option_type;
      var state = OptionState.NONE;
      var t:option_type;      

      proc Option(type option_type) {
         state = OptionState.NONE;
      }

      proc Option(type option_type, const val:option_type) {
         state = OptionState.SOME;
         t = val;
      }

      proc Option(type option_type, const that:Option(option_type)) {
         state = that.state;
         t = if that.t != nil then that.t else nil; 
      }

      proc isSome() { return state == OptionState.SOME; }
      proc isNone() { return state == OptionState.NONE; }

      proc get() : option_type { assert(state == OptionState.SOME, "Option is not a SOME"); return t; }
      proc get(const val:option_type) : option_type {  return if state == OptionState.NONE then val else t; }

      proc this() { return get(); }

   }

   proc type Option.none(type option_type) {
      return new Option(option_type);
   }

   proc type Option.some(const val:?option_type) {
      return new Option(option_type, val);
   }

   proc ==(lval:Option(?option_type), rval:Option(option_type)) {
      return (lval.state == OptionState.NONE && rval.state == OptionState.NONE) || (lval.state == OptionState.SOME && rval.state == OptionState.SOME && lval.t == rval.t);
   }

   record Result {
      type result_type;
      var t:Try(Option(result_type)) = Try(Option(result_type)).some(Option(result_type).none(result_type));

      proc Result(type result_type) {
         t = Try(Option(result_type)).some(Option(result_type).none(result_type));
      }

      proc Result(type result_type, const val:result_type) {
         if result_type == string {
            t = Try(Option(result_type)).error(Option(result_type), val);
         }
         else {
            t = Try(Option(result_type)).some(Option(result_type).some(val));
         }
writeln(t);
      }

      proc Result(type result_type, const that:Result(result_type)) {
         if that.isSome() {
            t = that.t;
         }
      }

      proc isSome() { return t.isSome() && t.get().isSome(); }
      proc isNone() { return t.isSome() && t.get().isNone(); }
      proc isError() { return t.isError(); }

      proc error() { return t.error(); }

      proc get() {
         if isSome() {
            const tt = t.get();
            if tt.isSome() {
              const ttt = tt.get();
              return ttt;
            }
         }

         assert(false, "error Result.get()");
         return t.get().get();
      }

      proc this() { return get(); }

   }

   proc type Result.none(type result_type) {
      return new Result(result_type);
   }

   proc type Result.some(const val:?T) {
      return new Result(T, val);
   }

   proc type Result.error(type T, const message:string) {
      return new Result(T, message);
   }

   record Try {
      type try_type;
      var t:Option(try_type) = Option(try_type).none(try_type);
      var e:Option(Error(try_type)) = Option(Error(try_type)).none(Error(try_type)); 

      proc Try(type try_type, val:try_type) {
         t = Option(try_type).some(val);
      }

      proc Try(type try_type, const message:string) {
         e = Option(Error(try_type)).some(new Error(try_type, message));
      }

      proc Try(type try_type, const err:Error) {
         e = Option(Error(try_type)).some(err);
      }

      proc Try(type try_type, const that:Try(try_type)) {
         if that.t.isSome() {
            t = that.t;
         }
         else {
            e = Option(Error(try_type)).some(new Error(try_type, that.message));
         }
      }

      proc error() : Error(try_type) {
         return e.get();
      }

      proc isSome() { return t.isSome(); }
      proc isError() { return e.isSome(); }

      proc get() {
         assert(isSome(), "Try is not some");
         return t.get();
      }

      proc this() {
         return get();
      }
   }
   
   proc ==(lval:Try(?T), rval:Try(T)) {
      if lval.isSome() && rval.isSome() {
         return (lval.get() == rval.get());
      }

      return false;
   }

   proc !=(lval:Try(?T), rval:Try(T)) {
      return !(lval == rval);
   }

   proc type Try.some(const val:?T) {
      return new Try(T, val);
   }

   proc type Try.error(type try_type, const message:string) {
      return new Try(try_type, message);
   }

} // end module 

