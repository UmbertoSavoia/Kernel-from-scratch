#ifndef SHELL_H
#define SHELL_H

#define LEN_PROMPT 3

struct stackframe {
    struct stackframe* ebp;
    uint32 eip;
};

typedef struct  s_cmds {
    char *name;
    void (*f)(void);
}               t_cmds;

void    print_header(void);
char    *get_name_symbol(uint32 eip);
void    print_stack(void);
void    handler_cmds(void);

#endif