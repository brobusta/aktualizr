""" module to load curl library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

def load_curl_repo():
    http_archive(
        name = "curl",
        build_file = "@//third_party/curl:curl.BUILD",
        sha256 = "8117d24a8c29a0c3aa160703eb487694f3d4aa72ea2067b45beb439ea4d47668",
        strip_prefix = "curl-curl-8_5_0",
        urls = [
            "https://github.com/curl/curl/archive/refs/tags/curl-8_5_0.tar.gz",
        ],
    )
