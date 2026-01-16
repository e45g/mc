#pragma once

#include "lexer.h"
#include "parser.h"

enum eval_type {
    EVAL_INT,
    EVAL_DOUBLE
};

typedef struct {
    enum eval_type type;
    union {
        int64_t i;
        double d;
    } r;
} val_t;

val_t *eval(ast_node_t *ast);
