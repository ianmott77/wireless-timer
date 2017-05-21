#include "tools.h"

char * str_append(char * src, const char * add_s){    
    //http://stackoverflow.com/questions/5901181/c-string-append
    char * new_str;
    if((new_str = (char *) malloc(strlen(src)+strlen(add_s)+1)) != NULL){
        new_str[0] = '\0';   // ensures the memory is an empty string
        strcpy(new_str, src);
        strcat(new_str, add_s);
        return new_str;
    } else {
        return NULL;
    }
}

char * substr(char * src, int start, int size){
    //http://stackoverflow.com/questions/4214314/get-a-substring-of-a-char
    char * sub = (char*) malloc(size);
    sub[0] = '\0';
    memcpy(sub, &(src[start]), size);
    sub[size] = '\0';
    return sub;
}

bool str_ends(char * src, const char * end){
    int src_n = strlen(src);
    int end_n = strlen(end);
    int start = src_n - end_n;
    for(int i = 0; i < end_n; i++){
        if(src[start] != end[i]){
            return false;
        }
        start++;
    }
    return true;
}

void mem(){
    Serial.print("mem: ");
    Serial.println(freeMemory());
}