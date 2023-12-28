load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "aktualizr",
    srcs = [
        "main.cc",
        "secondary.cc",
        "secondary.h",
        "secondary_config.cc",
        "secondary_config.h",
    ],
    includes = [
        ".",
    ],
    linkstatic = True,
    deps = [
        "//:libaktualizr_headers",
        "//src/libaktualizr",
        "//src/libaktualizr-posix:aktualizr-posix",
        "//src/virtual_secondary",
    ],
)
