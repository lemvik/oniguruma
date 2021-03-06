#ifndef REGPARSE_H
#define REGPARSE_H
/**********************************************************************
  regparse.h -  Oniguruma (regular expression library)
**********************************************************************/
/*-
 * Copyright (c) 2002-2017  K.Kosako  <sndgk393 AT ybb DOT ne DOT jp>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "regint.h"

/* node type */
typedef enum {
  NODE_STRING    =  0,
  NODE_CCLASS    =  1,
  NODE_CTYPE     =  2,
  NODE_BACKREF   =  3,
  NODE_QUANT     =  4,
  NODE_ENCLOSURE =  5,
  NODE_ANCHOR    =  6,
  NODE_LIST      =  7,
  NODE_ALT       =  8,
  NODE_CALL      =  9,
  NODE_GIMMICK   = 10
} NodeType;

enum GimmickType {
  GIMMICK_FAIL = 0,
  GIMMICK_KEEP = 1,
  GIMMICK_SAVE = 2,
  GIMMICK_UPDATE_VAR = 3,
};

/* node type bit */
#define NODE_TYPE2BIT(type)      (1<<(type))

#define BIT_NODE_STRING     NODE_TYPE2BIT(NODE_STRING)
#define BIT_NODE_CCLASS     NODE_TYPE2BIT(NODE_CCLASS)
#define BIT_NODE_CTYPE      NODE_TYPE2BIT(NODE_CTYPE)
#define BIT_NODE_BACKREF    NODE_TYPE2BIT(NODE_BACKREF)
#define BIT_NODE_QUANT      NODE_TYPE2BIT(NODE_QUANT)
#define BIT_NODE_ENCLOSURE  NODE_TYPE2BIT(NODE_ENCLOSURE)
#define BIT_NODE_ANCHOR     NODE_TYPE2BIT(NODE_ANCHOR)
#define BIT_NODE_LIST       NODE_TYPE2BIT(NODE_LIST)
#define BIT_NODE_ALT        NODE_TYPE2BIT(NODE_ALT)
#define BIT_NODE_CALL       NODE_TYPE2BIT(NODE_CALL)
#define BIT_NODE_GIMMICK    NODE_TYPE2BIT(NODE_GIMMICK)

#define NODE_IS_SIMPLE_TYPE(node) \
  ((NODE_TYPE2BIT(NODE_TYPE(node)) & \
    (BIT_NODE_STRING | BIT_NODE_CCLASS | BIT_NODE_CTYPE | BIT_NODE_BACKREF)) != 0)

#define NODE_TYPE(node)             ((node)->u.base.node_type)
#define NODE_SET_TYPE(node, ntype)   (node)->u.base.node_type = (ntype)

#define STR_(node)         (&((node)->u.str))
#define CCLASS_(node)      (&((node)->u.cclass))
#define CTYPE_(node)       (&((node)->u.ctype))
#define BACKREF_(node)     (&((node)->u.backref))
#define QUANT_(node)       (&((node)->u.quant))
#define ENCLOSURE_(node)   (&((node)->u.enclosure))
#define ANCHOR_(node)      (&((node)->u.anchor))
#define CONS_(node)        (&((node)->u.cons))
#define CALL_(node)        (&((node)->u.call))
#define GIMMICK_(node)     (&((node)->u.gimmick))

#define NODE_CAR(node)         (CONS_(node)->car)
#define NODE_CDR(node)         (CONS_(node)->cdr)

#define CTYPE_ANYCHAR      -1
#define NODE_IS_ANYCHAR(node) \
  (NODE_TYPE(node) == NODE_CTYPE && CTYPE_(node)->ctype == CTYPE_ANYCHAR)

#define CTYPE_OPTION(node, reg) \
  (NODE_IS_FIXED_OPTION(node) ? CTYPE_(node)->options : reg->options)


#define ANCHOR_ANYCHAR_STAR_MASK (ANCHOR_ANYCHAR_STAR | ANCHOR_ANYCHAR_STAR_ML)
#define ANCHOR_END_BUF_MASK      (ANCHOR_END_BUF | ANCHOR_SEMI_END_BUF)

#define ENCLOSURE_MEMORY           (1<<0)
#define ENCLOSURE_OPTION           (1<<1)
#define ENCLOSURE_STOP_BACKTRACK   (1<<2)
#define ENCLOSURE_IF_ELSE          (1<<3)

#define NODE_STRING_MARGIN         16
#define NODE_STRING_BUF_SIZE       24  /* sizeof(CClassNode) - sizeof(int)*4 */
#define NODE_BACKREFS_SIZE       6

#define STRING_RAW                (1<<0) /* by backslashed number */
#define STRING_AMBIG              (1<<1)
#define STRING_DONT_GET_OPT_INFO  (1<<2)

#define NODE_STRING_LEN(node)             ((node)->u.str.end - (node)->u.str.s)
#define NODE_STRING_SET_RAW(node)          (node)->u.str.flag |= STRING_RAW
#define NODE_STRING_CLEAR_RAW(node)        (node)->u.str.flag &= ~STRING_RAW
#define NODE_STRING_SET_AMBIG(node)        (node)->u.str.flag |= STRING_AMBIG
#define NODE_STRING_SET_DONT_GET_OPT_INFO(node) \
  (node)->u.str.flag |= STRING_DONT_GET_OPT_INFO
#define NODE_STRING_IS_RAW(node)          (((node)->u.str.flag & STRING_RAW)   != 0)
#define NODE_STRING_IS_AMBIG(node)        (((node)->u.str.flag & STRING_AMBIG) != 0)
#define NODE_STRING_IS_DONT_GET_OPT_INFO(node) \
  (((node)->u.str.flag & STRING_DONT_GET_OPT_INFO) != 0)

#define BACKREFS_P(br) \
  (IS_NOT_NULL((br)->back_dynamic) ? (br)->back_dynamic : (br)->back_static);

#define QUANT_BODY_IS_NOT_EMPTY    0
#define QUANT_BODY_IS_EMPTY        1
#define QUANT_BODY_IS_EMPTY_MEM    2
#define QUANT_BODY_IS_EMPTY_REC    3

/* node status bits */
#define NST_MIN_FIXED             (1<<0)
#define NST_MAX_FIXED             (1<<1)
#define NST_CLEN_FIXED            (1<<2)
#define NST_MARK1                 (1<<3)
#define NST_MARK2                 (1<<4)
#define NST_STOP_BT_SIMPLE_REPEAT (1<<5)
#define NST_RECURSION             (1<<6)
#define NST_CALLED                (1<<7)
#define NST_ADDR_FIXED            (1<<8)
#define NST_NAMED_GROUP           (1<<9)
#define NST_IN_REAL_REPEAT        (1<<10) /* STK_REPEAT is nested in stack. */
#define NST_IN_ZERO_REPEAT        (1<<11) /* (....){0} */
#define NST_IN_MULTI_ENTRY        (1<<12)
#define NST_NEST_LEVEL            (1<<13)
#define NST_BY_NUMBER             (1<<14) /* {n,m} */
#define NST_BY_NAME               (1<<15) /* backref by name */
#define NST_BACKREF               (1<<16)
#define NST_CHECKER               (1<<17)
#define NST_FIXED_OPTION          (1<<18)
#define NST_PROHIBIT_RECURSION    (1<<19)
#define NST_SUPER                 (1<<20)


#define NODE_STATUS(node)           (((Node* )node)->u.base.status)
#define NODE_STATUS_ADD(node,f)     (NODE_STATUS(node) |= (f))
#define NODE_STATUS_REMOVE(node,f)  (NODE_STATUS(node) &= ~(f))

#define NODE_IS_BY_NUMBER(node)       ((NODE_STATUS(node) & NST_BY_NUMBER)      != 0)
#define NODE_IS_IN_REAL_REPEAT(node)  ((NODE_STATUS(node) & NST_IN_REAL_REPEAT) != 0)
#define NODE_IS_CALLED(node)          ((NODE_STATUS(node) & NST_CALLED)         != 0)
#define NODE_IS_IN_MULTI_ENTRY(node)  ((NODE_STATUS(node) & NST_IN_MULTI_ENTRY) != 0)
#define NODE_IS_RECURSION(node)       ((NODE_STATUS(node) & NST_RECURSION)      != 0)
#define NODE_IS_IN_ZERO_REPEAT(node)  ((NODE_STATUS(node) & NST_IN_ZERO_REPEAT) != 0)
#define NODE_IS_NAMED_GROUP(node)     ((NODE_STATUS(node) & NST_NAMED_GROUP)  != 0)
#define NODE_IS_ADDR_FIXED(node)      ((NODE_STATUS(node) & NST_ADDR_FIXED)   != 0)
#define NODE_IS_CLEN_FIXED(node)      ((NODE_STATUS(node) & NST_CLEN_FIXED)   != 0)
#define NODE_IS_MIN_FIXED(node)       ((NODE_STATUS(node) & NST_MIN_FIXED)    != 0)
#define NODE_IS_MAX_FIXED(node)       ((NODE_STATUS(node) & NST_MAX_FIXED)    != 0)
#define NODE_IS_MARK1(node)           ((NODE_STATUS(node) & NST_MARK1)        != 0)
#define NODE_IS_MARK2(node)           ((NODE_STATUS(node) & NST_MARK2)        != 0)
#define NODE_IS_NEST_LEVEL(node)      ((NODE_STATUS(node) & NST_NEST_LEVEL)   != 0)
#define NODE_IS_BY_NAME(node)         ((NODE_STATUS(node) & NST_BY_NAME)      != 0)
#define NODE_IS_BACKREF(node)         ((NODE_STATUS(node) & NST_BACKREF)      != 0)
#define NODE_IS_CHECKER(node)         ((NODE_STATUS(node) & NST_CHECKER)      != 0)
#define NODE_IS_FIXED_OPTION(node)    ((NODE_STATUS(node) & NST_FIXED_OPTION) != 0)
#define NODE_IS_SUPER(node)           ((NODE_STATUS(node) & NST_SUPER)        != 0)
#define NODE_IS_PROHIBIT_RECURSION(node) \
    ((NODE_STATUS(node) & NST_PROHIBIT_RECURSION) != 0)
#define NODE_IS_STOP_BT_SIMPLE_REPEAT(node) \
    ((NODE_STATUS(node) & NST_STOP_BT_SIMPLE_REPEAT) != 0)

#define NODE_BODY(node)           ((node)->u.base.body)
#define NODE_QUANT_BODY(node)      ((node)->body)
#define NODE_ENCLOSURE_BODY(node)   ((node)->body)
#define NODE_CALL_BODY(node)      ((node)->body)
#define NODE_ANCHOR_BODY(node)    ((node)->body)


typedef struct {
  NodeType node_type;
  int status;

  UChar* s;
  UChar* end;
  unsigned int flag;
  int    capa;    /* (allocated size - 1) or 0: use buf[] */
  UChar  buf[NODE_STRING_BUF_SIZE];
} StrNode;

typedef struct {
  NodeType node_type;
  int status;

  unsigned int flags;
  BitSet bs;
  BBuf*  mbuf;   /* multi-byte info or NULL */
} CClassNode;

typedef struct {
  NodeType node_type;
  int status;
  struct _Node* body;

  int lower;
  int upper;
  int greedy;
  int body_empty_info;
  struct _Node* head_exact;
  struct _Node* next_head_exact;
  int is_refered;     /* include called node. don't eliminate even if {0} */
#ifdef USE_COMBINATION_EXPLOSION_CHECK
  int comb_exp_check_num;  /* 1,2,3...: check,  0: no check  */
#endif
} QuantNode;

typedef struct {
  NodeType node_type;
  int status;
  struct _Node* body;

  int type;
  union {
    struct {
      int regnum;
      AbsAddrType called_addr;
      int entry_count;
      int called_state;
    } m;
    struct {
      OnigOptionType options;
    } o;
    struct {
      /* body is condition */
      struct _Node* Then;
      struct _Node* Else;
    } te;
  };
  /* for multiple call reference */
  OnigLen min_len;   /* min length (byte) */
  OnigLen max_len;   /* max length (byte) */
  int char_len;      /* character length  */
  int opt_count;     /* referenced count in optimize_node_left() */
} EnclosureNode;

#ifdef USE_CALL

typedef struct {
  int           offset;
  struct _Node* target;
} UnsetAddr;

typedef struct {
  int        num;
  int        alloc;
  UnsetAddr* us;
} UnsetAddrList;

typedef struct {
  NodeType node_type;
  int status;
  struct _Node* body; /* to EnclosureNode : ENCLOSURE_MEMORY */

  int     by_number;
  int     group_num;
  UChar*  name;
  UChar*  name_end;
  int     entry_count;
} CallNode;

#endif

typedef struct {
  NodeType node_type;
  int status;

  int  back_num;
  int  back_static[NODE_BACKREFS_SIZE];
  int* back_dynamic;
  int  nest_level;
} BackRefNode;

typedef struct {
  NodeType node_type;
  int status;
  struct _Node* body;

  int type;
  int char_len;
} AnchorNode;

typedef struct {
  NodeType node_type;
  int status;

  struct _Node* car;
  struct _Node* cdr;
} ConsAltNode;

typedef struct {
  NodeType node_type;
  int status;

  int ctype;
  int not;
  OnigOptionType options;
} CtypeNode;

typedef struct {
  NodeType node_type;
  int status;

  enum GimmickType type;
  int  detail_type;
  int  id;
} GimmickNode;

typedef struct _Node {
  union {
    struct {
      NodeType node_type;
      int status;
      struct _Node* body;
    } base;

    StrNode       str;
    CClassNode    cclass;
    QuantNode     quant;
    EnclosureNode enclosure;
    BackRefNode   backref;
    AnchorNode    anchor;
    ConsAltNode   cons;
    CtypeNode     ctype;
#ifdef USE_CALL
    CallNode      call;
#endif
    GimmickNode   gimmick;
  } u;
} Node;


#define NULL_NODE  ((Node* )0)

#define SCANENV_MEMENV_SIZE               8
#define SCANENV_MEMENV(senv) \
 (IS_NOT_NULL((senv)->mem_env_dynamic) ? \
    (senv)->mem_env_dynamic : (senv)->mem_env_static)

typedef struct {
  Node* node;
#if 0
  int in;
  int recursion;
#endif
} MemEnv;

typedef struct {
  enum SaveType type;
} SaveItem;

typedef struct {
  OnigOptionType   options;
  OnigCaseFoldType case_fold_flag;
  OnigEncoding     enc;
  OnigSyntaxType*  syntax;
  MemStatusType    capture_history;
  MemStatusType    bt_mem_start;
  MemStatusType    bt_mem_end;
  MemStatusType    backrefed_mem;
  UChar*           pattern;
  UChar*           pattern_end;
  UChar*           error;
  UChar*           error_end;
  regex_t*         reg;       /* for reg->names only */
  int              num_call;
#ifdef USE_CALL
  UnsetAddrList*   unset_addr_list;
  int              has_call_zero;
#endif
  int              num_mem;
#ifdef USE_NAMED_GROUP
  int              num_named;
#endif
  int              mem_alloc;
  MemEnv            mem_env_static[SCANENV_MEMENV_SIZE];
  MemEnv*           mem_env_dynamic;
#ifdef USE_COMBINATION_EXPLOSION_CHECK
  int num_comb_exp_check;
  int comb_exp_max_regnum;
  int curr_max_regnum;
  int has_recursion;
#endif
  unsigned int parse_depth;

  int keep_num;
  int save_num;
  int save_alloc_num;
  SaveItem* saves;
} ScanEnv;


#define IS_SYNTAX_OP(syn, opm)    (((syn)->op  & (opm)) != 0)
#define IS_SYNTAX_OP2(syn, opm)   (((syn)->op2 & (opm)) != 0)
#define IS_SYNTAX_BV(syn, bvm)    (((syn)->behavior & (bvm)) != 0)

#ifdef USE_NAMED_GROUP
typedef struct {
  int new_val;
} GroupNumRemap;

extern int    onig_renumber_name_table P_((regex_t* reg, GroupNumRemap* map));
#endif

extern int    onig_strncmp P_((const UChar* s1, const UChar* s2, int n));
extern void   onig_strcpy P_((UChar* dest, const UChar* src, const UChar* end));
extern void   onig_scan_env_set_error_string P_((ScanEnv* env, int ecode, UChar* arg, UChar* arg_end));
extern int    onig_scan_unsigned_number P_((UChar** src, const UChar* end, OnigEncoding enc));
extern void   onig_reduce_nested_quantifier P_((Node* pnode, Node* cnode));
extern void   onig_node_conv_to_str_node P_((Node* node, int raw));
extern int    onig_node_str_cat P_((Node* node, const UChar* s, const UChar* end));
extern int    onig_node_str_set P_((Node* node, const UChar* s, const UChar* end));
extern void   onig_node_free P_((Node* node));
extern Node*  onig_node_new_enclosure P_((int type));
extern Node*  onig_node_new_anchor P_((int type));
extern Node*  onig_node_new_str P_((const UChar* s, const UChar* end));
extern Node*  onig_node_new_list P_((Node* left, Node* right));
extern Node*  onig_node_list_add P_((Node* list, Node* x));
extern Node*  onig_node_new_alt P_((Node* left, Node* right));
extern void   onig_node_str_clear P_((Node* node));
extern int    onig_names_free P_((regex_t* reg));
extern int    onig_parse_tree P_((Node** root, const UChar* pattern, const UChar* end, regex_t* reg, ScanEnv* env));
extern int    onig_free_shared_cclass_table P_((void));
extern int    onig_is_code_in_cc P_((OnigEncoding enc, OnigCodePoint code, CClassNode* cc));
extern OnigLen onig_get_tiny_min_len(Node* node, unsigned int inhibit_node_types, int* invalid_node);

#ifdef ONIG_DEBUG
#ifdef USE_NAMED_GROUP
extern int onig_print_names(FILE*, regex_t*);
#endif
#endif

#endif /* REGPARSE_H */
