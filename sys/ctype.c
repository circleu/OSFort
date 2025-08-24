#include "ctype.h"


int isalnum(int c) {
    return (('0' <= c && c <= '9') || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}
int isalpha(int c) {
    return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}
int isascii(int c) {
    return (0 <= c && c <= 127);
}
int isblank(int c) {
    return (c == '\t' || c == ' ');
}
int iscntrl(int c) {
    return ((0 <= c && c <= 31) || c == 127);
}
int isdigit(int c) {
    return ('0' <= c && c <= '9');
}
int isgraph(int c) {
    return (33 <= c && c <= 126);
}
int islower(int c) {
    return ('a' <= c && c <= 'z');
}
int isprint(int c) {
    return (32 <= c && c <= 126);
}
int ispunct(int c) {
    return ((33 <= c && c <= 47) || (58 <= c && c <= 64) || (91 <= c && c <= 96) || (123 <= c && c <= 126));
}
int isspace(int c) {
    return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
}
int isupper(int c) {
    return ('A' <= c && c <= 'Z');
}
int isxdigit(int c) {
    return (('0' <= c && c <= '9') || ('A' <= c && c <= 'F') || ('a' <= c && c <= 'f'));
}
int toascii(int c) {
    return (c & 0b1111111);
}
int toupper(int c) {
    if (islower(c))
        return _toupper(c);
    else
        return 0;
}
int tolower(int c) {
    if (isupper(c))
        return _tolower(c);
    else
        return 0;
}