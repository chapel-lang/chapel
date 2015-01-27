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

	// Define types
	// NOTE : Ideally, this should be mapped to complex(128), but I seem to run into 
	// casting issues in the C code. This is a simple workaround for now - although ugly.
	// Also note that if one used complex types, then one would need to include complex.h at the command line
	extern type fftw_complex = 2*real(64); // 4.1.1
	extern type fftw_plan; // opaque type

	// Planner functions
	// Complex : 4.3.1
	// NOTE : We pass in arrays using ref since we can't pass in multidimensional arrays.
	extern proc fftw_plan_dft_1d(n0 : c_int, ref in1 : fftw_complex, ref out1 : fftw_complex, sign : c_int, flags : c_uint) : fftw_plan;
	extern proc fftw_plan_dft_2d(n0 : c_int, n1 : c_int, ref in1 : fftw_complex, ref out1 : fftw_complex, sign : c_int, flags : c_uint) : fftw_plan;
	extern proc fftw_plan_dft_3d(n0 : c_int, n1 : c_int, n2 : c_int, ref in1 : fftw_complex, ref out1 : fftw_complex, sign : c_int, flags : c_uint) : fftw_plan;


	// Using plans 
	extern proc fftw_execute(const plan : fftw_plan);
	extern proc fftw_destroy_plan(plan : fftw_plan);
	extern proc fftw_cleanup();


}
