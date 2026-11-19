#ifndef TYPES_H
#define TYPES_H

#define S_R "\033[31m"
#define END "\033[0m"
#define S_G "\033[32m"
#define S_C "\033[36m"

/* User defined types */
typedef unsigned int uint;

/* Status will be used in fn. return type */
typedef enum
{
    e_success,
    e_failure
} Status;

typedef enum
{
    e_encode,
    e_decode,
    e_unsupported
} OperationType;

#endif
