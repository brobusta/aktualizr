""" module to load gtest library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_gtest_repo():
    # GoogleTest/GoogleMock framework. Used by most unit-tests.
    # https://github.com/google/googletest/releases
    http_archive(
        name = "gtest",
        sha256 = "353571c2440176ded91c2de6d6cd88ddd41401d14692ec1f99e35d013feda55a",
        strip_prefix = "googletest-release-1.11.0",
        urls = [
            "https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip",
        ],
    )
