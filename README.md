json spirit
===========

A json library using Boost.Spirit V2 and X3 for the parser.

The primary goal of the json spirit library was to create a conformant json parser that resulted in a
json object that could be manipulated much like in javascript or python.

Quick Example
-------------

```c++
// the json::value is the primary data type
// you can simply assign it values

json::value v = 42;
v = "foo";
v = true;
v = 14.5;

// you can also treat the json::value like an array
v[0] = "foo";
v[1] = true;

// or a json object
v["foo"] = 42;
v["bar"] = false;

// and of course, these can be more complex
json::value z;
z[4] = v;

std::cout << z << "\n";
```

which would result in:

```
[null, null, null, null, {"bar":false, "foo":42}]
```


Building
---------

We have Boost.Build and CMake support.

For Boost.Build, we assume either your user-config.jam file has a line like this:

    use-project /boost	:	/sandbox/boost/trunk ;

or you have set the environment variable BOOST_ROOT to the location of
the boost version you want to use.

You can build the library via:

    cd json_spirit
    bjam json


You can build the tests via:

    cd json_spirit/libs/json/test
    bjam

For CMake, do this to build the library:

    cd json_spirit
    mkdir build
    cd build
    cmake ..
    make

You may specify the version of Boost to build against by defining BOOST_ROOT on the CMake
command line like this:

    cmake -DBOOST_ROOT=/sandbox/boost/trunk ..

To run the unit tests:

    make test

Documentation
--------------

Coming soon.  Unit tests located in libs/json/test/*.cpp provide a good starting point for exploration of the functionality.

Licensing
---------
json_spirit is licensed under the Boost Software License.  See LICENSE_1_0.txt for details.
