package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sota_tools_lib",
    srcs = [
        "authenticate.cc",
        "server_credentials.cc",
        "oauth2.cc",
        "treehub_server.cc",
    ],
    hdrs = [
        "authenticate.h",
        "server_credentials.h",
        "oauth2.h",
        "treehub_server.h",
    ],
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    alwayslink = True,
    deps = [
        "//src/libaktualizr",
    ],
)
