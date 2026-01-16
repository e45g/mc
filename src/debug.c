#include <stdio.h>

#include "lexer.h"
#include "parser.h"

void lexer_print(token_t *t) {
    int i = 0;
    for(token_t *c = t; c != NULL; c = c->next) {
        if(c->type == INT) {
            printf("[%d] %d , %ld\n", i, c->type, c->val.i);
        }
        else if(c->type == DOUBLE) {
            printf("[%d] %d , %g\n", i, c->type, c->val.d);
        }
        else {
            printf("[%d] %d\n", i, c->type);
        }
        i++;
    }
}

const char* token_type_to_string(token_type_t type) {
    switch(type) {
        case PLUS: return "+";
        case MINUS: return "-";
        case STAR: return "*";
        case SLASH: return "/";
        case FACT: return "!";
        case COMB: return "C";
        case POWER: return "^";
        default: return "?";
    }
}

void print_ast_node(ast_node_t *node, int depth) {
    if (node == NULL) {
        for (int i = 0; i < depth; i++) printf("  ");
        printf("(null)\n");
        return;
    }

    for (int i = 0; i < depth; i++) printf("  ");

    switch(node->expr) {
        case AST_INT:
            printf("INT: %ld\n",node->val.i);
            break;

        case AST_DOUBLE:
            printf("DOUBLE: %lf\n", node->val.d);
            break;

        case AST_BINARY_OP:
            printf("BINARY_OP: %s\n", token_type_to_string(node->binary_op.op));

            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("+- LEFT:\n");
            print_ast_node(node->binary_op.left, depth + 2);

            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("+- RIGHT:\n");
            print_ast_node(node->binary_op.right, depth + 2);
            break;

        case AST_UNARY_OP:
            printf("UNARY_OP: %s\n", token_type_to_string(node->unary_op.op));

            for (int i = 0; i < depth + 1; i++) printf("  ");
            printf("+- NODE:\n");
            print_ast_node(node->unary_op.node, depth + 2);
            break;

        default:
            printf("UNKNOWN_NODE_TYPE: %d\n", node->expr);
            break;
    }
}
