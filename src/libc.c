#include "../include/kernel.h"
#include "../include/libc.h"

int     toupper(int c)
{
    return (('a' <= c && c <= 'z') ? (c - 32) : c);
}

int     tolower(int c)
{
    return (('A' <= c && c <= 'Z') ? (c + 32) : c);
}

char    *strnstr(const char *s1, const char *s2, uint32 len)
{
    uint32 l2;

    l2 = 0;
    while (s2[l2])
        l2++;
    if (!l2)
        return ((char*)s1);
    while (len >= l2)
    {
        if (!memcmp(s1, s2, l2))
            return ((char*)s1);
        s1++;
        len--;
    }
    return (0);
}

void    *memset(void *s, int c, uint32 n)
{
    char *temp;

    temp = s;
    while (n--)
        *temp++ = (unsigned char)c;
    return (s);
}

void    *memmove(void *dest, const void *src, uint32 n)
{
    char        *tmp;
    const char  *s;

    if (!dest && !src)
        return (0);
    if (dest <= src)
    {
        tmp = dest;
        s = src;
        while (n--)
            *tmp++ = *s++;
    }
    else
    {
        tmp = dest;
        tmp += n;
        s = src;
        s += n;
        while (n--)
            *--tmp = *--s;
    }
    return (dest);
}

void    *memcpy(void *dest, const void *src, uint32 n)
{
    char        *temp;
    const char  *s;

    if (!dest && !src)
        return (0);
    s = src;
    temp = dest;
    while (n--)
        *temp++ = *s++;
    return (dest);
}

int     memcmp(const void *s1, const void *s2, uint32 n)
{
    const unsigned char *su1;
    const unsigned char *su2;
    int                 ret;

    su1 = s1;
    su2 = s2;
    ret = 0;
    while (0 < n)
    {
        if ((ret = *su1 - *su2) != 0)
            break ;
        su1++;
        su2++;
        n--;
    }
    return (ret);
}

void    *memchr(const void *s, int c, uint32 n)
{
    const unsigned char	*temp;

    temp = s;
    while (n--)
    {
        if (*temp == (unsigned char)c)
            return ((void*)temp);
        temp++;
    }
    return (0);
}

void    *memccpy(void *dest, const void *src, int c, uint32 n)
{
    while (n--)
    {
        *(char*)dest++ = *(char*)src;
        if (*(unsigned char*)src == (unsigned char)c)
            return (dest);
        src++;
    }
    return (0);
}

int     isprint(int c)
{
    return (' ' <= c && c <= '~');
}

int     isdigit(int c)
{
    return ('0' <= c && c <= '9');
}

int     isascii(int c)
{
    return (0 <= c && c <= 127);
}

int     isalpha(int c)
{
    return (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'));
}

int     isalnum(int c)
{
    return (isalpha(c) || isdigit(c));
}

void    bzero(void *s, uint32 n)
{
    while (n--)
        *(char*)s++ = 0;
}

uint32  strlen(const char *s)
{
    uint32 ret = 0;

    while (s && *s++)
        ret++;
    return (ret);
}

uint32 nbrlen(int num, uint32 base_len)
{
    uint32 count = 0;
    uint32 u_num = num < 0 ? -(uint32)num : (uint32)num;

    if(num == 0)
        return 1;
    while(u_num > 0) {
        count++;
        u_num = u_num / base_len;
    }
    return count;
}

void    itoa(int num, int base_len, char *str)
{
    int len_num = nbrlen(num, base_len);
    char *base = (base_len == 10) ? "0123456789" : "0123456789abcdef";
    uint32 u_num = num < 0 ? -(uint32)num : (uint32)num;

    if (num == 0 && len_num == 1) {
        str[0] = '0';
        str[1] = 0;
    } else {
        if (num < 0)
            str[0] = '-';
        for (int i = num < 0 ? len_num : len_num - 1; u_num != 0; --i, u_num /= base_len) {
            str[i] = base[u_num % base_len];
        }
        str[num < 0 ? len_num + 1 : len_num] = 0;
    }
}

int     atoi(const char *str)
{
    int ret;
    int neg;

    ret = 0;
    neg = 1;
    while (*str == '\n' || *str == '\t' || *str == ' '
           || *str == '\v' || *str == '\r' || *str == '\f')
        str++;
    if (*str == '+' || *str == '-')
    {
        if (*str == '-')
            neg *= -1;
        str++;
    }
    while (*str >= '0' && *str <= '9')
    {
        ret = ret * 10 + (*str - '0');
        str++;
    }
    return (ret * neg);
}

uint32  putnbr(int num, int base_len)
{
    char s_num[nbrlen(num, base_len) + 2];

    itoa(num, base_len, s_num);
    putstr(s_num);
    return strlen(s_num);
}

int     printf(const char *fmt, ...)
{
    int ret = 0;
    char tmp[3] = {0};
    char *str = 0;
    int num = 0;
    int c = 0;
    uint32 unum = 0;
    uint8 count = 0;
    uint8 width = 0; // max width = 99
    uint8 meno = 0;

    va_list ap;
    va_start(ap, fmt);

    while (*fmt) {
        meno = width = num = count = unum = c = 0;
        if (*fmt == '%') {
            ++fmt;
            if (*fmt == '-') {
                meno = 1;
                ++fmt;
            }
            if (isdigit(*fmt)) {
                bzero(tmp, 3);
                for (uint8 i = 0; isdigit(*fmt) && i < 3; ++i, ++fmt) {
                    tmp[i] = *fmt;
                }
                width = atoi(tmp);
            }
            switch (*fmt) {
                case 's':
                    str = va_arg(ap, char*);
                    if (width > 0) {
                        count = width - strlen(str);
                        if (meno) {
                            ret += putstr(str);
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar(' ');
                            }
                        } else {
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar(' ');
                            }
                            ret += putstr(str);
                        }
                    } else {
                        ret += putstr(str);
                    }
                    break;
                case 'd':
                    num = va_arg(ap, int);
                    if (width > 0) {
                        count = width - nbrlen(num, 10);
                        if (meno) {
                            ret += putnbr(num, 10);
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar('0');
                            }
                        } else {
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar('0');
                            }
                            ret += putnbr(num, 10);
                        }
                    } else {
                        ret += putnbr(num, 10);
                    }
                    break;
                case 'x':
                    unum = va_arg(ap, uint32);
                    if (width > 0) {
                        count = width - nbrlen(unum, 16);
                        if (meno) {
                            ret += putnbr(unum, 16);
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar('0');
                            }
                        } else {
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar('0');
                            }
                            ret += putnbr(unum, 16);
                        }
                    } else {
                        ret += putnbr(unum, 16);
                    }
                    break;
                case 'u':
                    unum = va_arg(ap, uint32);
                    if (width > 0) {
                        count = width - nbrlen(unum, 10);
                        if (meno) {
                            ret += putnbr(unum, 10);
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar('0');
                            }
                        } else {
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar('0');
                            }
                            ret += putnbr(unum, 10);
                        }
                    } else {
                        ret += putnbr(unum, 10);
                    }
                    break;
                default:
                    c = va_arg(ap, int);
                    if (width > 0) {
                        count = width - 1;
                        if (meno) {
                            putchar(c);
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar(' ');
                            }
                        } else {
                            if (count > 0) {
                                ret += count;
                                for (int i = 0; i < count; ++i)
                                    putchar(' ');
                            }
                            putchar(c);
                        }
                    } else {
                        putchar(c);
                    }
                    ++ret;
                    break;
            }
        } else if (*fmt == '#') {
            ++fmt;
            if (isdigit(*fmt)) {
                bzero(tmp, 3);
                for (int i = 0; isdigit(*fmt); ++i, ++fmt)
                    tmp[i] = *fmt;
                fore_color = atoi(tmp);
                continue;
            }
        } else {
            putchar(*fmt);
        }
        ++fmt;
    }
    va_end(ap);

    return ret;
}