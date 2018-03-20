#include "m_string.h"

uint8_t str_cmp(const char *str1, const char *str2){
    do{
        
        if( *str1  !=  *str2 )
            return 1;
        str2++;
    }while(*str1++);
    return 0;
}