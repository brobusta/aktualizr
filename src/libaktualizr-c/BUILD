load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "libaktualizr-c",
    srcs = [
        "libaktualizr-c.cc",
    ],
    linkstatic = True,
    deps = [
        "//:libaktualizr_c_headers",
        "//src/libaktualizr",
    ],
)
