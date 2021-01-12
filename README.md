json spirit
===========

A json library using Boost.Spirit V2 and X3 for the parser.

The primary goal of the json spirit library was to create a conformant json parser that resulted in a
json object that could be manipulated much like in javascript or python.


# Quick Example


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


# Building

We have CMake and Boost.Build support.

## CMake
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

### Using in another CMakeLists.txt

One way to use the json library in another CMake project is to treat it as an external project.

Here is an example of how to do that:

```
  # Find json spirit
  ExternalProject_Add(
    dependency.json_spirit
    URL ${CMAKE_CURRENT_SOURCE_DIR}/vendor/json_spirit
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/json_spirit
    CMAKE_COMMAND ${CMAKE_COMMAND}
    CMAKE_GENERATOR ${CMAKE_GENERATOR}
    CMAKE_ARGS ${BOOST_CMAKE_ARGS} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
    BUILD_COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR> --config $<CONFIG> --target json
    INSTALL_COMMAND "")
  
  ExternalProject_Get_Property(dependency.json_spirit SOURCE_DIR BINARY_DIR)
  set(INSTALL_DIR ${BINARY_DIR}/${CMAKE_CFG_INTDIR})
  
  add_library(json_spirit STATIC IMPORTED)
  
  set(libjson_name "${CMAKE_STATIC_LIBRARY_PREFIX}json${CMAKE_STATIC_LIBRARY_SUFFIX}")
  set_property(
    TARGET json_spirit
    PROPERTY IMPORTED_LOCATION ${INSTALL_DIR}/${libjson_name})
  
  set_property(
    TARGET json_spirit
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${SOURCE_DIR})
  
  add_dependencies(json_spirit dependency.json_spirit)
```

and for your specific target you can:

```
    target_link_libraries(my_target
                          json_spirit
                          .... other libraries ... )
```


## Boost.Build
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

## Bazel Build
For bazel depending on this library using the bazel build system add the following to your `WORKSPACE` file

```python
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

git_repository(
  name = "com_github_cierelabs_json_spirit",
  remote = "https://github.com/cierelabs/json_spirit",
  commit = "371c3b5d283f6272860d4d2eac996591ffad259c"
)

load("@com_github_cierelabs_json_spirit//:json_spirit_deps.bzl", "json_spirit_deps")

json_spirit_deps()

load("@com_github_nelhage_rules_boost//:boost/boost.bzl", "boost_deps")

boost_deps()
```

The library can now be depended on using the target `@com_github_cierelabs_json_spirit:json_spirit` e.g.

``` python
cc_library(
  name = "some_lib",
  deps = ["@com_github_cierelabs_json_spirit:json_spirit"],
  # ...
)
```

# Documentation

Coming soon.  Unit tests located in `libs/json/test/*.cpp` provide a good starting point for exploration of the functionality.


# Licensing

`json_spirit` is licensed under the Boost Software License.  See LICENSE_1_0.txt for details.
