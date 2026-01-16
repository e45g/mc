#pragma once

#include "lexer.h"

enum ast_expr {
    AST_INT,
    AST_DOUBLE,
    AST_BINARY_OP,
    AST_UNARY_OP,
    AST_FUNCTION, // SIN, COS, ...
};

typedef struct ast_node {
    enum ast_expr expr;

    union {
        int64_t i;
        double d;
    } val;

    struct {
        struct ast_node *left;
        struct ast_node *right;
        token_type_t op;
    } binary_op;

    struct {
        struct ast_node *node;
        token_type_t op;
    } unary_op;
} ast_node_t;

ast_node_t *expression(token_t **token);
ast_node_t *parse(token_t *token);
