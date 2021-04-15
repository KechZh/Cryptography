#include <stdio.h>
#include <string.h>

int main(){
    FILE *fp = fopen("text.txt", "w");
    char *str = "This person is a gay\n";
    int len = strlen(str);

    for(int i = 0; i < 10 * 1024 * 1024 / len; i++)
        fwrite(str, 1, len, fp);

    return 0;
}