#include <stdio.h>

#define MAXLINE 1000

int getline_c(char s[], int lim);
void copy(char to[], char from[]);

int main() {
    int len, max;
    char line[MAXLINE];
    char longest[MAXLINE];

    max = 0;
    while ((len = getline_c(line, MAXLINE)) > 0) {
        if (len > max) {
            max = len;
            copy(longest, line);
        }
    }
    if (max > 0) {
        printf("\nLongest line has length %d and it is:\n", max);
        printf("%s\n", longest);
    }
    return 0;
}

int getline_c(char s[], int lim) {
    int c, i;

    for (i = 0; (c=getchar()) != EOF && c != '\n'; ++i) {
        if (i < lim-1) {
            s[i] = c;
        }
    }
    if (c == '\n') {
        if (i < lim-1) {
            s[i] = c;
            ++i;
        }
        else {
            s[lim-2] = '\n';
        }
    }
    if (i < lim) {
        s[i] = '\0';
    }
    else {
        s[lim-1] = '\0';
    }
    return i;
}

void copy(char to[], char from[]) {
    int i;
    i = 0;
    while ((to[i] = from[i]) != '\0') {
        ++i;
    }
}
