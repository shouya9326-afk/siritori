#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 256

int main(void){
    char input[MAX_WORD_LENGTH];
    printf("単語を入力してください");

    if(fgets(input, sizeof(input), stdin) == NULL){
        return 0;
    }
    size_t len=strlen(input);
    if(len>0&&input[len - 1]=='\n'){
        input[len-1]='\0';
    }
    printf("入力された単語：%s\n", input);
    return 0;
}