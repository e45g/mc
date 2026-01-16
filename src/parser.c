#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

ast_node_t *parse(token_t *token) {
    return expression(&token);
}

int expect(token_t **token, token_type_t type) {
    if(*token == NULL) return -1;
    return ((*token)->type == type) ? 1 : 0;
}

int expect_move(token_t **token, token_type_t type) {
    if(expect(token, type)) {
        if((*token)->next != NULL) *token = (*token)->next;
        return 1;
    }
    return 0;
}

void next(token_t **token) {
    if((*token)->next != NULL) {
        *token = (*token)->next;
    }
}

ast_node_t *factor(token_t **token) {
    switch((*token)->type) {
        case INT: {
            ast_node_t *node = malloc(sizeof(ast_node_t));
            node->expr = AST_INT;
            node->val.i = (*token)->val.i;
            next(token);
            return node;
        }

        case DOUBLE: {
            ast_node_t *node = malloc(sizeof(ast_node_t));
            node->expr = AST_DOUBLE;
            node->val.d = (*token)->val.d;
            next(token);
            return node;
        }
        case L_PAREN: {
            next(token);
            ast_node_t *node = expression(token);

            if (!expect_move(token, R_PAREN)) {
                fprintf(stderr, "dummy, missing )");
                return NULL;
            }
            return node;
            break;
        }
        default: {
            fprintf(stderr, "tf is this");
            return NULL;
        }
    }
    return NULL;
}

ast_node_t *prior(token_t **token) {
    ast_node_t *f = factor(token);
    if(f == NULL) return NULL;

    while(expect(token, POWER) || expect(token, COMB) || expect(token, FACT)) {
        if(expect(token, FACT)) {
            ast_node_t *node = malloc(sizeof(ast_node_t));
            node->expr = AST_UNARY_OP;
            node->unary_op.node= f;
            node->unary_op.op = (*token)->type;
            next(token);
            f = node;
        }
        else {
            ast_node_t *node = malloc(sizeof(ast_node_t));
            node->expr = AST_BINARY_OP;
            node->binary_op.left = f;
            node->binary_op.op = (*token)->type;
            next(token);
            node->binary_op.right = factor(token);
            f = node;
        }
    }

    return f;
}

ast_node_t *term(token_t **token) {
    ast_node_t *p = prior(token);
    if(p == NULL) return NULL;

    while(expect(token, STAR) || expect(token, SLASH)) {
            ast_node_t *node = malloc(sizeof(ast_node_t));
            node->expr = AST_BINARY_OP;
            node->binary_op.left = p;
            node->binary_op.op = (*token)->type;
            next(token);
            node->binary_op.right = prior(token);
            p = node;
    }

    return p;
}

ast_node_t *expression(token_t **token) {
    ast_node_t *t = term(token);
    if(t == NULL) return NULL;

    while(expect(token, PLUS) || expect(token, MINUS)) {
        ast_node_t *node = malloc(sizeof(ast_node_t));
        node->expr = AST_BINARY_OP;
        node->binary_op.left = t;
        node->binary_op.op = (*token)->type;
        next(token);
        node->binary_op.right = term(token);
        t = node;
    }

    return t;
}
