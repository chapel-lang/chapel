#include "plugin.hpp"
#include "system.hpp"
#include "instrumentation.hpp"
#include "instrumentationcontext_decl.hpp"

namespace nanos {

class InstrumentationPrintTrace: public Instrumentation 
{
#ifndef NANOS_INSTRUMENTATION_ENABLED
   public:
      // constructor
      InstrumentationPrintTrace() : Instrumentation() {}
      // destructor
      ~InstrumentationPrintTrace() {}

      // low-level instrumentation interface (mandatory functions)
      void initialize( void ) {}
      void finalize( void ) {}
      void addEventList ( unsigned int count, Event *events ) {}
#else
   public:
      // constructor
      InstrumentationPrintTrace() : Instrumentation( *new InstrumentationContext() ) {}
      // destructor
      ~InstrumentationPrintTrace ( ) {}

      // low-level instrumentation interface (mandatory functions)
      void initialize( void ) {}
      void finalize( void ) {}
      void addEventList ( unsigned int count, Event *events ) {}
#endif

};

namespace ext {

class InstrumentationPrintTracePlugin : public Plugin {
   public:
      InstrumentationPrintTracePlugin () : Plugin("Instrumentation which print the trace to std out.",1) {}
      ~InstrumentationPrintTracePlugin () {}

      void config( Config &config ) {}

      void init ()
      {
         sys.setInstrumentation( new InstrumentationPrintTrace() );	
      }
};

} // namespace ext

} // namespace nanos

nanos::ext::InstrumentationPrintTracePlugin NanosXPlugin;

