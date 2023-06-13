load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "virtual_secondary",
    srcs = [
        "virtualsecondary.cc",
        "managedsecondary.cc",
    ],
    hdrs = [
        "virtualsecondary.h",
        "managedsecondary.h",
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

