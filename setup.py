# This is a modified verison of the pybind11 example project setup.py
# See: https://github.com/pybind/python_example
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

import sys
import setuptools

__version__ = '0.0.1'


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the ``get_include()``
    method can be invoked. """

    def __str__(self):
        import pybind11
        return pybind11.get_include()


ext_modules = [
    Extension(
        'sp800_90b',
        # Sort input source files to ensure bit-for-bit reproducible builds
        # (https://github.com/pybind/python_example/pull/53)
        sorted(['src/iid_lib.cpp']),
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
        ],
        language='c++'
    ),
]


def cpp_flag(compiler):
    """Return the -std=c++[11/14/17] compiler flag.
    The newer version is prefered over c++11 (when it is available).
    """
    flags = ['-std=c++17', '-std=c++14', '-std=c++11']

    for flag in flags:
        if has_flag(compiler, flag):
            return flag

    raise RuntimeError('Unsupported compiler -- at least C++11 support '
                       'is needed!')


class BuildExt(build_ext):
    """A custom build extension for adding compiler-specific options."""

    c_opts = ['-fopenmp', '-Inist_impl/cpp', '-std=c++11']
    l_opts = ['-fopenmp', '-lbz2', '-lpthread', '-ldivsufsort']

    def build_extensions(self):

        if not self.compiler.compiler_type == 'unix':
            raise RuntimeError('Unsupported compiler -- need a unix compiler like g++')

        for ext in self.extensions:
            ext.define_macros = [('VERSION_INFO', '"{}"'.format(self.distribution.get_version()))]
            ext.extra_compile_args = self.c_opts
            ext.extra_link_args = self.l_opts

        build_ext.build_extensions(self)

setup(
    name='sp800_90b',
    version=__version__,
    author='Hans Niklas Jacob',
    author_email='hnj@posteo.de',
    url='https://github.com/hnj2/sp800_90b',
    description='SP 800-90B entropy assesment',
    long_description='Uses the nist-provided implementation available here: https://github.com/usnistgov/SP800-90B_EntropyAssessment',
    ext_modules=ext_modules,
    setup_requires=['pybind11>=2.5.0'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
