#ifndef _LIBC_CTYPE_H_
#define _LIBC_CTYPE_H_

#define tolower(a) ((a) >= 'A' && (a) <= 'Z' ? (a) + 'Z' - 'A' : (a) )
#define toupper(a) ((a) >= 'a' && (a) <= 'z' ? (a) - 'Z' - 'A' : (a) )

#endif