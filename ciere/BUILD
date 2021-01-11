cc_library(
    name = "ciere",
    hdrs = glob(["**/*.hpp"]),
    defines = ["BOOST_SPIRIT_USE_PHOENIX_V3"],
    visibility = ["//libs/json/src:__pkg__"],
    deps = [
        "@boost//:container",
        "@boost//:conversion",
        "@boost//:endian",
        "@boost//:spirit",
        "@boost//:variant",
    ],
)
