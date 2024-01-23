#include <stdio.h>
#include <stdlib.h>

#define TAB_COUNT 4
#define MAX_LINE 1000

int detab(char *line, int sptab);
int entab(char *line, int sptab);
void replace_space(char *line, int sptab);

int main(int argc, char *argv[]) {
    int spetab, spdtab, len;
    char s[MAX_LINE];

    spetab = spdtab = TAB_COUNT;

    if (argc > 1) {
        while (--argc > 0 && ((*++argv)[0] == '-' || (*argv)[0] == '+')) {
            if ((*argv)[0] == '-') {
                spetab = atoi(++argv[0]);
            }
            else if ((*argv)[0] == '+') {
                spdtab = atoi(++argv[0]);
            }
        }
    }

    printf("Detab\n");

    while ((len = detab(s, spdtab)) > 0) {
        printf("%s", s);
    }

    //printf("Entab\n");

    //while ((len = entab(s, spetab)) > 0) {
    //    printf("%s", s);
    //}
    return 0;
}

int detab(char *line, int sptab) {
    int c, i;

    for (i=0; (c = getchar()) != EOF && c != '\n';) {
        if (i < MAX_LINE-1) {
            if (c == '\t') {
                do {
                    *(line + i++) = ' ';
                } while (i < MAX_LINE-1 && i%sptab);
            }
            else {
                *(line + i++) = c;
            }
        }
    }
    if (c == '\n') {
        if (i < MAX_LINE-1) {
            *(line + i++) = c;
        }
        else {
            *(line + MAX_LINE - 2) = '\n';
        }
    }
    if (i < MAX_LINE) {
        *(line + i) = '\0';
    }
    else {
        *(line + MAX_LINE - 1) = '\0';
    }
    return i;
}

int entab(char *line, int sptab) {
    int c, i;

    for (i=0; (c = getchar()) != EOF && c != '\n'; ++i) {
        if (i < MAX_LINE-1) {
            line[i] = c;
        }
    }
    if (c == '\n') {
        if (i < MAX_LINE-1) {
            line[i++] = c;
        }
        else {
            line[MAX_LINE - 2] = '\n';
        }
    }
    if (i < MAX_LINE) {
        line[i] = '\0';
    }
    else {
        line[MAX_LINE - 1] = '\0';
    }
    replace_space(line, sptab);
    return i;
}

void replace_space(char *line, int sptab) {
    int i, cur, is_tab, k;

    for (i = 0, cur = 0; i < MAX_LINE;) {
        if (line[i] == ' ') {
            k = i;
            do {
                is_tab = line[i++] == ' ';
            }
            while (i < MAX_LINE && i%sptab);
            if (is_tab) {
                line[cur++] = '\t';
            }
            else {
                for (; k < i; cur++, k++) {
                    line[cur] = line[k];
                }
            }
        }
        else {
            line[cur++] = line[i++];
        }
    }
}
