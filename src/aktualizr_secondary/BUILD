load("@rules_cc//cc:defs.bzl", "cc_binary")

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "aktualizr-secondary",
    srcs = [
        "main.cc",
    ],
    includes = [
        ".",
    ],
    linkstatic = True,
    deps = [
        ":aktualizr_secondary_lib",
        "@boost.filesystem",
        "@boost.program_options",
    ],
)

cc_library(
    name = "aktualizr_secondary_lib",
    srcs = [
      "aktualizr_secondary.cc",
      "aktualizr_secondary_config.cc",
      "aktualizr_secondary_file.cc",
      "msg_handler.cc",
      "secondary_tcp_server.cc",
      "update_agent_file.cc",
    ],
    hdrs = [
      "aktualizr_secondary.h",
      "aktualizr_secondary_config.h",
      "aktualizr_secondary_file.h",
      "msg_handler.h",
      "secondary_tcp_server.h",
      "update_agent_file.h",
      "update_agent.h",
    ],
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    deps = [
        "//:libaktualizr_headers",
        "//src/libaktualizr",
        "//src/libaktualizr-posix:aktualizr-posix",
    ],
    alwayslink = True,
)

cc_test(
    name = "aktualizr_secondary_config_test",
    size = "small",
    srcs = ["aktualizr_secondary_config_test.cc"],
    data = [
        "//tests:config",
    ],
    deps = [
        ":aktualizr_secondary_lib",
        "@googletest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "aktualizr_secondary_test",
    size = "small",
    srcs = ["aktualizr_secondary_test.cc"],
    tags = ["manual"],
    data = [
        "//tests:config",
    ],
    deps = [
        ":aktualizr_secondary_lib",
        "//tests:test_utils",
        "//src/uptane_generator:uptane_generator_lib",
        "@googletest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "secondary_rpc_test",
    size = "small",
    srcs = ["secondary_rpc_test.cc"],
    data = [
        "//tests:config",
    ],
    deps = [
        ":aktualizr_secondary_lib",
        "//tests:test_utils",
        "@googletest//:gtest_main",
    ],
    linkstatic = True,
)
