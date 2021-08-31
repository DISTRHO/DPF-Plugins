# ============================================================================
#  https://www.gnu.org/software/autoconf-archive/ax_prototype_setsockopt.html
# ============================================================================
#
# SYNOPSIS
#
#   AX_PROTOTYPE_SETSOCKOPT
#
# DESCRIPTION
#
#   Requires the AX_PROTOTYPE macro. FIXME: Put this in the code!
#
#   Find the type of argument three of setsockopt. User must include the
#   following in acconfig.h:
#
#    /* Type of third argument of setsockopt */
#    #undef SETSOCKOPT_ARG3
#
# LICENSE
#
#   Copyright (c) 2008 Loic Dachary <loic@senga.org>
#
#   This program is free software; you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by the
#   Free Software Foundation; either version 2 of the License, or (at your
#   option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#   Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program. If not, see <https://www.gnu.org/licenses/>.
#
#   As a special exception, the respective Autoconf Macro's copyright owner
#   gives unlimited permission to copy, distribute and modify the configure
#   scripts that are the output of Autoconf when processing the Macro. You
#   need not follow the terms of the GNU General Public License when using
#   or distributing such scripts, even though portions of the text of the
#   Macro appear in them. The GNU General Public License (GPL) does govern
#   all other use of the material that constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the Autoconf
#   Macro released by the Autoconf Archive. When you make and distribute a
#   modified version of the Autoconf Macro, you may extend this special
#   exception to the GPL to apply to your modified version as well.

#serial 6

AU_ALIAS([AC_PROTOTYPE_SETSOCKOPT], [AX_PROTOTYPE_SETSOCKOPT])
AC_DEFUN([AX_PROTOTYPE_SETSOCKOPT],[
AX_PROTOTYPE(setsockopt,
 [
  #include <sys/types.h>
  #include <sys/socket.h>
 ],
 [
  int a = 0;
  ARG3 b = 0;
  setsockopt(a, SOL_SOCKET, SO_REUSEADDR, b, sizeof(a));
 ],
 ARG3, [const void*, const char*, void*, char*])
])
