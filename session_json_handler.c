/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "zend_smart_str.h"

#include "ext/standard/info.h"
#include "php_session_json_handler.h"

#include "ext/session/php_session.h" /* for php_session_register_serializer */



#define IF_SESSION_VARS() \
	if (Z_ISREF_P(&PS(http_session_vars)) && Z_TYPE_P(Z_REFVAL(PS(http_session_vars))) == IS_ARRAY)


/* If you declare any globals in php_session_json_handler.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(session_json_handler)
*/

/* True global resources - no need for thread safety here */
static int le_session_json_handler;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("session_json_handler.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_session_json_handler_globals, session_json_handler_globals)
    STD_PHP_INI_ENTRY("session_json_handler.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_session_json_handler_globals, session_json_handler_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_session_json_handler_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_session_json_handler_compiled)
{
	char *arg = NULL;
	size_t arg_len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "session_json_handler", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_session_json_handler_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_session_json_handler_init_globals(zend_session_json_handler_globals *session_json_handler_globals)
{
	session_json_handler_globals->global_value = 0;
	session_json_handler_globals->global_string = NULL;
}
*/
/* }}} */









PS_SERIALIZER_ENCODE_FUNC(json){


	smart_str buf = {0};

	char *retstr;
	zval funname;
	zval retval;
	zval params[1];

	ZVAL_STRING(&funname, "json_encode");

	IF_SESSION_VARS() {

		params[0] = *Z_REFVAL(PS(http_session_vars));

		call_user_function_ex(EG(function_table), NULL, &funname, &retval, 1, params, 0, NULL);
	}

	smart_str_sets(&buf, Z_STRVAL_P(&retval));
	smart_str_0(&buf);

	//ZSTR_VAL(buf.s);

	return buf.s;

}






PS_SERIALIZER_DECODE_FUNC(json){

	// char *endptr = val + vallen;

	zend_string *key_str;
	zval *value;

	zval funname;
	zval retval;
	zval params[2];
	zval strjson;
	zval assoc;

	ZVAL_STRING(&funname, "json_decode");
	ZVAL_STRING(&strjson, val);
	ZVAL_BOOL(&assoc, TRUE);

	params[0] = strjson;
	params[1] = assoc;

	if(strlen(val) > 0){

		call_user_function_ex(EG(function_table), NULL, &funname, &retval, 2, params, 0, NULL);

		//php_var_dump(&retval, 1);

		if (Z_TYPE_P(&retval) == IS_ARRAY) {

			ZEND_HASH_FOREACH_STR_KEY_VAL(HASH_OF(&retval), key_str, value) {
					if (key_str) {
						php_set_session_var(key_str, value, NULL);
						php_add_session_var(key_str);
						ZVAL_UNDEF(value);
					}
				} ZEND_HASH_FOREACH_END();
				zval_ptr_dtor(&retval);
		}

	}

	return SUCCESS;
}













/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(session_json_handler)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/

	php_session_register_serializer("json", PS_SERIALIZER_ENCODE_NAME(json), PS_SERIALIZER_DECODE_NAME(json));


	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(session_json_handler)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(session_json_handler)
{
#if defined(COMPILE_DL_SESSION_JSON_HANDLER) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(session_json_handler)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(session_json_handler)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "session_json_handler support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ session_json_handler_functions[]
 *
 * Every user visible function must have an entry in session_json_handler_functions[].
 */
const zend_function_entry session_json_handler_functions[] = {
	PHP_FE(confirm_session_json_handler_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in session_json_handler_functions[] */
};
/* }}} */

/* {{{ session_json_handler_module_entry
 */
zend_module_entry session_json_handler_module_entry = {
	STANDARD_MODULE_HEADER,
	"session_json_handler",
	session_json_handler_functions,
	PHP_MINIT(session_json_handler),
	PHP_MSHUTDOWN(session_json_handler),
	PHP_RINIT(session_json_handler),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(session_json_handler),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(session_json_handler),
	PHP_SESSION_JSON_HANDLER_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SESSION_JSON_HANDLER
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(session_json_handler)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
