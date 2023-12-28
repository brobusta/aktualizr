load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "aktualizr_info",
    srcs = [
        "aktualizr_info_config.cc",
        "aktualizr_info_config.h",
        "main.cc",
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
