from distutils.core import setup, Extension

module1 = Extension('dparser_swigc',
                    sources = ['dparser_wrap.c', 'pydparser.c'],
                    define_macros = [('SWIG_GLOBAL', None)],
		    libraries = ['dparse'],
                    extra_compile_args = ['-Wall'])

setup(name = 'dparser_swigc',
       version = '0.1',
       description = 'dparser for python',
       ext_modules = [module1])

setup(name="dparser",
      version="0.1",
      description = 'dparser for python',
      py_modules=["dparser"])
