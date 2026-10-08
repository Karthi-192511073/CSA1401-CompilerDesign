#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAX_LEN 31

const char* kw[] = {
    "auto","break","case","char","const","continue","default","do","double",
    "else","enum","extern","float","for","goto","if","int","long","register",
    "return","short","signed","sizeof","static","struct","switch","typedef",
    "union","unsigned","void","volatile","while"
};

int is_kw(const char* s) {
    for (int i = 0; i < 32; i++)
        if (!strcmp(s, kw[i])) return 1;
    return 0;
}

void analyze(const char* s) {
    int i = 0, len = strlen(s);
    while (i < len) {
        if (isspace(s[i])) { i++; continue; }

        if (s[i] == '/') {
            if (i + 1 < len && s[i + 1] == '/') {
                while (i < len && s[i] != '\n') i++;
                continue;
            }
            if (i + 1 < len && s[i + 1] == '*') {
                i += 2;
                while (i + 1 < len && !(s[i] == '*' && s[i + 1] == '/')) i++;
                i += 2;
                continue;
            }
        }

        if (isalpha(s[i]) || s[i] == '_') {
            char buf[MAX_LEN + 1];
            int k = 0;
            while (i < len && (isalnum(s[i]) || s[i] == '_')) {
                if (k < MAX_LEN) buf[k++] = s[i];
                i++;
            }
            buf[k] = '\0';
            printf("[%s] : %s\n", is_kw(buf) ? "KEYWORD" : "IDENTIFIER", buf);
            continue;
        }

        if (isdigit(s[i])) {
            char buf[64];
            int k = 0, dot = 0;
            while (i < len && (isdigit(s[i]) || (s[i] == '.' && !dot))) {
                if (s[i] == '.') dot = 1;
                if (k < 63) buf[k++] = s[i];
                i++;
            }
            buf[k] = '\0';
            printf("[CONSTANT]   : %s\n", buf);
            continue;
        }

        if (strchr("+-*/%=<>!&|^", s[i])) {
            if (i + 1 < len && strchr("+-*/%=<>!&|^", s[i + 1])) {
                printf("[OPERATOR]   : %c%c\n", s[i], s[i + 1]);
                i += 2;
            } else {
                printf("[OPERATOR]   : %c\n", s[i++]);
            }
            continue;
        }

        i++;
    }
}

int main() {
    const char code[] = "int main() {\n  float area = 3.14 * r * r; // formula\n  if (area >= 100) count += 1;\n}";
    analyze(code);
    return 0;
}
