#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "parser.h"
#include "eval.h"

void lexer_print(token_t *t);
const char* token_type_to_string(token_type_t type);
void print_ast_node(ast_node_t *node, int depth);

int main(void) {
    char buf[4096] = {0};
    while (1) {
        printf(">> ");
        fflush(stdout);

        fgets(buf, sizeof(buf), stdin);
        *(strchr(buf, '\n')) = '\0';

        token_t *t = lexer(buf, sizeof(buf));
        // lexer_print(t);

        ast_node_t *ast = parse(t);
        // print_ast_node(ast, 0);

        val_t *r = eval(ast);
        if(r->type == EVAL_INT) {
            printf("Result: %ld\n", r->r.i);
        }
        else {
            printf("Result: %lf\n", r->r.d);
        }
    }
}
