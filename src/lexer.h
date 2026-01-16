#pragma once

typedef long int int64_t;

typedef enum token_type {
    INT,
    DOUBLE,

    PLUS,
    MINUS,
    STAR,
    SLASH,
    L_PAREN,
    R_PAREN,

    POWER,
    COMB,
    FACT,

    OEF,
} token_type_t;

union token_val {
    double d;
    int64_t i;
};

typedef struct token {
    token_type_t type;
    union token_val val;
    struct token *next;
} token_t;

token_t *lexer(char *buf, int size);
