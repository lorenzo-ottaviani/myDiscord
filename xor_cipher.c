#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* xor_encryption(char *src)
{
    /*
    function that randomly creates a one bit integer as key
    then a string character is encrypted with our key
    */
    int key = (rand() % 255) + 1 ;
    int taille = strlen(src) ;

    char *crypte = malloc(sizeof(char)* (taille +1));
    if (crypte == NULL)
    {
        return NULL ;
    }

    int indice = 0 ;
    while (src[indice] != '\0')
    {
        char ciph_letter = src[indice] ^ key ;
        crypte[indice] = ciph_letter ;

        indice ++ ;
    }
    crypte[taille-1] = '\0';
    
    return crypte ;
}

