""" module to load sodium library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libsodium_repo():
    http_archive(
        name = "libsodium",
        sha256 = "8e5aeca07a723a27bbecc3beef14b0068d37e7fc0e97f51b3f1c82d2a58005c1",
        strip_prefix = "libsodium-1.0.20-RELEASE",
        build_file = "@//third_party/libsodium:libsodium.BUILD",
        urls = [
            "https://github.com/jedisct1/libsodium/archive/refs/tags/1.0.20-RELEASE.tar.gz",
        ],
    )
