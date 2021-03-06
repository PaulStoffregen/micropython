// This file contains default configuration settings for MicroPython.
// You can override any of these options using mpconfigport.h file located
// in a directory of your port.

#include <mpconfigport.h>

// Any options not explicitly set in mpconfigport.h will get default
// values below.

/*****************************************************************************/
/* Micro Python emitters                                                     */

// Whether to emit CPython byte codes (for debugging/testing)
// Enabling this overrides all other emitters
#ifndef MICROPY_EMIT_CPYTHON
#define MICROPY_EMIT_CPYTHON (0)
#endif

// Whether to emit x64 native code
#ifndef MICROPY_EMIT_X64
#define MICROPY_EMIT_X64 (0)
#endif

// Whether to emit thumb native code
#ifndef MICROPY_EMIT_THUMB
#define MICROPY_EMIT_THUMB (0)
#endif

// Whether to enable the thumb inline assembler
#ifndef MICROPY_EMIT_INLINE_THUMB
#define MICROPY_EMIT_INLINE_THUMB (0)
#endif

/*****************************************************************************/
/* Internal debugging stuff                                                  */

// Whether to collect memory allocation stats
#ifndef MICROPY_MEM_STATS
#define MICROPY_MEM_STATS (0)
#endif

// Whether to build code to show byte code
#ifndef MICROPY_SHOW_BC
#define MICROPY_SHOW_BC (0)
#endif

/*****************************************************************************/
/* Fine control over Python features                                         */

// Whether to include the garbage collector
#ifndef MICROPY_ENABLE_GC
#define MICROPY_ENABLE_GC (0)
#endif

// Whether to include REPL helper function
#ifndef MICROPY_ENABLE_REPL_HELPERS
#define MICROPY_ENABLE_REPL_HELPERS (0)
#endif

// Whether to include lexer helper function for unix
#ifndef MICROPY_ENABLE_LEXER_UNIX
#define MICROPY_ENABLE_LEXER_UNIX (0)
#endif

// Whether to support float and complex types
#ifndef MICROPY_ENABLE_FLOAT
#define MICROPY_ENABLE_FLOAT (0)
#endif

// Whether to support slice object and correspondingly
// slice subscript operators
#ifndef MICROPY_ENABLE_SLICE
#define MICROPY_ENABLE_SLICE (1)
#endif

/*****************************************************************************/
/* Miscellaneous settings                                                    */

// printf format spec to use for machine_int_t and friends
#ifndef INT_FMT
#ifdef __LP64__
// Archs where machine_int_t == long, long != int
#define UINT_FMT "%lu"
#define INT_FMT "%ld"
#else
// Archs where machine_int_t == int
#define UINT_FMT "%u"
#define INT_FMT "%d"
#endif
#endif //INT_FMT
