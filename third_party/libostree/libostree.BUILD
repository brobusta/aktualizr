load("@bazel_skylib//rules:write_file.bzl", "write_file")
load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

write_file(
    name = "config_h",
    out = "src/libostree/config.h",
    content = ["""
/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */
"""],
)

write_file(
    name = "ostree_version_h",
    out = "src/libostree/ostree-version.h",
    content = ["""
/*
 * Copyright (C) 2017 Georges Basile Stavracas Neto <georges.stavracas@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2 of the licence or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

/**
 * SECTION:ostree-version
 * @short_description: ostree version checking
 *
 * ostree provides macros to check the version of the library
 * at compile-time
 */

/**
 * OSTREE_YEAR_VERSION:
 *
 * ostree year version component (e.g. 2017 if %OSTREE_VERSION is 2017.2)
 *
 * Since: 2017.4
 */
#define OSTREE_YEAR_VERSION (2023)

/**
 * OSTREE_RELEASE_VERSION:
 *
 * ostree release version component (e.g. 2 if %OSTREE_VERSION is 2017.2)
 *
 * Since: 2017.4
 */
#define OSTREE_RELEASE_VERSION (3)

/**
 * OSTREE_VERSION
 *
 * ostree version.
 *
 * Since: 2017.4
 */
#define OSTREE_VERSION (2023.3)

/**
 * OSTREE_VERSION_S:
 *
 * ostree version, encoded as a string, useful for printing and
 * concatenation.
 *
 * Since: 2017.4
 */
#define OSTREE_VERSION_S "2023.3"

#define OSTREE_ENCODE_VERSION(year,release) \
        ((year) << 16 | (release))

/**
 * OSTREE_VERSION_HEX:
 *
 * ostree version, encoded as an hexadecimal number, useful for
 * integer comparisons.
 *
 * Since: 2017.4
 */
#define OSTREE_VERSION_HEX \
        (OSTREE_ENCODE_VERSION (OSTREE_YEAR_VERSION, OSTREE_RELEASE_VERSION))

/**
 * OSTREE_CHECK_VERSION:
 * @year: required year version
 * @release: required release version
 *
 * Compile-time version checking. Evaluates to %TRUE if the version
 * of ostree is equal or greater than the required one.
 *
 * Since: 2017.4
 */
#define OSTREE_CHECK_VERSION(year,release)   \
        (OSTREE_YEAR_VERSION > (year) || \
         (OSTREE_YEAR_VERSION == (year) && OSTREE_RELEASE_VERSION >= (release)))

/**
 * OSTREE_BUILT_FEATURES:
 *
 * Whitespace separated set of features this libostree was configured with at build time.
 * Consult the source code in configure.ac (or the CLI `ostree --version`) for examples.
 *
 * Since: 2019.3
 */
#ifndef __GI_SCANNER__
#define OSTREE_BUILT_FEATURES "@OSTREE_FEATURES@"
#endif

"""],
)

cc_library(
    name = "libostree",
    hdrs = glob([
        "src/libostree/*.h",
    ]) + [
        ":ostree_version_h",
        ":config_h",
    ],
    srcs = glob([
        "src/libostree/*.c",
    ]),
    strip_include_prefix = "src/libostree/",
    local_defines = [
        "HAVE_CONFIG_H",
    ],
    deps = [
        # "@openssl",
        # "@zlib",
    ],
    linkopts = [
    ],
)

