#ifndef LIBC_H
#define LIBC_H

typedef __builtin_va_list va_list;
#define va_start(v, l)	__builtin_va_start(v, l)
#define va_end(v)	__builtin_va_end(v)
#define va_arg(v, T)	__builtin_va_arg(v, T)
#define va_copy(d, s)	__builtin_va_copy(d, s)

int     printf(const char *fmt, ...);

uint32  putnbr(int num, int base_len);
void    itoa(int num, int base_len, char *str);
uint32  nbrlen(int num, uint32 base_len);
uint32  strlen(const char *s);
void    bzero(void *s, uint32 n);
int     isalnum(int c);
int     isalpha(int c);
int     isascii(int c);
int     isdigit(int c);
int     isprint(int c);
void    *memccpy(void *dest, const void *src, int c, uint32 n);
void    *memchr(const void *s, int c, uint32 n);
int     memcmp(const void *s1, const void *s2, uint32 n);
void    *memcpy(void *dest, const void *src, uint32 n);
void    *memmove(void *dest, const void *src, uint32 n);
void    *memset(void *s, int c, uint32 n);
char    *strnstr(const char *s1, const char *s2, uint32 len);
int     tolower(int c);
int     toupper(int c);
int     atoi(const char *str);

#endif
