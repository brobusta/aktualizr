load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

package(default_visibility = ["//visibility:public"])

genrule(
    name = "gen_sql_schemas",
    srcs = ["storage/embed_schemas.py"],
    outs = ["storage/sql_schemas.cc"],
    cmd = ("python3 $< " +
           "$(location //config:sql_schema) " +
           "$@ libaktualizr"),
    tools = [
        "//config:sql_migration",
        "//config:sql_schema",
    ],
)

cc_library(
    name = "libaktualizr",
    srcs = glob(
        ["**/*.cc"],
        exclude = [
            "crypto/p11engine_dummy.cc",
            "**/*_test.cc",
            "**/*_test_*.cc",
            "**/*ostree*.cc",
        ],
    ) + [":gen_sql_schemas"],
    hdrs = glob(
        ["**/*.h"],
        exclude = ["**/*ostree*.h"],
    ),
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    deps = [
        "//:libaktualizr_headers",
        "@boost//:filesystem",
        "@boost//:log",
        "@boost//:program_options",
        "@boost//:property_tree",
        "@boost//:signals2",
        "@boost//:thread",
        "@boost//:uuid",
        "@curl",
        "@gtest",
        "@jsoncpp",
        "@libarchive",
        "@libp11",
        "@libsodium",
        "@openssl",
        "@sqlite3",
    ],
    alwayslink = True,
)

cc_test(
    name = "storage_atomic_test",
    size = "small",
    srcs = ["storage/storage_atomic_test.cc"],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "sql_utils_test",
    size = "small",
    srcs = ["storage/sql_utils_test.cc"],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "storage_common_test",
    size = "small",
    srcs = ["storage/storage_common_test.cc"],
    deps = [
        "//src/libaktualizr",
        "//src/uptane_generator:uptane_generator_lib",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "bootloader_test",
    size = "small",
    srcs = ["bootloader/bootloader_test.cc"],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

filegroup(
    name = "campaign_test_data",
    srcs = ["campaign/test/campaigns_sample.json"],
)

cc_test(
    name = "campaign_test",
    size = "small",
    srcs = ["campaign/campaign_test.cc"],
    args = [
        "$(location :campaign_test_data)",
    ],
    data = [
        ":campaign_test_data",
    ],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "crypto_test",
    size = "small",
    srcs = ["crypto/crypto_test.cc"],
    data = [
        "//tests:test_data",
    ],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "hash_test",
    size = "small",
    srcs = ["crypto/hash_test.cc"],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "keymanager_test",
    size = "small",
    srcs = ["crypto/keymanager_test.cc"],
    data = [
        "//tests:test_data",
    ],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "httpclient_test",
    size = "small",
    srcs = ["http/httpclient_test.cc"],
    data = [
        "//tests:fake_http_server",
    ],
    deps = [
        "//src/libaktualizr",
        "//tests:test_utils",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "fetcher_death_test",
    size = "medium",
    srcs = ["package_manager/fetcher_death_test.cc"],
    data = [
        "//tests:fake_http_server",
    ],
    deps = [
        "//src/libaktualizr",
        "//tests:test_utils",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "fetcher_test",
    size = "small",
    srcs = ["package_manager/fetcher_test.cc"],
    data = [
        "//tests:fake_http_server",
    ],
    deps = [
        "//src/libaktualizr",
        "//tests:test_utils",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "packagemanagerconfig_test",
    size = "small",
    srcs = ["package_manager/packagemanagerconfig_test.cc"],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "packagemanagerfactory_test",
    size = "small",
    srcs = ["package_manager/packagemanagerfactory_test.cc"],
    local_defines = [
        "__NO_MAIN__",
    ],
    deps = [
        "//src/libaktualizr",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "packagemanagerfake_test",
    size = "small",
    srcs = ["package_manager/packagemanagerfake_test.cc"],
    local_defines = [
        "__NO_MAIN__",
    ],
    deps = [
        "//src/libaktualizr",
        "//tests:test_utils",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)

cc_test(
    name = "aktualizr_test",
    size = "small",
    srcs = ["primary/aktualizr_test.cc"],
    args = [
        "$(locations //tests/uptane_repo_generation:uptane_repo_full_no_correlation_id)",
    ],
    data = [
        "//tests:config",
        "//tests:test_data",
        "//tests/uptane_repo_generation:uptane_repo_full_no_correlation_id",
    ],
    deps = [
        "//src/libaktualizr",
        "//tests:test_utils",
        "//tests:uptane_test_common",
        "@gtest//:gtest_main",
    ],
    linkstatic = True,
)
