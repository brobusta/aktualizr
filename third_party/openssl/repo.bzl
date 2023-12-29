load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_openssl_repo():
    http_archive(
        name = "openssl",
        build_file = "@//third_party/openssl:openssl.BUILD",
        sha256 = "cf3098950cb4d853ad95c0841f1f9c6d3dc102dccfcacd521d93925208b76ac8",
        strip_prefix = "openssl-1.1.1w",
        urls = [
            "https://www.openssl.org/source/openssl-1.1.1w.tar.gz",
            "https://github.com/openssl/openssl/archive/OpenSSL_1_1_1w.tar.gz",
        ],
    )
