/* Optparse --- portable, reentrant, embeddable, getopt-like option parser
 *
 * This is free and unencumbered software released into the public domain.
 *
 * To get the implementation, define OPTPARSE_IMPLEMENTATION.
 * Optionally define OPTPARSE_API to control the API's visibility
 * and/or linkage (static, __attribute__, __declspec).
 *
 * The POSIX getopt() option parser has three fatal flaws. These flaws
 * are solved by Optparse.
 *
 * 1) Parser state is stored entirely in global variables, some of
 * which are static and inaccessible. This means only one thread can
 * use getopt(). It also means it's not possible to recursively parse
 * nested sub-arguments while in the middle of argument parsing.
 * Optparse fixes this by storing all state on a local struct.
 *
 * 2) The POSIX standard provides no way to properly reset the parser.
 * This means for portable code that getopt() is only good for one
 * run, over one argv with one option string. It also means subcommand
 * options cannot be processed with getopt(). Most implementations
 * provide a method to reset the parser, but it's not portable.
 * Optparse provides an optparse_arg() function for stepping over
 * subcommands and continuing parsing of options with another option
 * string. The Optparse struct itself can be passed around to
 * subcommand handlers for additional subcommand option parsing. A
 * full reset can be achieved by with an additional optparse_init().
 *
 * 3) Error messages are printed to stderr. This can be disabled with
 * opterr, but the messages themselves are still inaccessible.
 * Optparse solves this by writing an error message in its errmsg
 * field. The downside to Optparse is that this error message will
 * always be in English rather than the current locale.
 *
 * Optparse should be familiar with anyone accustomed to getopt(), and
 * it could be a nearly drop-in replacement. The option string is the
 * same and the fields have almost the same names as the getopt() global
 * variables (option_arg, option_ind, option_opt).
 *
 * Optparse also supports GNU-style long options with optparse_long().
 * The interface is slightly different and simpler than getopt_long().
 *
 * By default, argv is permuted as it is parsed, moving non-option
 * arguments to the end. This can be disabled by setting the `permute`
 * field to 0 after initialization.
 */

#ifndef OPTPARSE_H
#define OPTPARSE_H

//#include "reader/support.h"
#include <tchar.h>

#ifndef OPTPARSE_API
#  define OPTPARSE_API
#endif

 // Do not decrease this value.
#define MAX_ERRMSG_LENGTH 64

struct optparse {
    TCHAR** argv;
    int permute;
    int option_ind;
    int option_opt;
    TCHAR* option_arg;
    TCHAR errmsg[MAX_ERRMSG_LENGTH];
    int subopt;
};

enum optparse_argtype {
    OPTPARSE_NONE,
    OPTPARSE_REQUIRED,
    OPTPARSE_OPTIONAL
};

struct optparse_long {
    const TCHAR* longname;
    int shortname;
    enum optparse_argtype argtype;
};

/**
 * Initializes the parser state.
 */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        void optparse_init(struct optparse* options, TCHAR** argv);
#ifdef __cplusplus
}
#endif //__cplusplus

/**
 * Read the next option in the argv array.
 * @param optstring a getopt()-formatted option string.
 * @return the next option character, -1 for done, or '?' for error
 *
 * Just like getopt(), a character followed by no colons means no
 * argument. One colon means the option has a required argument. Two
 * colons means the option takes an optional argument.
 */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        int optparse(struct optparse* options, const TCHAR* optstring);
#ifdef __cplusplus
}
#endif //__cplusplus

/**
 * Handles GNU-style long options in addition to getopt() options.
 * This works a lot like GNU's getopt_long(). The last option in
 * longopts must be all zeros, marking the end of the array. The
 * longindex argument may be NULL.
 */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        int optparse_long(struct optparse* options,
            const struct optparse_long* longopts,
            int* longindex);
#ifdef __cplusplus
}
#endif //__cplusplus

/**
 * Works a lot like GNU's getopt_long_only().
*/
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        int optparse_long_only(struct optparse* options,
            const struct optparse_long* longopts,
            int* longindex);
#ifdef __cplusplus
}
#endif //__cplusplus

/**
 * Used for stepping over non-option arguments.
 * @return the next non-option argument, or NULL for no more arguments
 *
 * Argument parsing can continue with optparse() after using this
 * function. That would be used to parse the options for the
 * subcommand returned by optparse_arg(). This function allows you to
 * ignore the value of option_ind.
 */
#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        TCHAR* optparse_arg(struct optparse* options);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* OPTPARSE_H */

#ifdef OPTPARSE_IMPLEMENTATION
/* Implementation */

// Must be a way shorter than MAX_ERRMSG_LENGTH (CPCSP-10635)
#define OPTPARSE_MSG_INVALID _TEXT("invalid option")
#define OPTPARSE_MSG_MISSING _TEXT("option requires an argument")
#define OPTPARSE_MSG_TOOMANY _TEXT("option takes no arguments")
#define OPTPARSE_MSG_AMBIGUITY _TEXT("option is ambiguous")

#define DASH_SEPARATOR _TEXT(" -- ")

#define THREE_DOTS_LENGTH 3

static int
optparse_error(struct optparse* options, const TCHAR* msg, const TCHAR* data)
{
    unsigned p = 0;
    const TCHAR* sep = DASH_SEPARATOR;
    const unsigned errmsg_len = (unsigned)_countof(options->errmsg);
    while (*msg) {
        options->errmsg[p++] = *msg++;
    }
    while (*sep) {
        options->errmsg[p++] = *sep++;
    }

    // Add opening apostrophe.
    options->errmsg[p++] = _TEXT('\'');

    // Add option name. 2 is for \'\0 in the end of errmsg.
    while (p + 2 < errmsg_len && *data) {
        options->errmsg[p++] = *data++;
    }

    // Add three dots if option name is too long.
    if (*data) {
        int i = 0;
        p -= THREE_DOTS_LENGTH;
        for (i = 0; i < THREE_DOTS_LENGTH; ++i) {
            options->errmsg[p++] = _TEXT('.');
        }
    }

    // Add closing apostrophe and null-terminate errmsg.
    options->errmsg[p++] = _TEXT('\'');
    options->errmsg[p++] = _TEXT('\0');
    return '?';
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        void
        optparse_init(struct optparse* options, TCHAR** argv)
    {
        options->argv = argv;
        options->permute = 1;
        options->option_ind = 1;
        options->subopt = 0;
        options->option_arg = 0;
        options->errmsg[0] = _TEXT('\0');
    }
#ifdef __cplusplus
}
#endif //__cplusplus

static int
optparse_is_dashdash(const TCHAR* arg)
{
    return arg != 0 && (arg[0] == _TEXT('-') || arg[0] == _TEXT('–')) && (arg[1] == _TEXT('-') || arg[1] == _TEXT('–')) && arg[2] == _TEXT('\0');
}

static int
optparse_is_shortopt(const TCHAR* arg)
{
    return arg != 0 && (arg[0] == _TEXT('-') || arg[0] == _TEXT('–')) && !(arg[1] == _TEXT('-') || arg[1] == _TEXT('–')) && arg[1] != _TEXT('\0');
}

static int
optparse_is_longopt(const TCHAR* arg)
{
    return arg != 0 && (arg[0] == _TEXT('-') || arg[0] == _TEXT('–')) && (arg[1] == _TEXT('-') || arg[1] == _TEXT('–')) && arg[2] != _TEXT('\0');
}

static void
optparse_permute(struct optparse* options, int index)
{
    TCHAR* nonoption = options->argv[index];
    int i;
    for (i = index; i < options->option_ind - 1; i++)
        options->argv[i] = options->argv[i + 1];
    options->argv[options->option_ind - 1] = nonoption;
}

static int
optparse_argtype(const TCHAR* optstring, TCHAR c)
{
    int count = OPTPARSE_NONE;
    if (c == ':')
        return -1;
    for (; *optstring && c != *optstring; optstring++);
    if (!*optstring)
        return -1;
    if (optstring[1] == ':')
        count += optstring[2] == ':' ? 2 : 1;
    return count;
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        int
        optparse(struct optparse* options, const TCHAR* optstring)
    {
        int type;
        TCHAR* next;
        TCHAR* option = options->argv[options->option_ind];
        options->errmsg[0] = _TEXT('\0');
        options->option_opt = 0;
        options->option_arg = 0;
        if (option == 0) {
            return -1;
        }
        else if (optparse_is_dashdash(option)) {
            options->option_ind++; /* consume "--" */
            return -1;
        }
        else if (!optparse_is_shortopt(option)) {
            if (options->permute) {
                int index = options->option_ind++;
                int r = optparse(options, optstring);
                optparse_permute(options, index);
                options->option_ind--;
                return r;
            }
            else {
                return -1;
            }
        }
        option += options->subopt + 1;
        options->option_opt = option[0];
        type = optparse_argtype(optstring, option[0]);
        next = options->argv[options->option_ind + 1];
        switch (type) {
        case -1: {
            TCHAR str[2] = { 0, 0 };
            str[0] = option[0];
            options->option_ind++;
            return optparse_error(options, OPTPARSE_MSG_INVALID, str);
        }
        case OPTPARSE_NONE:
            if (option[1]) {
                options->subopt++;
            }
            else {
                options->subopt = 0;
                options->option_ind++;
            }
            return option[0];
        case OPTPARSE_REQUIRED:
            options->subopt = 0;
            options->option_ind++;
            if (option[1]) {
                options->option_arg = option + 1;
            }
            else if (next != 0) {
                options->option_arg = next;
                options->option_ind++;
            }
            else {
                TCHAR str[2] = { 0, 0 };
                str[0] = option[0];
                options->option_arg = 0;
                return optparse_error(options, OPTPARSE_MSG_MISSING, str);
            }
            return option[0];
        case OPTPARSE_OPTIONAL:
            options->subopt = 0;
            options->option_ind++;
            if (option[1])
                options->option_arg = option + 1;
            else
                options->option_arg = 0;
            return option[0];
        }
        return 0;
    }
#ifdef __cplusplus
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        TCHAR*
        optparse_arg(struct optparse* options)
    {
        TCHAR* option = options->argv[options->option_ind];
        options->subopt = 0;
        if (option != 0)
            options->option_ind++;
        return option;
    }
#ifdef __cplusplus
}
#endif //__cplusplus

static int
optparse_longopts_end(const struct optparse_long* longopts, int i)
{
    return !longopts[i].longname && !longopts[i].shortname;
}

static void
optparse_from_long(const struct optparse_long* longopts, TCHAR* optstring)
{
    TCHAR* p = optstring;
    int i;
    for (i = 0; !optparse_longopts_end(longopts, i); i++) {
        if (longopts[i].shortname) {
            int a;
            *p++ = (TCHAR)longopts[i].shortname;
            for (a = 0; a < (int)longopts[i].argtype; a++)
                *p++ = _TEXT(':');
        }
    }
    *p = _TEXT('\0');
}

/* Unlike strcmp(), handles options containing "=". */
/* is_exact_match is true if LCP(longname, option) == option */
static int
optparse_longopts_match(const TCHAR* longname, const TCHAR* option, BOOL* is_exact_match)
{
    const TCHAR* a = option, * n = longname;
    *is_exact_match = FALSE;
    if (longname == 0)
        return 0;
    for (; *a && *n && *a != '='; a++, n++)
        if (*a != *n)
            return 0;
    if (*n == '\0' && (*a == '\0' || *a == '=')) {
        *is_exact_match = TRUE;
        return TRUE;
    } if (*a == '\0' || *a == '=') {
        // longname can be longer.
        return TRUE;
    }
    return FALSE;
}

/* Return the part after "=", or NULL. */
static TCHAR*
optparse_longopts_arg(TCHAR* option)
{
    for (; *option && *option != '='; option++);
    if (*option == '=')
        return option + 1;
    else
        return 0;
}

static int
optparse_long_fallback(struct optparse* options,
    const struct optparse_long* longopts,
    int* longindex)
{
    int result;
    TCHAR optstring[96 * 3 + 1]; /* 96 ASCII printable characters */
    optparse_from_long(longopts, optstring);
    result = optparse(options, optstring);
    if (longindex != 0) {
        *longindex = -1;
        if (result != -1) {
            int i;
            for (i = 0; !optparse_longopts_end(longopts, i); i++)
                if (longopts[i].shortname == options->option_opt)
                    *longindex = i;
        }
    }
    return result;
}

static int
parse_long_option(TCHAR* option,
    struct optparse* options,
    const struct optparse_long* longopts,
    int* longindex,
    BOOL is_long_only)
{
    int i;
    int matched_options_count = 0;
    int last_matched_longopt_index = -1;
    const TCHAR* name = NULL;
    const TCHAR* last_matched_name = NULL;
    TCHAR* arg;

    options->errmsg[0] = _TEXT('\0');
    options->option_opt = 0;
    options->option_arg = 0;
    if (is_long_only && !optparse_is_longopt(option)) {
        option += 1; /* skip "-" */
    }
    else {
        option += 2; /* skip "--" */
    }
    options->option_ind++;

    for (i = 0; !optparse_longopts_end(longopts, i); i++) {
        BOOL is_exact_match = FALSE;
        name = longopts[i].longname;
        if (!optparse_longopts_match(name, option, &is_exact_match)) {
            continue;
        }
        if (is_exact_match) {
            // exact match
            last_matched_name = name;
            last_matched_longopt_index = i;
            matched_options_count = 1;
            break;
        }
        else {
            // nonexact match
            last_matched_name = name;
            last_matched_longopt_index = i;
            ++matched_options_count;
        }
    }

    if (matched_options_count > 1) {
        return optparse_error(options, OPTPARSE_MSG_AMBIGUITY, option);
    }
    if (matched_options_count == 0) {
        return optparse_error(options, OPTPARSE_MSG_INVALID, option);
    }

    // matched_options_count == 1
    i = last_matched_longopt_index;
    name = last_matched_name;
    if (longindex)
        *longindex = i;
    options->option_opt = longopts[i].shortname;
    arg = optparse_longopts_arg(option);
    if (longopts[i].argtype == OPTPARSE_NONE && arg != 0) {
        return optparse_error(options, OPTPARSE_MSG_TOOMANY, name);
    } if (arg != 0) {
        options->option_arg = arg;
    }
    else if (longopts[i].argtype == OPTPARSE_REQUIRED) {
        options->option_arg = options->argv[options->option_ind];
        if (options->option_arg == 0)
            return optparse_error(options, OPTPARSE_MSG_MISSING, name);
        else
            options->option_ind++;
    }
    return options->option_opt;
}

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        int
        optparse_long(struct optparse* options,
            const struct optparse_long* longopts,
            int* longindex)
    {
        TCHAR* option = options->argv[options->option_ind];
        if (option == 0) {
            return -1;
        }
        else if (optparse_is_dashdash(option)) {
            options->option_ind++; /* consume "--" */
            return -1;
        }
        else if (optparse_is_shortopt(option)) {
            return optparse_long_fallback(options, longopts, longindex);
        }
        else if (!optparse_is_longopt(option)) {
            if (options->permute) {
                int index = options->option_ind++;
                int r = optparse_long(options, longopts, longindex);
                optparse_permute(options, index);
                options->option_ind--;
                return r;
            }
            else {
                return -1;
            }
        }
        return parse_long_option(option, options, longopts, longindex, FALSE);
    }
#ifdef __cplusplus
}
#endif //__cplusplus

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus
    OPTPARSE_API
        int
        optparse_long_only(struct optparse* options,
            const struct optparse_long* longopts,
            int* longindex)
    {
        TCHAR* option = options->argv[options->option_ind];
        if (option == 0) {
            return -1;
        }
        else if (optparse_is_dashdash(option)) {
            options->option_ind++; /* consume "--" */
            return -1;
        }
        else if (!optparse_is_shortopt(option) && !optparse_is_longopt(option)) {
            if (options->permute) {
                int index = options->option_ind++;
                int r = optparse_long_only(options, longopts, longindex);
                optparse_permute(options, index);
                options->option_ind--;
                return r;
            }
            else {
                return -1;
            }
        }
        return parse_long_option(option, options, longopts, longindex, TRUE);
    }
#ifdef __cplusplus
}
#endif //__cplusplus

#endif /* OPTPARSE_IMPLEMENTATION */
