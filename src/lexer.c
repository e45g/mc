#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "lexer.h"

void lexer_push(token_t **head, token_t **tail, token_type_t type) {
    token_t *tmp = malloc(sizeof(token_t));
    tmp->next = NULL;
    tmp->type = type;

    if(*head == NULL) {
        *head = tmp;
        *tail = tmp;
    }
    else {
        (*tail)->next = tmp;
        *tail = tmp;
    }

}

token_t *lexer(char *buf, int size) {
    token_t *tail = NULL;
    token_t *head = NULL;
    int i = 0;
    char c = 0;

    // lexer_push(&head, &tail, INT);
    // tail->val.i=0;

    while(i < size && buf[i] != '\0') {
        unsigned char uc = (unsigned char)buf[i];
        c = tolower(uc);

        if(isspace(uc)) {
            i++;
            continue;
        }

        switch(c) {
            case '+': lexer_push(&head, &tail, PLUS); break;
            case '-': lexer_push(&head, &tail, MINUS); break;
            case '*': lexer_push(&head, &tail, STAR); break;
            case '/': lexer_push(&head, &tail, SLASH); break;
            case '(': lexer_push(&head, &tail, L_PAREN); break;
            case ')': lexer_push(&head, &tail, R_PAREN); break;
            case '^': lexer_push(&head, &tail, POWER); break;
            case 'c': lexer_push(&head, &tail, COMB); break;
            case '!': lexer_push(&head, &tail, FACT); break;

            default: {
                if(isdigit(uc)) {
                    int start = i;
                    int f = 0;
                    while (i < size && buf[i] != '\0' && (isdigit((unsigned char)buf[i]) || (buf[i] == '.' && f == 0))) {
                        if(buf[i] == '.') f = 1;
                        i++;
                    }
                    char *num = malloc(i - start);
                    memcpy(num, buf+start, i-start);
                    if(f == 0) {
                        lexer_push(&head, &tail, INT);
                        tail->val.i = strtol(num, NULL, 10);
                    }
                    if(f == 1) {
                        lexer_push(&head, &tail, DOUBLE);
                        tail->val.d = strtod(num, NULL);
                    }

                    free(num);
                    continue;
                }
                else {
                    fprintf(stderr, "Lexer error: invalid character '%c'\n", buf[i]);
                    return NULL;
                }
            }
        }
        i++;
    }
    lexer_push(&head, &tail, EOF);
    return head;
}
