""" module to load sodium library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_libsodium_repo():
    http_archive(
        name = "libsodium",
        sha256 = "1d281a8a5e299a38e5c16ff60f293bba0796dc0fda8e49bc582d4bc1935572ed",
        strip_prefix = "libsodium-1.0.19",
        build_file = "@//third_party/libsodium:libsodium.BUILD",
        urls = [
            "https://github.com/jedisct1/libsodium/archive/refs/tags/1.0.19.tar.gz",
        ],
    )
