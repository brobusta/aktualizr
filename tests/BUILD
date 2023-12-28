package(default_visibility = ["//visibility:public"])

filegroup(
    name = "config",
    srcs = glob(["config/*.toml"]),
)

filegroup(
    name = "test_data",
    srcs = glob(["test_data/**/*"]),
)

filegroup(
    name = "fake_http_server",
    srcs = glob(["fake_http_server/*"]),
)

cc_library(
    name = "test_utils",
    srcs = [
        "httpfake.cc",
        "metafake.cc",
        "test_utils.cc",
    ],
    hdrs = [
        "httpfake.h",
        "metafake.h",
        "test_utils.h",
    ],
    copts = [
        "-Wno-deprecated-declarations",
    ],
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    deps = [
        "//src/libaktualizr",
        "//src/sota_tools:sota_tools_lib",
        "//src/uptane_generator:uptane_generator_lib",
        "@boost//:process",
        "@gtest",
    ],
    alwayslink = True,
)

cc_library(
    name = "uptane_test_common",
    srcs = [],
    hdrs = [
        "uptane_test_common.h",
    ],
    copts = [],
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    deps = [
        "//src/libaktualizr",
        "//src/virtual_secondary",
    ],
    alwayslink = True,
)
