load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

HERMETIC_CC_TOOLCHAIN_VERSION = "v2.1.3"

http_archive(
    name = "hermetic_cc_toolchain",
    sha256 = "a5caccbf6d86d4f60afd45b541a05ca4cc3f5f523aec7d3f7711e584600fb075",
    urls = [
        "https://mirror.bazel.build/github.com/uber/hermetic_cc_toolchain/releases/download/{0}/hermetic_cc_toolchain-{0}.tar.gz".format(HERMETIC_CC_TOOLCHAIN_VERSION),
        "https://github.com/uber/hermetic_cc_toolchain/releases/download/{0}/hermetic_cc_toolchain-{0}.tar.gz".format(HERMETIC_CC_TOOLCHAIN_VERSION),
    ],
)

load("@hermetic_cc_toolchain//toolchain:defs.bzl", zig_toolchains = "toolchains")

zig_toolchains()

register_toolchains(
    "@zig_sdk//toolchain:linux_amd64_gnu.2.34",
    "@zig_sdk//toolchain:linux_arm64_gnu.2.34",
)

# Load rules_foreign_cc
http_archive(
    name = "rules_foreign_cc",
    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
    strip_prefix = "rules_foreign_cc-0.9.0",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.9.0.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()

# Load all external repos:
load("//third_party:repos.bzl", "load_external_repos")

load_external_repos()
