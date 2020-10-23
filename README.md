# EntropyAssessment

Cryptographic random bit generators (RBGs), also known as random number generators (RNGs), require a noise source that produces digital outputs with some level of unpredictability, expressed as min-entropy. [SP 800-90B](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90B.pdf) provides a standardized means of estimating the quality of a source of entropy.

## Installing

### Prerequisites

You will need to have the `bz2` and 'divsufsort' libraries installed, as well as a g++-compatible compiler that suports c++11.
On debian/ubuntu like systems the following command sould be sufficient:
```
sudo apt install libbz2-dev libdivsufsort-dev build-essential
```

You should be using a virtual environment with at least python version 3.

### Using pip

You can simply run:
```
pip install sp800-90b
```

### Using setuptools

Don't forget to check out the `nist_impl` submodule:
```
git submodule init
git submodule update
```

You will also need the `setuptools` and the `pybind11` python package:
```
pip install setuptools pybind11
```

Now you can install the package using:
```
python setup.py build
python setup.py install
```

### Manual compilation

With the same prerequisites as in the setuptools installation you can also run
```
make -f Manual.make
```
to manually compile the package.

**Note:** You will only be able to use it in the directory where the generated `.so` library is present.

## Installing

Use the python command `help(sp800_90b)` for documentation.

## Manual test

The following commands can be used for a crude test of the package:
```
(venv) hnj@prokrastinator:~/sp800_90b$ python
Python 3.6.9 (default, Oct  8 2020, 12:12:24)
Type 'copyright', 'credits' or 'license' for more information
IPython 7.16.1 -- An enhanced Interactive Python. Type '?' for help.

In [1]: import random

In [2]: r = bytes(random.getrandbits(8) for _ in range(1000000))

In [3]: n = bytes(random.getrandbits(8) for _ in range(500000))

In [4]: n = n + bytes(random.getrandbits(7) for _ in range(500000))

In [5]: import sp800_90b

In [6]: help(sp800_90b)


In [7]: rd = sp800_90b.Data(r)

In [8]: rd.iid_tests()
Beginning initial tests...
Beginning permutation tests... these may take some time
Out[8]: True

In [9]: rd.h_initial()
Out[9]: 7.189185202883665

In [10]: nd = sp800_90b.Data(n)

In [11]: nd.iid_tests()
igamc: UNDERFLOW
Out[11]: False

In [12]: nd.h_initial()
Out[12]: 5.194695581531671
```


## Disclaimer

This software is a repackaged and slightly modified version of a NIST-developed project [(that can be found here: https://github.com/usnistgov/SP800-90B_EntropyAssessment)](https://github.com/usnistgov/SP800-90B_EntropyAssessment).

### Original NIST Disclaimer

NIST-developed software is provided by NIST as a public service. You may use, copy and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify and create derivative works of the software or any portion of the software, and you may distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.

### Modifications

The modifications that were made were are:
 - Adding a header file and some helper functions to wrap the provided nist code inside an object file that can be linked to other components ([`src/nist.hpp`](src/nist.hpp) ans [`src/nist.cpp`](src/nist.cpp)).
 - Writing a class that provides the functionality of the nist code as a class interface ([`src/data.hpp`](src/data.hpp) ans [`src/data.cpp`](src/data.cpp)).
 - Adding pybind11 bindings and boilderplate code to create a python package ([`src/bindings.cpp`](src/bindings.cpp), [`setup.py`](setup.py) and [`MANIFEST.in`](MANIFEST.in)).

## More Information

For more information on the nist code that is running, [look into the original repository](https://github.com/usnistgov/SP800-90B_EntropyAssessment).

For more information on the estimation methods, see [SP 800-90B](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90B.pdf).

