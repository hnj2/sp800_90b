# This is a modified verison of the pybind11 example project setup.py
# See: https://github.com/pybind/python_example
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext

import sys
import setuptools

__version__ = '0.1.1'

# read the contents of your README file
from os import path
this_directory = path.abspath(path.dirname(__file__))
with open(path.join(this_directory, 'README.md'), encoding='utf-8') as f:
    long_description = f.read()

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
        sorted(['src/bindings.cpp', 'src/data.cpp', 'src/nist.cpp']),
        include_dirs=[
            # Path to pybind11 headers
            get_pybind_include(),
            'nist_impl/cpp',
        ],
        extra_compile_args = ['-std=c++11'],
        libraries = [
            'bz2', 'pthread', 'divsufsort',
        ],
        extra_link_args = ['-fopenmp'],
        language='c++'
    ),
]

setup(
    name='sp800_90b',
    version=__version__,
    author='Hans Niklas Jacob',
    author_email='hnj@posteo.de',
    url='https://github.com/hnj2/sp800_90b',
    description='SP 800-90B entropy assesment',
    long_description=long_description,
    long_description_content_type='text/markdown',
    ext_modules=ext_modules,
    setup_requires=['pybind11>=2.5.0'],
    #cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
