""" module to load curl library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive", "http_file")

def load_curl_repo():
    http_archive(
        name = "curl",
        build_file = "@//third_party/curl:curl.BUILD",
        # sha256 = "8085bfc250ffa6f3b5b1b05bea42b368a0361ede004f7c2b9edf102a59e8efb4",
        strip_prefix = "curl-curl-8_1_2",
        urls = [
            "https://github.com/curl/curl/archive/refs/tags/curl-8_1_2.tar.gz",
        ],
    )

