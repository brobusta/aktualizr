package(default_visibility = ["//visibility:public"])

cc_library(
    name = "sota_tools_lib",
    srcs = [
        "authenticate.cc",
        "oauth2.cc",
        "server_credentials.cc",
        "treehub_server.cc",
    ],
    hdrs = [
        "authenticate.h",
        "oauth2.h",
        "server_credentials.h",
        "treehub_server.h",
    ],
    includes = [
        ".",
    ],
    linkopts = [],
    linkstatic = True,
    deps = [
        "//src/libaktualizr",
    ],
    alwayslink = True,
)
