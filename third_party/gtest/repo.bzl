""" module to load gtest library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_gtest_repo():
    http_archive(
        name = "gtest",
        sha256 = "f179ec217f9b3b3f3c6e8b02d3e7eda997b49e4ce26d6b235c9053bec9c0bf9f",
        strip_prefix = "googletest-1.15.2",
        urls = [
            "https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip",
        ],
    )
