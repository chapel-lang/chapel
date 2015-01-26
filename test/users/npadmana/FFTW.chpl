// Wrapper for FFTW version 3.
module FFTW {

	use SysCTypes;

	// Define the various planner flags
	// See Sec. 4.3.2 of FFTW manual "Planner Flags"
	extern const FFTW_ESTIMATE : c_uint;
	extern const FFTW_MEASURE : c_uint;
	extern const FFTW_PATIENT : c_uint;
	extern const FFTW_EXHAUSTIVE : c_uint;
	extern const FFTW_WISDOM_ONLY : c_uint;
	extern const FFTW_DESTROY_INPUT : c_uint;
	extern const FFTW_PRESERVE_INPUT : c_uint;
	extern const FFTW_UNALIGNED : c_uint;



}
