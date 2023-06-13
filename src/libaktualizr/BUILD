load("@rules_cc//cc:defs.bzl", "cc_library")

package(default_visibility = ["//visibility:public"])

genrule(
    name = "gen_sql_schemas",
    srcs = ["storage/embed_schemas.py"],
    outs = ["storage/sql_schemas.cc"],
    cmd = ("python3 $< " +
           "$(location //config:sql_schema) " +
           "$@ libaktualizr"),
    tools = [
        "//config:sql_schema",
        "//config:sql_migration",
    ],
)

cc_library(
    name = "libaktualizr",
    srcs = glob(["**/*.cc"], exclude = [
        "crypto/p11engine_dummy.cc",
        "**/*_test.cc",
        "**/*_test_*.cc",
        "**/*ostree*.cc",
    ]) + [":gen_sql_schemas"],
    hdrs = glob(["**/*.h"], exclude=["**/*ostree*.h"]),
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    alwayslink = True,
    deps = [
        "//:libaktualizr_headers",
        "@openssl",
        "@boost//:filesystem",
        "@boost//:program_options",
        "@boost//:property_tree",
        "@boost//:thread",
        "@boost//:log",
        "@boost//:signals2",
        "@boost//:uuid",
        "@curl",
        "@jsoncpp",
        "@libsodium",
        "@libp11",
        "@gtest",
        "@sqlite3",
        "@libarchive",
    ],
)

