""" module to load sqlite3 library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_sqlite3_repo():
    http_archive(
        name = "sqlite3",
        build_file = "@//third_party/sqlite3:sqlite3.BUILD",
        sha256 = "833be89b53b3be8b40a2e3d5fedb635080e3edb204957244f3d6987c2bb2345f",
        strip_prefix = "sqlite-amalgamation-3440200",
        urls = [
            "https://www.sqlite.org/2023/sqlite-amalgamation-3440200.zip",
        ],
    )
