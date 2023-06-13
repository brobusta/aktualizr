load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sqlite3",
    srcs = ["sqlite3.c"],
    hdrs = ["sqlite3.h"],
    includes = [
        ".",
    ],
    # Some Unix platforms have pthread and/or dlopen in separate libraries.
    linkopts = select({
        "@platforms//os:freebsd": ["-lpthread"],
        "@platforms//os:linux": [
            "-lpthread",
            "-ldl",
        ],
        "@platforms//os:netbsd": ["-lpthread"],
        "@platforms//os:openbsd": ["-lpthread"],
        "//conditions:default": [],
    }),
)
