load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "uptane_generator_lib",
    srcs = [
        "director_repo.cc",
        "image_repo.cc",
        "repo.cc",
        "uptane_repo.cc",
    ],
    hdrs = [
        "director_repo.h",
        "image_repo.h",
        "repo.h",
        "uptane_repo.h",
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

cc_binary(
    name = "uptane_generator",
    srcs = [
        "main.cc",
    ],
    includes = [
        ".",
    ],
    deps = [
        ":uptane_generator_lib",
        "//src/libaktualizr",
    ],
)

cc_test(
    name = "repo_test",
    size = "small",
    srcs = ["repo_test.cc"],
    args = [
        "$(location //src/uptane_generator)",
    ],
    data = [
        ":uptane_generator",
        "//tests:test_data",
    ],
    deps = [
        ":uptane_generator_lib",
        "//tests:test_utils",
        "@googletest//:gtest_main",
    ],
    linkstatic = True,
)
