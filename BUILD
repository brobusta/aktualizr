load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libaktualizr_headers",
    hdrs = glob([
        "include/libaktualizr/**/*.h",
    ]),
    strip_include_prefix = "/include/",
)

cc_library(
    name = "libaktualizr_c_headers",
    hdrs = [
        "include/libaktualizr-c.h",
    ],
    strip_include_prefix = "/include/",
    deps = [
        ":libaktualizr_headers",
    ],
)
