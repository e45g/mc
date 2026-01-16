#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "eval.h"
#include "parser.h"

int64_t fact(int64_t i) {
    if(i > 1) {
        return i * fact(i-1);
    }
    else {
        return 1;
    }
}

int64_t comb(int64_t l, int64_t r) {
    if (r < 0 || r > l) return 0;
    if (r == 0 || r == l) return 1;

    if (r > l - r) r = l - r;

    int64_t result = 1;
    for (int64_t i = 1; i <= r; ++i) {
        result = result * (l - r + i) / i;
    }
    return result;
}

void free_val(val_t *v) {
    if (v != NULL) {
        free(v);
    }
}

val_t *walk(ast_node_t *ast) {
    switch(ast->expr) {
        case AST_BINARY_OP: {
            val_t *l = walk(ast->binary_op.left);
            val_t *r = walk(ast->binary_op.right);

            if (l == NULL || r == NULL) {
                free_val(l);
                free_val(r);
                return NULL;
            }

            val_t *v = malloc(sizeof(val_t));
            if (v == NULL) {
                free_val(l);
                free_val(r);
                return NULL;
            }

            switch (ast->binary_op.op) {
                case PLUS:
                    if (l->type == EVAL_INT && r->type == EVAL_INT) {
                        v->type = EVAL_INT;
                        v->r.i = l->r.i + r->r.i;
                    } else {
                        v->type = EVAL_DOUBLE;
                        double ld = (l->type == EVAL_INT) ? l->r.i : l->r.d;
                        double rd = (r->type == EVAL_INT) ? r->r.i : r->r.d;
                        v->r.d = ld + rd;
                    }
                    break;

                case MINUS:
                    if (l->type == EVAL_INT && r->type == EVAL_INT) {
                        v->type = EVAL_INT;
                        v->r.i = l->r.i - r->r.i;
                    } else {
                        v->type = EVAL_DOUBLE;
                        double ld = (l->type == EVAL_INT) ? l->r.i : l->r.d;
                        double rd = (r->type == EVAL_INT) ? r->r.i : r->r.d;
                        v->r.d = ld - rd;
                    }
                    break;

                case STAR:
                    if (l->type == EVAL_INT && r->type == EVAL_INT) {
                        v->type = EVAL_INT;
                        v->r.i = l->r.i * r->r.i;
                    } else {
                        v->type = EVAL_DOUBLE;
                        double ld = (l->type == EVAL_INT) ? l->r.i : l->r.d;
                        double rd = (r->type == EVAL_INT) ? r->r.i : r->r.d;
                        v->r.d = ld * rd;
                    }
                    break;

                case SLASH: {
                    double rd = (r->type == EVAL_INT) ? r->r.i : r->r.d;
                    if (rd == 0.0) {
                        fprintf(stderr, "Error: division by zero\n");
                        free_val(l);
                        free_val(r);
                        free_val(v);
                        return NULL;
                    }
                    v->type = EVAL_DOUBLE;
                    double ld = (l->type == EVAL_INT) ? l->r.i : l->r.d;
                    v->r.d = ld / rd;
                    break;
                }

                case COMB: {
                    v->type = EVAL_INT;
                    v->r.i = comb(l->r.i, r->r.i);
                    break;
                }

                case POWER: {
                    v->type = EVAL_DOUBLE;
                    double ld = (l->type == EVAL_INT) ? l->r.i : l->r.d;
                    double rd = (r->type == EVAL_INT) ? r->r.i : r->r.d;
                    v->r.d = pow(ld, rd);
                    break;
                }

                default:
                    fprintf(stderr, "Error: unknown binary operator\n");
                    free_val(l);
                    free_val(r);
                    free_val(v);
                    return NULL;
            }

            free_val(l);
            free_val(r);
            return v;
        }

        case AST_UNARY_OP: {
            val_t *n = walk(ast->unary_op.node);

            val_t *v = malloc(sizeof(val_t));

            switch(ast->unary_op.op) {
                case FACT: {
                    v->type = EVAL_INT;
                    v->r.i = fact(n->r.i);
                    break;
                }

                default:
                    return NULL;
            }
            return v;
        }

        case AST_INT: {
            val_t *v = malloc(sizeof(val_t));
            v->type = EVAL_INT;
            v->r.i = ast->val.i;
            return v;
        }
        case AST_DOUBLE: {
            val_t *v = malloc(sizeof(val_t));
            v->type = EVAL_DOUBLE;
            v->r.d = ast->val.d;
            return v;
        }
        case AST_FUNCTION: {
            break;
        }
    }
    return NULL;
}

val_t *eval(ast_node_t *ast) {
    val_t *r = walk(ast);
    return r;
}
