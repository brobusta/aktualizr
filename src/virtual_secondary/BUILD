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
