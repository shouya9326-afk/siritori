#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_WORD_LENGTH 256
#define MAX_HISTORY 100

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

int is_in_dictionary(const char *word){
    FILE *fp = fopen("dictionary.txt", "r");
    if(fp==NULL){
        return 1;
    }
    char line[MAX_WORD_LENGTH];
    int found=0;
    while(fgets(line, sizeof(line), fp)!=NULL){
        size_t len = strlen(line);
        if(len > 0 && line[len - 1] == '\n'){
            line[len - 1] = '\0';
        } 
        if(strcmp(word,line)==0){
            found = 1;
            fclose(fp);
            return found;
        }   
    }
    fclose(fp);
    return found;
}

int main(void){
    char next_word[MAX_WORD_LENGTH];
    char history[MAX_HISTORY][MAX_WORD_LENGTH];
    int history_count = 0;
    const char *start_chars[] = {
    "あ", "い", "う", "え", "お",
    "か", "き", "く", "け", "こ",
    "さ", "し", "す", "せ", "そ",
    "た", "ち", "つ", "て", "と",
    "な", "に", "ぬ", "ね", "の",
    "は", "ひ", "ふ", "へ", "ほ",
    "ま", "み", "む", "め", "も",
    "や", "ゆ", "よ", "わ"
    };
    srand((unsigned int)time(NULL));  
    int idx = rand() % 39;
    strcpy(history[0], start_chars[idx]);
    history_count = 1;

    printf("しりとりを始めます！\n");
    printf("（終了するには Ctrl+D を押してください）\n\n");

    while(1){

        if(history_count==1){
            printf("最初の文字：%s\n", history[0]);
        } 
        else{
            printf("前の単語：%s\n", history[history_count-1]);
        }
        printf("次の単語を入力してください>");
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
        if (strcmp(next_word, "reset") == 0) {
            idx = rand() % 39;
            strcpy(history[0], start_chars[idx]);
            history_count = 1;
            printf("\nリセットしました。最初からやり直します。\n\n");
            continue;
        }

        size_t prev_len = strlen(history[history_count-1]);
        if (is_hiragana_only(next_word, len)==0) {
            printf("エラー: ひらがなを入力してください\n\n");
            continue;
        }
        
        if(is_in_dictionary(next_word)==0){
            printf("エラー: 辞書に存在しない単語です\n\n");
            continue;
        }

        if (strncmp(history[history_count-1] + prev_len - 3, next_word, 3) == 0) {
            if ((unsigned char)next_word[len - 3] == 0xE3 &&(unsigned char)next_word[len - 2] == 0x82 &&(unsigned char)next_word[len - 1] == 0x93) {
                printf("\n「%s」は「ん」で終わるため、ゲーム終了です！\n", next_word);
                break;
            }
            int duplicated = 0;
            for (int i = 1; i < history_count; i++) {
                if (strcmp(history[i], next_word) == 0) {
                    duplicated = 1;
                    break;
                }
            }
            if (duplicated) {
                printf("\n「%s」は既に使われた単語です。ゲーム終了です！\n", next_word);
                break;
            }
            strcpy(history[history_count], next_word);
            history_count++;
            printf("\n");
        } else {
            printf("エラー: 前の単語に続いていません\n\n");
        }
    }
    return 0;
}
