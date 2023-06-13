""" module to load sqlite3 library """

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

def load_sqlite3_repo():
    http_archive(
        name = "sqlite3",
        build_file = "@//third_party/sqlite3:sqlite3.BUILD",
        sha256 = "1cc824d0f5e675829fa37018318fda833ea56f7e9de2b41eddd9f7643b5ec29e",
        strip_prefix = "sqlite-amalgamation-3420000",
        urls = [
            "https://www.sqlite.org/2023/sqlite-amalgamation-3420000.zip",
        ],
    )
