from distutils.core import setup, Extension
from distutils.command.install_data import install_data
import os, sys

#Pete Shinner's distutils data file fix... from distutils-sig
#data installer with improved intelligence over distutils
#data files are copied into the project directory instead
#of willy-nilly
class smart_install_data(install_data):
    def run(self):
        #need to change self.install_dir to the library dir
        install_cmd = self.get_finalized_command('install')
        self.install_dir = getattr(install_cmd, 'install_lib')
        return install_data.run(self)

module_swigc = Extension('dparser_swigc',
    sources = ['dparser_wrap.c', 'pydparser.c', 'make_tables.c'],
    define_macros = [('SWIG_GLOBAL', None)],
                         libraries = ['mkdparse', 'dparse'],
                         library_dirs = ['../'],
                         extra_compile_args = ['-Wall'])
    
setup(name="dparser",
    cmdclass = {"install_data": smart_install_data},
    version = "1.9",
    description = 'DParser for Python',
    py_modules = ["dparser"],
    ext_modules = [module_swigc],
)
