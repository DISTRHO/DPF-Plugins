# ===========================================================================
#       https://www.gnu.org/software/autoconf-archive/ax_lib_expat.html
# ===========================================================================
#
# SYNOPSIS
#
#   AX_LIB_EXPAT([MINIMUM-VERSION])
#
# DESCRIPTION
#
#   This macro provides tests of availability of Expat XML Parser of
#   particular version or newer. This macro checks for Expat XML Parser
#   headers and libraries and defines compilation flags
#
#   Macro supports following options and their values:
#
#   1) Single-option usage:
#
#     --with-expat      -- yes, no, or path to Expat XML Parser
#                          installation prefix
#
#   2) Three-options usage (all options are required):
#
#     --with-expat=yes
#     --with-expat-inc  -- path to base directory with Expat headers
#     --with-expat-lib  -- linker flags for Expat
#
#   This macro calls:
#
#     AC_SUBST(EXPAT_CFLAGS)
#     AC_SUBST(EXPAT_LIBS)
#     AC_SUBST(EXPAT_LDFLAGS)
#     AC_SUBST(EXPAT_VERSION)  -- only if version requirement is used
#
#   And sets:
#
#     HAVE_EXPAT
#
# LICENSE
#
#   Copyright (c) 2008 Mateusz Loskot <mateusz@loskot.net>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 11

AC_DEFUN([AX_LIB_EXPAT],
[
    AC_ARG_WITH([expat],
        AS_HELP_STRING([--with-expat=@<:@ARG@:>@],
            [use Expat XML Parser from given prefix (ARG=path); check standard prefixes (ARG=yes); disable (ARG=no)]
        ),
        [
        if test "$withval" = "yes"; then
            if test -f /usr/local/include/expat.h ; then
                expat_prefix=/usr/local
            elif test -f /usr/include/expat.h ; then
                expat_prefix=/usr
            else
                expat_prefix=""
            fi
            expat_requested="yes"
        elif test -d "$withval"; then
            expat_prefix="$withval"
            expat_requested="yes"
        else
            expat_prefix=""
            expat_requested="no"
        fi
        ],
        [
        dnl Default behavior is implicit yes
        if test -f /usr/local/include/expat.h ; then
            expat_prefix=/usr/local
        elif test -f /usr/include/expat.h ; then
            expat_prefix=/usr
        else
            expat_prefix=""
        fi
        ]
    )

    AC_ARG_WITH([expat-inc],
        AS_HELP_STRING([--with-expat-inc=@<:@DIR@:>@],
            [path to Expat XML Parser headers]
        ),
        [expat_include_dir="$withval"],
        [expat_include_dir=""]
    )
    AC_ARG_WITH([expat-lib],
        AS_HELP_STRING([--with-expat-lib=@<:@ARG@:>@],
            [link options for Expat XML Parser libraries]
        ),
        [expat_lib_flags="$withval"],
        [expat_lib_flags=""]
    )

    EXPAT_CFLAGS=""
    EXPAT_LIBS=""
    EXPAT_VERSION=""

    dnl
    dnl Collect include/lib paths and flags
    dnl
    run_expat_test="no"

    if test -n "$expat_prefix"; then
        expat_include_dir="$expat_prefix/include"
        expat_ld_flags="-L$expat_prefix/lib"
        expat_lib_flags="-lexpat"
        run_expat_test="yes"
    elif test "$expat_requested" = "yes"; then
        if test -n "$expat_include_dir" -a -n "$expat_lib_flags"; then
            run_expat_test="yes"
        fi
    else
        run_expat_test="no"
    fi

    dnl
    dnl Check Expat XML Parser files
    dnl
    if test "$run_expat_test" = "yes"; then

        saved_CPPFLAGS="$CPPFLAGS"
        CPPFLAGS="$CPPFLAGS -I$expat_include_dir"

        saved_LIBS="$LIBS"
        LIBS="$LIBS $expat_lib_flags"

        saved_LDFLAGS="$LDFLAGS"
        LDFLAGS="$LDFLAGS $expat_ld_flags"

        dnl
        dnl Check Expat headers
        dnl
        AC_MSG_CHECKING([for Expat XML Parser headers in $expat_include_dir])

        AC_LANG_PUSH([C++])
        AC_COMPILE_IFELSE([
            AC_LANG_PROGRAM(
                [[
@%:@include <expat.h>
                ]],
                [[]]
            )],
            [
            EXPAT_CFLAGS="-I$expat_include_dir"
            expat_header_found="yes"
            AC_MSG_RESULT([found])
            ],
            [
            expat_header_found="no"
            AC_MSG_RESULT([not found])
            ]
        )
        AC_LANG_POP([C++])

        dnl
        dnl Check Expat libraries
        dnl
        if test "$expat_header_found" = "yes"; then

            AC_MSG_CHECKING([for Expat XML Parser libraries])

            AC_LANG_PUSH([C++])
            AC_LINK_IFELSE([
                AC_LANG_PROGRAM(
                    [[
@%:@include <expat.h>
                    ]],
                    [[
XML_Parser p = XML_ParserCreate(NULL);
XML_ParserFree(p);
p = NULL;
                    ]]
                )],
                [
                EXPAT_LIBS="$expat_lib_flags"
                EXPAT_LDFLAGS="$expat_ld_flags"
                expat_lib_found="yes"
                AC_MSG_RESULT([found])
                ],
                [
                expat_lib_found="no"
                AC_MSG_RESULT([not found])
                ]
            )
            AC_LANG_POP([C++])
        fi

        CPPFLAGS="$saved_CPPFLAGS"
        LDFLAGS="$saved_LDFLAGS"
        LIBS="$saved_LIBS"
    fi

    AC_MSG_CHECKING([for Expat XML Parser])

    if test "$run_expat_test" = "yes"; then
        if test "$expat_header_found" = "yes" -a "$expat_lib_found" = "yes"; then

            AC_SUBST([EXPAT_CFLAGS])
            AC_SUBST([EXPAT_LDFLAGS])
            AC_SUBST([EXPAT_LIBS])

            HAVE_EXPAT="yes"
        else
            HAVE_EXPAT="no"
        fi

        AC_MSG_RESULT([$HAVE_EXPAT])

        dnl
        dnl Check Expat version
        dnl
        if test "$HAVE_EXPAT" = "yes"; then

            expat_version_req=ifelse([$1], [], [], [$1])

            if test  -n "$expat_version_req"; then

                AC_MSG_CHECKING([if Expat XML Parser version is >= $expat_version_req])

                if test -f "$expat_include_dir/expat.h"; then

                    expat_major=`cat $expat_include_dir/expat.h | \
                                    grep '^#define.*XML_MAJOR_VERSION.*[0-9]$' | \
                                    sed -e 's/#define XML_MAJOR_VERSION.//'`

                    expat_minor=`cat $expat_include_dir/expat.h | \
                                    grep '^#define.*XML_MINOR_VERSION.*[0-9]$' | \
                                    sed -e 's/#define XML_MINOR_VERSION.//'`

                    expat_revision=`cat $expat_include_dir/expat.h | \
                                    grep '^#define.*XML_MICRO_VERSION.*[0-9]$' | \
                                    sed -e 's/#define XML_MICRO_VERSION.//'`

                    EXPAT_VERSION="$expat_major.$expat_minor.$expat_revision"
                    AC_SUBST([EXPAT_VERSION])

                    dnl Decompose required version string and calculate numerical representation
                    expat_version_req_major=`expr $expat_version_req : '\([[0-9]]*\)'`
                    expat_version_req_minor=`expr $expat_version_req : '[[0-9]]*\.\([[0-9]]*\)'`
                    expat_version_req_revision=`expr $expat_version_req : '[[0-9]]*\.[[0-9]]*\.\([[0-9]]*\)'`
                    if test "x$expat_version_req_revision" = "x"; then
                        expat_version_req_revision="0"
                    fi

                    expat_version_req_number=`expr $expat_version_req_major \* 10000 \
                                               \+ $expat_version_req_minor \* 100 \
                                               \+ $expat_version_req_revision`

                    dnl Calculate numerical representation of detected version
                    expat_version_number=`expr $expat_major \* 10000 \
                                          \+ $expat_minor \* 100 \
                                           \+ $expat_revision`

                    expat_version_check=`expr $expat_version_number \>\= $expat_version_req_number`
                    if test "$expat_version_check" = "1"; then
                        AC_MSG_RESULT([yes])
                    else
                        AC_MSG_RESULT([no])
                        AC_MSG_WARN([Found Expat XML Parser $EXPAT_VERSION, which is older than required. Possible compilation failure.])
                    fi
                else
                    AC_MSG_RESULT([no])
                    AC_MSG_WARN([Missing expat.h header. Unable to determine Expat version.])
                fi
            fi
        fi

    else
        HAVE_EXPAT="no"
        AC_MSG_RESULT([$HAVE_EXPAT])

        if test "$expat_requested" = "yes"; then
            AC_MSG_WARN([Expat XML Parser support requested but headers or library not found. Specify valid prefix of Expat using --with-expat=@<:@DIR@:>@ or provide include directory and linker flags using --with-expat-inc and --with-expat-lib])
        fi
    fi
])
