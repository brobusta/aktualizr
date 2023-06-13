load("@bazel_skylib//rules:write_file.bzl", "write_file")
package(default_visibility = ["//visibility:public"])

# config_setting(
#     name = "linux_x86_64",
#     constraint_values = [
#         "@platforms//cpu:x86_64",
#         "@platforms//os:linux",
#     ],
# )

# config_setting(
#     name = "linux_aarch64",
#     constraint_values = [
#         "@platforms//cpu:aarch64",
#         "@platforms//os:linux",
#     ],
# )

write_file(
    name = "config_h",
    out = "config.h",
    content = ["""
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if building universal (internal helper macro) */
/* #undef AC_APPLE_UNIVERSAL_BUILD */

/* Define to 1 if you have the <alloca.h> header file. */
#define HAVE_ALLOCA_H 1

/* Define to 1 if you have the declaration of `alloca strcasecmp', and to 0 if
   you don't. */
#define HAVE_DECL_ALLOCA_STRCASECMP 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `mergesort' function. */
/* #undef HAVE_MERGESORT */

/* Define to 1 if you have the `mkstemps' function. */
#define HAVE_MKSTEMPS 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdio.h> header file. */
#define HAVE_STDIO_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the `strtoimax' function. */
#define HAVE_STRTOIMAX 1

/* Define to 1 if you have the `strtoll' function. */
#define HAVE_STRTOLL 1

/* Define to 1 if you have the symlink function. */
#define HAVE_SYMLINK 1

/* Define to 1 if you have the <sys/param.h> header file. */
#define HAVE_SYS_PARAM_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the `timegm' function. */
#define HAVE_TIMEGM 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory where libtool stores uninstalled libraries. */
#define LT_OBJDIR ".libs/"

/* Name of package */
#define PACKAGE "asn1c"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "vlm@lionet.info"

/* Define to the full name of this package. */
#define PACKAGE_NAME "asn1c"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "asn1c 0.9.28"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "asn1c"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "0.9.28"

/* Define to 1 if all of the C90 standard headers exist (not just the ones
   required in a freestanding environment). This macro is provided for
   backward compatibility; new code need not use it. */
#define STDC_HEADERS 1

/* Define to 1 if your <sys/time.h> declares `struct tm'. */
/* #undef TM_IN_SYS_TIME */

/* Version number of package */
#define VERSION "0.9.28"

/* Define WORDS_BIGENDIAN to 1 if your processor stores words with the most
   significant byte first (like Motorola and SPARC, unlike Intel). */
#if defined AC_APPLE_UNIVERSAL_BUILD
# if defined __BIG_ENDIAN__
#  define WORDS_BIGENDIAN 1
# endif
#else
# ifndef WORDS_BIGENDIAN
/* #  undef WORDS_BIGENDIAN */
# endif
#endif

/* Define to 1 if `lex' declares `yytext' as a `char *' by default, not a
   `char[]'. */
#define YYTEXT_POINTER 1

/* Define to `int64_t' if <sys/types.h> does not define. */
/* #undef intmax_t */

/* Define to `long int' if <sys/types.h> does not define. */
/* #undef off_t */

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */

#ifndef NULL
#define NULL (0)
#endif

"""]
)

filegroup(
    name = "skeletons_readme",
    srcs = ["skeletons/README"],
    visibility = ["//visibility:public"],
)

filegroup(
    name = "skeletons",
    srcs = glob(["skeletons/**/*"]),
    visibility = ["//visibility:public"],
)

cc_binary(
    name = "asn1c",
    srcs = glob([
        "libasn1common/*.c",
        "libasn1common/*.h",
        "libasn1compiler/*.c",
        "libasn1compiler/*.h",
        "libasn1parser/*.c",
        "libasn1parser/*.h",
        "libasn1fix/*.c",
        "libasn1fix/*.h",
        "libasn1print/*.c",
        "libasn1print/*.h",
        "skeletons/*.c",
        "skeletons/*.h",
    ],
    exclude = [
        "libasn1compiler/check_compiler.c",
        "libasn1parser/check_asn1p_integer.c",
        "libasn1fix/check_fixer.c",
        "skeletons/converter-example.c",
    ]) + [
        "asn1c/asn1c.c",
        ":config_h",
    ],
    includes = [
        ".",
    ],
    copts = [
        # "-std=c99",
        "-Iexternal/asn1c",
        "-Iexternal/asn1c/asn1c",
        "-Iexternal/asn1c/libasn1common",
        "-Iexternal/asn1c/libasn1compiler",
        "-Iexternal/asn1c/libasn1parser",
        "-Iexternal/asn1c/libasn1fix",
        "-Iexternal/asn1c/libasn1print",
        "-Iexternal/asn1c/skeletons",
    ],
    local_defines = [
        "HAVE_CONFIG_H",
        "DATADIR=\\\"external/asn1c/skeletons\\\"",
    ],
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
)
