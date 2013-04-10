json spirit
===========

A json library using Boost.Spirit 2.x for the parser.

The primary goal of the json spirit library was to create a conformant json parser that resulted in a
json object that could be manipulated much like in javascript or python.

Quick Example
-------------

json spirit
===========

A json library using Boost.Spirit 2.x for the parser.

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

We have Boost.Build support right now. CMake is coming soon.

Assumes either your user-config.jam file has a line like this:

    use-project /boost	:	/sandbox/boost/trunk ;

or set the environment variable BOOST_ROOT to the location of
the boost version you want to use.

You can build the library via:

    cd json_spirit
    bjam json


You can build the tests via:

    cd json_spirit/libs/json/test
    bjam


Documentation
--------------

Coming soon.



