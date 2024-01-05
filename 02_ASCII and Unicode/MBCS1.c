// Multi Byte Character Set (MBCS)
#include <stdio.h>
#include <string.h>

int main(void) {

    char str[] = "ABC한글";  // 3 are alphabet(1 byte/char), 2 are hangul(2 byte/char)
                            // traditional problem of MBCS system
    int size = sizeof(str);
    int len = strlen(str);

    printf("Size of an array: %d\n", size);
    printf("Length of a string: %d\n", len);

    return 0;

}