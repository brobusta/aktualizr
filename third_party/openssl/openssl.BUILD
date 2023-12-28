load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
)

_CONFIGURE_OPTIONS = [
    "no-comp",
    "no-idea",
    "no-weak-ssl-ciphers",
    "no-shared",
]

configure_make(
    name = "openssl",
    args = [
        "-j8",
    ],
    configure_command = "Configure",
    configure_in_place = True,
    configure_options = select({
        "@platforms//cpu:aarch64": ["linux-aarch64"],
        "//conditions:default": ["linux-x86_64"],
    }) + _CONFIGURE_OPTIONS,
    lib_name = "openssl",
    lib_source = ":all_srcs",
    out_static_libs = [
        "libssl.a",
        "libcrypto.a",
    ],
    targets = [
        "build_libs",
        "install_dev",
    ],
    visibility = ["//visibility:public"],
)
