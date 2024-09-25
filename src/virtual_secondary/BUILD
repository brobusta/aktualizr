load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "virtual_secondary",
    srcs = [
        "managedsecondary.cc",
        "virtualsecondary.cc",
    ],
    hdrs = [
        "managedsecondary.h",
        "virtualsecondary.h",
    ],
    includes = [
        ".",
    ],
    linkstatic = True,
    deps = [
        "//:libaktualizr_headers",
        "//src/libaktualizr",
    ],
)

cc_test(
    name = "virtual_secondary_test",
    size = "small",
    srcs = ["virtual_secondary_test.cc"],
    data = [
        "//tests:config",
        "//tests:test_data",
    ],
    deps = [
        ":virtual_secondary",
        "//src/uptane_generator:uptane_generator_lib",
        "//tests:test_utils",
        "//tests:uptane_test_common",
        "@googletest//:gtest_main",
    ],
    linkstatic = True,
)
