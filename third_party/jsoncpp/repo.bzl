""" module to load jsoncpp library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_jsoncpp_repo():
    http_archive(
        name = "jsoncpp",
        sha256 = "f93b6dd7ce796b13d02c108bc9f79812245a82e577581c4c9aabe57075c90ea2",
        strip_prefix = "jsoncpp-1.9.6",
        urls = [
            "https://github.com/open-source-parsers/jsoncpp/archive/refs/tags/1.9.6.tar.gz",
        ],
    )
