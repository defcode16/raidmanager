/* 
 * File:   input.c
 * Author: dennis
 *
 * Created on 19 de agosto de 2013, 05:58 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
/*
 * 
 */

const char* concat(const char* str1, const char* str2)
{
    char* result;
    sprintf(&result, "%s%s", str1, str2);
    return result;
}
void clean(){
    int c;
    do{
        c = getchar();
    }while(c != EOF && c != '\n');
}

char *read(int m_size) {
    char *line;
    int size = 0;
    line = malloc(m_size + 1);
    while ( (size < m_size) && (line[size]=getchar())!= EOF) {
        size++;
    }
    line[size] = 0;
    return line;
}

void strip(char *s) {
    char *p2 = s;
    while(*s != '\0') {
        if(*s != '\t' && *s != '\n') {
            *p2++ = *s++;
        } else {
            ++s;
        }
    }
    *p2 = '\0';
}


char *readln(int m_size){
    char *line;
    int c;
    line = malloc(m_size + 1);
    
    clean();
    fgets(line,m_size,stdin);
    
    strip(line);
    return line;
}

void waitkey(){
    readln(10);
}

char readchar(){
    clean();
    char c = getchar();
    return c;
}
