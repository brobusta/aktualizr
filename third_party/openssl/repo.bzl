load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_openssl_repo():
    http_archive(
        name = "openssl",
        build_file = "@//third_party/openssl:openssl.BUILD",
        sha256 = "e2f8d84b523eecd06c7be7626830370300fbcc15386bf5142d72758f6963ebc6",
        strip_prefix = "openssl-1.1.1u",
        urls = [
            "https://www.openssl.org/source/openssl-1.1.1u.tar.gz",
            "https://github.com/openssl/openssl/archive/OpenSSL_1_1_1u.tar.gz",
        ],
    )

