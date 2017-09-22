dnl $Id$
dnl config.m4 for extension session_json_handler

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(session_json_handler, for session_json_handler support,
dnl Make sure that the comment is aligned:
dnl [  --with-session_json_handler             Include session_json_handler support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(session_json_handler, whether to enable session_json_handler support,
dnl Make sure that the comment is aligned:
dnl [  --enable-session_json_handler           Enable session_json_handler support])

if test "$PHP_SESSION_JSON_HANDLER" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-session_json_handler -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/session_json_handler.h"  # you most likely want to change this
  dnl if test -r $PHP_SESSION_JSON_HANDLER/$SEARCH_FOR; then # path given as parameter
  dnl   SESSION_JSON_HANDLER_DIR=$PHP_SESSION_JSON_HANDLER
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for session_json_handler files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SESSION_JSON_HANDLER_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SESSION_JSON_HANDLER_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the session_json_handler distribution])
  dnl fi

  dnl # --with-session_json_handler -> add include path
  dnl PHP_ADD_INCLUDE($SESSION_JSON_HANDLER_DIR/include)

  dnl # --with-session_json_handler -> check for lib and symbol presence
  dnl LIBNAME=session_json_handler # you may want to change this
  dnl LIBSYMBOL=session_json_handler # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SESSION_JSON_HANDLER_DIR/$PHP_LIBDIR, SESSION_JSON_HANDLER_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_SESSION_JSON_HANDLERLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong session_json_handler lib version or lib not found])
  dnl ],[
  dnl   -L$SESSION_JSON_HANDLER_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SESSION_JSON_HANDLER_SHARED_LIBADD)

  PHP_NEW_EXTENSION(session_json_handler, session_json_handler.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
