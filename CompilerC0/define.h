//kind
#define _CONST 200
#define VAR 201
#define TEMP 202
#define PROC 203
#define FUNC 204
#define CONSTTEMP 205
#define INVAR 206
#define REG 207
//#define RET
//#define LAB

//type
#define _VOID 101
#define ERR 102
#define _INT 103
#define _CHAR 104
#define STRING 105

//operator
#define ADD 0 //src1, src2, obj(t)
#define SUB 1 //src1, src2, obj(t)
#define MUL 2 //src1, src2, obj(t)
#define DIV 3 //src1, src2, obj(t)
#define BT 4 //src1, src2, obj(t)
#define LT 5 //src1, src2, obj(t)
#define EQ 6 //src1, src2, obj(t)
#define BET 7 //src1, src2, obj(t)
#define LET 8 //src1, src2, obj(t)
#define NEQ 9 //src1, src2, obj(t)
#define ASN 10 //src1, NULL, obj
#define EGTO 11 //src1, NULL, obj
#define RD 12 //src1, NULL, NULL
#define WR 13 //src1, NULL, NULL(obj)
#define LAB 14 //src1, NULL, NULL
#define GTO 15 //src1, NULL, NULL
#define BEGIN 16 //src1, NULL, NULL
#define ARG 17 //src1, NULL, NULL
#define CALL 18 //src1, NULL, NULL
#define RET 19 //NULL, NULL, NULL
#define NRET 20 //src1, NULL, NULL
#define END 21 //NULL, NULL, NULL
#define DINT 22 //src1, NULL, NULL
#define DCHR 23 //src1, NULL, NULL
#define REV 24 //src1, NULL, obj
#define _IN 25 //src1, NULL, NULL
#define ECALL 26 //src1, NULL, obj
#define CST 27 //src1, NULL, obj
#define IDX 28 //src1, src2, obj(t) index,array,temp
#define IDXASN 29 //src1, src2, obj(a) value,index,array