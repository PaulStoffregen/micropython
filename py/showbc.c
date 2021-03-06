#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "misc.h"
#include "mpconfig.h"
#include "bc0.h"

#if MICROPY_SHOW_BC

#define DECODE_UINT do { unum = *ip++; if (unum > 127) { unum = ((unum & 0x3f) << 8) | (*ip++); } } while (0)
#define DECODE_ULABEL do { unum = (ip[0] | (ip[1] << 8)); ip += 2; } while (0)
#define DECODE_SLABEL do { unum = (ip[0] | (ip[1] << 8)) - 0x8000; ip += 2; } while (0)
#define DECODE_QSTR do { qstr = *ip++; if (qstr > 127) { qstr = ((qstr & 0x3f) << 8) | (*ip++); } } while (0)

void mp_show_byte_code(const byte *ip, int len) {
    const byte *ip_start = ip;

    // decode prelude
    {
        uint n_local = *ip++;
        printf("(NUM_LOCAL %u)\n", n_local);
        for (; n_local > 0; n_local--) {
            uint local_num = *ip++;
            printf("(INIT_CELL %u)\n", local_num);
        }
        len -= ip - ip_start;
        ip_start = ip;
    }

    machine_uint_t unum;
    qstr qstr;
    while (ip - ip_start < len) {
        printf("%02u ", (uint)(ip - ip_start));
        int op = *ip++;
        switch (op) {
            case MP_BC_LOAD_CONST_FALSE:
                printf("LOAD_CONST_FALSE");
                break;

            case MP_BC_LOAD_CONST_NONE:
                printf("LOAD_CONST_NONE");
                break;

            case MP_BC_LOAD_CONST_TRUE:
                printf("LOAD_CONST_TRUE");
                break;

            case MP_BC_LOAD_CONST_ELLIPSIS:
                printf("LOAD_CONST_ELLIPSIS");
                break;

            case MP_BC_LOAD_CONST_SMALL_INT:
                unum = (ip[0] | (ip[1] << 8) | (ip[2] << 16)) - 0x800000;
                ip += 3;
                printf("LOAD_CONST_SMALL_INT %d", (int)unum);
                break;

                /*
            case MP_BC_LOAD_CONST_DEC:
                DECODE_QSTR;
                PUSH(rt_load_const_dec(qstr));
                break;
                */

            case MP_BC_LOAD_CONST_ID:
                DECODE_QSTR;
                printf("LOAD_CONST_ID %s", qstr_str(qstr));
                break;

            case MP_BC_LOAD_CONST_STRING:
                DECODE_QSTR;
                printf("LOAD_CONST_STRING %s", qstr_str(qstr));
                break;

            case MP_BC_LOAD_FAST_0:
                printf("LOAD_FAST_0");
                break;

            case MP_BC_LOAD_FAST_1:
                printf("LOAD_FAST_1");
                break;

            case MP_BC_LOAD_FAST_2:
                printf("LOAD_FAST_2");
                break;

            case MP_BC_LOAD_FAST_N:
                DECODE_UINT;
                printf("LOAD_FAST_N " UINT_FMT, unum);
                break;

            case MP_BC_LOAD_DEREF:
                DECODE_UINT;
                printf("LOAD_DEREF " UINT_FMT, unum);
                break;

            case MP_BC_LOAD_NAME:
                DECODE_QSTR;
                printf("LOAD_NAME %s", qstr_str(qstr));
                break;

            case MP_BC_LOAD_GLOBAL:
                DECODE_QSTR;
                printf("LOAD_GLOBAL %s", qstr_str(qstr));
                break;

            case MP_BC_LOAD_ATTR:
                DECODE_QSTR;
                printf("LOAD_ATTR %s", qstr_str(qstr));
                break;

            case MP_BC_LOAD_METHOD:
                DECODE_QSTR;
                printf("LOAD_METHOD %s", qstr_str(qstr));
                break;

            case MP_BC_LOAD_BUILD_CLASS:
                printf("LOAD_BUILD_CLASS");
                break;

            case MP_BC_STORE_FAST_0:
                printf("STORE_FAST_0");
                break;

            case MP_BC_STORE_FAST_1:
                printf("STORE_FAST_1");
                break;

            case MP_BC_STORE_FAST_2:
                printf("STORE_FAST_2");
                break;

            case MP_BC_STORE_FAST_N:
                DECODE_UINT;
                printf("STORE_FAST_N " UINT_FMT, unum);
                break;

            case MP_BC_STORE_DEREF:
                DECODE_UINT;
                printf("STORE_DEREF " UINT_FMT, unum);
                break;

            case MP_BC_STORE_NAME:
                DECODE_QSTR;
                printf("STORE_NAME %s", qstr_str(qstr));
                break;

            case MP_BC_STORE_GLOBAL:
                DECODE_QSTR;
                printf("STORE_GLOBAL %s", qstr_str(qstr));
                break;

            case MP_BC_STORE_ATTR:
                DECODE_QSTR;
                printf("STORE_ATTR %s", qstr_str(qstr));
                break;

            case MP_BC_STORE_SUBSCR:
                printf("STORE_SUBSCR");
                break;

            case MP_BC_DUP_TOP:
                printf("DUP_TOP");
                break;

            case MP_BC_DUP_TOP_TWO:
                printf("DUP_TOP_TWO");
                break;

            case MP_BC_POP_TOP:
                printf("POP_TOP");
                break;

                /*
            case MP_BC_ROT_TWO:
                obj1 = sp[0];
                sp[0] = sp[1];
                sp[1] = obj1;
                break;
                */

            case MP_BC_ROT_THREE:
                printf("ROT_THREE");
                break;

            case MP_BC_JUMP:
                DECODE_SLABEL;
                printf("JUMP " UINT_FMT, ip + unum - ip_start);
                break;

            case MP_BC_POP_JUMP_IF_TRUE:
                DECODE_SLABEL;
                printf("POP_JUMP_IF_TRUE " UINT_FMT, ip + unum - ip_start);
                break;

            case MP_BC_POP_JUMP_IF_FALSE:
                DECODE_SLABEL;
                printf("POP_JUMP_IF_FALSE " UINT_FMT, ip + unum - ip_start);
                break;

                /*
            case MP_BC_JUMP_IF_TRUE_OR_POP:
                DECODE_SLABEL;
                if (rt_is_true(*sp)) {
                    ip += unum;
                } else {
                    sp++;
                }
                break;

            case MP_BC_JUMP_IF_FALSE_OR_POP:
                DECODE_SLABEL;
                if (rt_is_true(*sp)) {
                    sp++;
                } else {
                    ip += unum;
                }
                break;
                */

            case MP_BC_SETUP_EXCEPT:
                DECODE_ULABEL; // except labels are always forward
                printf("SETUP_EXCEPT " UINT_FMT, ip + unum - ip_start);
                break;

            case MP_BC_END_FINALLY:
                // if TOS is an exception, reraises the exception (3 values on TOS)
                // if TOS is an integer, does something else
                // if TOS is None, just pops it and continues
                // else error
                printf("END_FINALLY");
                break;

            case MP_BC_GET_ITER:
                printf("GET_ITER");
                break;

            case MP_BC_FOR_ITER:
                DECODE_ULABEL; // the jump offset if iteration finishes; for labels are always forward
                printf("FOR_ITER " UINT_FMT, ip + unum - ip_start);
                break;

            case MP_BC_POP_BLOCK:
                // pops block and restores the stack
                printf("POP_BLOCK");
                break;

            case MP_BC_POP_EXCEPT:
                // pops block, checks it's an exception block, and restores the stack, saving the 3 exception values to local threadstate
                printf("POP_EXCEPT");
                break;

                /*
            case MP_BC_UNARY_OP:
                unum = *ip++;
                *sp = rt_unary_op(unum, *sp);
                break;
                */

            case MP_BC_BINARY_OP:
                unum = *ip++;
                printf("BINARY_OP " UINT_FMT, unum);
                break;

            case MP_BC_COMPARE_OP:
                unum = *ip++;
                printf("COMPARE_OP " UINT_FMT, unum);
                break;

            case MP_BC_BUILD_TUPLE:
                DECODE_UINT;
                printf("BUILD_TUPLE " UINT_FMT, unum);
                break;

            case MP_BC_BUILD_LIST:
                DECODE_UINT;
                printf("BUILD_LIST " UINT_FMT, unum);
                break;

            case MP_BC_LIST_APPEND:
                DECODE_UINT;
                printf("LIST_APPEND " UINT_FMT, unum);
                break;

            case MP_BC_BUILD_MAP:
                DECODE_UINT;
                printf("BUILD_MAP " UINT_FMT, unum);
                break;

                /*
            case MP_BC_STORE_MAP:
                sp += 2;
                rt_store_map(sp[0], sp[-2], sp[-1]);
                break;

            case MP_BC_MAP_ADD:
                DECODE_UINT;
                // I think it's guaranteed by the compiler that sp[unum + 1] is a map
                rt_store_map(sp[unum + 1], sp[0], sp[1]);
                sp += 2;
                break;
                */

            case MP_BC_BUILD_SET:
                DECODE_UINT;
                printf("BUILD_SET " UINT_FMT, unum);
                break;

            case MP_BC_SET_ADD:
                DECODE_UINT;
                printf("SET_ADD " UINT_FMT, unum);
                break;

            case MP_BC_UNPACK_SEQUENCE:
                DECODE_UINT;
                printf("UNPACK_SEQUENCE " UINT_FMT, unum);
                break;

            case MP_BC_MAKE_FUNCTION:
                DECODE_UINT;
                printf("MAKE_FUNCTION " UINT_FMT, unum);
                break;

            case MP_BC_MAKE_CLOSURE:
                DECODE_UINT;
                printf("MAKE_CLOSURE " UINT_FMT, unum);
                break;

            case MP_BC_CALL_FUNCTION:
                DECODE_UINT;
                printf("CALL_FUNCTION n=" UINT_FMT " nkw=" UINT_FMT, unum & 0xff, (unum >> 8) & 0xff);
                break;

            case MP_BC_CALL_METHOD:
                DECODE_UINT;
                printf("CALL_METHOD n=" UINT_FMT " nkw=" UINT_FMT, unum & 0xff, (unum >> 8) & 0xff);
                break;

            case MP_BC_RETURN_VALUE:
                printf("RETURN_VALUE");
                break;

            case MP_BC_YIELD_VALUE:
                printf("YIELD_VALUE");
                break;

            case MP_BC_IMPORT_NAME:
                DECODE_QSTR;
                printf("IMPORT NAME %s", qstr_str(qstr));
                break;

            case MP_BC_IMPORT_FROM:
                DECODE_QSTR;
                printf("IMPORT NAME %s", qstr_str(qstr));
                break;

            default:
                printf("code %p, byte code 0x%02x not implemented\n", ip, op);
                assert(0);
                return;
        }
        printf("\n");
    }
}

#endif // MICROPY_SHOW_BC
