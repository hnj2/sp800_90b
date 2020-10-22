# EntropyAssessment

Cryptographic random bit generators (RBGs), also known as random number generators (RNGs), require a noise source that produces digital outputs with some level of unpredictability, expressed as min-entropy. [SP 800-90B](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90B.pdf) provides a standardized means of estimating the quality of a source of entropy.

## Original Software by NIST

This software is a repackaged and slightly modified version of a NIST-developed project [(that can be found here: https://github.com/usnistgov/SP800-90B_EntropyAssessment)](https://github.com/usnistgov/SP800-90B_EntropyAssessment).

The modifications that were made were are:
    - Adding the function `construct_data_t` in [`src/data_t.h`](src/data_t.h) with code copied from the `read_file_subset` function in [`nist_code/cpp/shared/utils.h`](nist_code/cpp/shared/utils.h).
    - Adding the function `ea_iid` in [`src/iid_lib.cpp`](src/iid_lib.cpp) with code copied from the `main` function in [`nist_code/cpp/iid_main.cpp`](nist_code/cpp/iid_main.cpp).
    - Adding pybind11 boilerplate code to create a python package.

## Disclaimer

NIST-developed software is provided by NIST as a public service. You may use, copy and distribute copies of the software in any medium, provided that you keep intact this entire notice. You may improve, modify and create derivative works of the software or any portion of the software, and you may distribute such modifications or works. Modified works should carry a notice stating that you changed the software and should note the date and nature of any such change. Please explicitly acknowledge the National Institute of Standards and Technology as the source of the software.

## Prerequisites

You will need to have the `bz2` and 'divsufsort' libraries installed.
On debian/ubuntu like systems the following command sould be sufficient:
```
sudo apt install libbz2-dev libdivsufsort-dev
```

You will also need the `pybind11` python package (and probably at least verison 3 of python).
Preferably in a virtual python environment do:
```
pip install pybind11
```

## Setuptools installation

With the `setuptools` package installed you can run
```
python setup.py build
python setup.py install
```
to install the package into your virtual python environment.

## Manual compilation

You can run `make -f Manual.make` to manually compile the package.

## Manual test

The following commands can be used for a crude test of the package:
```
(venv) hnj@prokrastinator:~/sp800_90b$ python
Python 3.6.9 (default, Oct  8 2020, 12:12:24) 
[GCC 8.4.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
>>> import sp800_90b
>>> import random
>>> sp800_90b.entropy_assesment_iid(bytes(random.getrandbits(8) for i in range(1000000)))
Calculating baseline statistics...
H_original: 7.883765
H_bitstring: 0.998292
min(H_original, 8 X H_bitstring): 7.883765

** Passed chi square tests

** Passed length of longest repeated substring test

Beginning initial tests...
Beginning permutation tests... these may take some time
** Passed IID permutation tests

>>> sp800_90b.entropy_assesment_iid(bytes(random.getrandbits(7 + (i // 500000)) for i in range(1000000)))
Calculating baseline statistics...
H_original: 7.322926
H_bitstring: 0.911240
min(H_original, 8 X H_bitstring): 7.289921

igamc: UNDERFLOW
** Failed chi square tests

** Passed length of longest repeated substring test

Beginning initial tests...
Beginning permutation tests... these may take some time
```

## More Information

For more information on the nist code that is running, [look into the original repository](https://github.com/usnistgov/SP800-90B_EntropyAssessment).

For more information on the estimation methods, see [SP 800-90B](https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90B.pdf).

