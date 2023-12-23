#include <stdio.h>

int main(void) {

    char str[] = "한글입니다";
    unsigned int i = 0;

    for(i = 0; i < 6; i++)
        fputc(str[i], stdout);
    fputs("\n", stdout);

    // The actual "length" of the given string str is 5,
    // but we have to run fputc() 15 times to print that "5-length" string
    for(i = 0; i < 15; i++)
        fputc(str[i], stdout);
    fputs("\n", stdout);

    return 0;

}