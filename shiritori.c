#include <stdio.h>
#include <string.h>

#define MAX_WORD_LENGTH 256

int is_hiragana_only(const char *word, size_t len) {
    if (len % 3 != 0) return 0;
    for (size_t i = 0; i < len; i += 3) {
        unsigned char b0 = (unsigned char)word[i];
        unsigned char b1 = (unsigned char)word[i + 1];
        if (b0 != 0xE3) return 0;
        if (b1 < 0x81 || b1 > 0x82) return 0;
    }
    return 1;
}

int main(void){
    char next_word[MAX_WORD_LENGTH];
    char previous_word[MAX_WORD_LENGTH]="しりとり";
    printf("しりとりを始めます！\n");
    printf("（終了するには Ctrl+D を押してください）\n\n");
    while(1){
        printf("前の単語：%s\n", previous_word);
        printf("次の単語を入力してください");

        if(fgets(next_word, sizeof(next_word), stdin) == NULL){
             printf("しりとりを終了します。\n");
             break;
        }
        size_t len=strlen(next_word);
        if(len>0&&next_word[len - 1]=='\n'){
            next_word[len-1]='\0';
            len--;
        }
        if(len==0){
            continue;
        }
        size_t prev_len = strlen(previous_word);
        if (is_hiragana_only(next_word, len)==0) {
            printf("エラー: ひらがなを入力してください\n\n");
            continue;
        }
        if (strncmp(previous_word + prev_len - 3, next_word, 3) == 0) {
            strcpy(previous_word, next_word);
            printf("\n");
        } else {
            printf("エラー: 前の単語に続いていません\n\n");
        }
    }
    return 0;
}
