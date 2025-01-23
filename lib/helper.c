//#include "cesocket.h"
#include <stdio.h>
#include <unistd.h>
#include "type.h"
#include <string.h>
#include <stdlib.h>

void saveEmbedding(const char *filename_word, const char *filename_embed, Embedding *Embed){
    FILE *fileEmbed = fopen(filename_embed, "wb");
    FILE *fileWord = fopen(filename_word, "wb");
    if(!fileEmbed || !fileWord){
        
    }
    int charLength = 0;
    int embedLength = Embed->count * Embed->token_per_word;

    // word->data = realloc(word->data, newWord->size * sizeof(char*))
    // for(int i = 0; i < newWord->size; i++){
    //     word->data[word->size + i] = malloc()
    // }

    printf("saving %zu embedding data with %zu byte\n", Embed->count, embedLength * sizeof(float));

    fwrite(&embedLength, sizeof(int), 1, fileEmbed);
    fwrite(&Embed->count, sizeof(int), 1, fileWord);
    fwrite(Embed->token, sizeof(float), embedLength, fileEmbed);
    for(int i = 0; i < Embed->count; i++){
        charLength = strlen(Embed->word[i]) + 1;
        fwrite(&charLength, sizeof(int), 1, fileWord);
        fwrite(Embed->word[i], sizeof(char), charLength, fileWord);
    }

    fclose(fileEmbed);
    fclose(fileWord);
}

void readEmbedding(const char *filename_word, const char *filename_embed, Embedding *Embed){
    FILE *fileEmbed = fopen(filename_embed, "rb");
    FILE *fileWord = fopen(filename_word, "rb");
    int countChar = 0;
    int countEmbed = 0;
    int countWord = 0;
    fread(&countEmbed, sizeof(int), 1, fileEmbed);
    fread(&countWord, sizeof(int), 1, fileWord);
    //char **word = malloc(sizeof(char) * countWord);
    //float *data = (float *)malloc(sizeof(float) * countEmbed);
    Embed->word = malloc(sizeof(char) * countWord);
    Embed->token = (float *)malloc(sizeof(float) * countEmbed);
    Embed->token_per_word = countEmbed / countWord;

    if(!fileEmbed || !fileWord){
        
    }

    fread(Embed->token, sizeof(float), countEmbed, fileEmbed);
    // for(int i = 0; i < countEmbed; i++){
    //     printf("%f, ", data[i]);
    // }

    for(int i = 0; i < countWord; i++){
        fread(&countChar, sizeof(int), 1, fileWord);
        Embed->word[i] = malloc(sizeof(char) * countChar);
        fread(Embed->word[i], sizeof(char), countChar, fileWord);
    }

    printf("\n {");
    for(int i = 0; i < countWord; i++){
        printf("%s", Embed->word[i]);
        printf(", ");
        //free(word[i]);
    }
    printf("} \n");

    fclose(fileEmbed);
    fclose(fileWord);
    //free(data);
    //free(word);
}

void randomVector(float *data){
    float num = ((float)rand() / (float)RAND_MAX) * 2.0f - 1.0f;
    data = &num;
}

void freeEmbed(Embedding *Embed){
    for(int i = 0; i < Embed->count; i++){
        free(Embed->word[i]);
    }
    free(Embed->word);
    free(Embed->token);
}

void initEmbed(Embedding *Embed, char **word, int token_per_word){
    Embed->count = sizeof(word) / sizeof(word[0]);
    Embed->token_per_word = token_per_word;
    Embed->token = (float *)malloc(sizeof(float) * Embed->count * Embed->token_per_word);
    memcpy(Embed->word, word, sizeof(word));
    for(int i = 0; i < Embed->count * Embed->token_per_word; i++){
        randomVector(&Embed->token[i]);
    }
}

void printExampleToken(Embedding *Embed, int start, int count){
    int loc = start * Embed->token_per_word;
    for(int i = 0; i <= count; i++){
        loc = (start + i) * Embed->token_per_word;
        printf("word: %s, embed: {%f, %f, ..., %f, %f} \n", Embed->word[start+i], Embed->token[loc], Embed->token[loc+1], Embed->token[loc+Embed->token_per_word-2], Embed->token[loc+Embed->token_per_word-1]);
    }
}