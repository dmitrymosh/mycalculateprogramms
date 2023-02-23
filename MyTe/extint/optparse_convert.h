/*
 * Copyright(C) 2000-2001 ������ ���
 *
 * ���� ���� �������� ����������, ����������
 * �������������� �������� ������ ���.
 *
 * ����� ����� ����� ����� �� ����� ���� �����������,
 * ����������, ���������� �� ������ �����,
 * ������������ ��� �������������� ����� ��������,
 * ���������������, �������� �� ���� � ��� ��
 * ����� ������������ ������� ��� ����������������
 * ���������� ���������� � ��������� ������ ���.
 *
 * ����������� ���, ������������ � ���� �����, ������������
 * ������������� ��� ����� �������� � �� ����� ���� �����������
 * ��� ������ ����������.
 *
 * �������� ������-��� �� ����� �������
 * ��������������� �� ���������������� ����� ����.
 */

#ifndef OPTPARSE_CONVERT_H
#  define OPTPARSE_CONVERT_H

#ifndef OPTPARSE_CONVERT_API
#  define OPTPARSE_CONVERT_API
#endif

#undef optarg
#define optarg options.option_arg
#undef optopt
#define optopt options.option_opt
#undef optind
#define optind options.option_ind

#define construct_optparse_table(getopt_long_options, optparse_long_options, cbOptions) { \
    \
    dwFnErr = convert_options_for_optparse(long_options, NULL, &cbOptions); \
    if (dwFnErr) { \
	dwErr = DebugErrorFL("Error during convert_options_for_optparse"); \
	goto err; \
    } \
    \
    optparse_long_options = (struct optparse_long *)malloc(cbOptions); \
    if (!optparse_long_options) { \
	dwErr = DebugCodeFL(ERROR_NOT_ENOUGH_MEMORY, "Out of memory"); \
	goto err; \
    } \
    \
    dwFnErr = convert_options_for_optparse(long_options, optparse_long_options, &cbOptions); \
    if (dwFnErr) { \
	dwErr = DebugErrorFL("Error during convert_options_for_optparse"); \
	goto err; \
    } \
    \
}

#define ARGS_DECLARE() \
    struct optparse_long *optparse_long_options = NULL; \
    struct optparse options; \
    DWORD  cbOptions = 0

#define ARGS_CONVERT() \
    UNUSED(argc); \
    construct_optparse_table(long_options, optparse_long_options, cbOptions); \
    optparse_init(&options, argv)

#define ARGS_DONE() \
    dwErr = NO_ERROR; \
err: \
    free(optparse_long_options)

#define ARGS_GET_OPTION() optparse_long_only(&options, optparse_long_options, (int *)0)

 /*! \internal
 *	\ingroup functions
 *	\brief ������������ ������� �����
 *	������������ ������� ������� ����� ��� getopt_long() � ������� ��� optparse_long().
 *	���� dst == NULL, ������ ����� ���������� ���� � cbOptions.
 *	��������! ����� ������� ��������� �� ���������� ������!
 *	\param src [in] getopt-������� ������� �����
 *	\param dst [out] optparse-������� ������� �����.
 *	\param cbOptions [out] ������ optparse-������� � ������.
 *	\return � ������ ��������� ���������� 0, ����� ��� ������
 */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_CONVERT_API
        DWORD convert_options_for_optparse(
            const struct option* src,
            struct optparse_long* dst,
            DWORD* cbOptions
        );
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* OPTPARSE_CONVERT_H */

#ifdef OPTPARSE_CONVERT_IMPLEMENTATION
/* Implementation */

/*! \internal
*	\ingroup functions
*	\brief ������� ����� ������� �����
*	\param src [in] getopt-������� �����.
*	���������� ����� ������� getopt-������� ����� {0, 0, 0, 0}
*	\return �������� �� ����� ������ �������
*/
static BOOL
is_getopt_options_end(const struct option* src) {
    return (!src->name && !src->has_arg && !src->flag && !src->val);
}

/*! \internal
*	\ingroup functions
*	\brief ������������ ���� �����
*	������������ getopt-������� ����� � optparse-������� �����.
*	����� ������������ ��������� ����� {0, 0, 0, 0} (����� �������).
*	��������! ����� ����� ��������� �� ���������� ������!
*	\param src [in] ��������� �� getopt-������� �����.
*	\param dst [out] ��������� �� optparse-������� �����.
*	\return � ������ ��������� ���������� 0, ����� ��� ������
*/
static DWORD
convert_one_option(const struct option* src, struct optparse_long* dst) {
    DWORD   dwErr = CSPTEST_COMMON;

    // src.flag is always NULL so getopt_long() returned src.val
    dst->longname = src->name;
    dst->shortname = src->val;
    switch (src->has_arg) {
    case no_argument:
        dst->argtype = OPTPARSE_NONE;
        break;
    case required_argument:
        dst->argtype = OPTPARSE_REQUIRED;
        break;
    case optional_argument:
        dst->argtype = OPTPARSE_OPTIONAL;
        break;
    default:
        goto err;
    }

    dwErr = NO_ERROR;
err:
    return dwErr;
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_CONVERT_API
        DWORD
        convert_options_for_optparse(const struct option* src, struct optparse_long* dst, DWORD* cbOptions) {
        DWORD   dwErr = CSPTEST_COMMON;
        DWORD   cwOptions = 0;
        DWORD   i;

        // Check for empty src!!!
        while (!is_getopt_options_end(&src[cwOptions++])) {
        }
        *cbOptions = cwOptions * sizeof(struct optparse_long);

        if (!dst) {
            dwErr = NO_ERROR;
            goto err;
        }

        for (i = 0; i < cwOptions; ++i) {
            convert_one_option(&src[i], &dst[i]);
        }
        dwErr = NO_ERROR;
    err:
        return dwErr;
    }
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* OPTPARSE_CONVERT_IMPLEMENTATION */
