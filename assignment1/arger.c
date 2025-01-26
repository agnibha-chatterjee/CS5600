#include <stdio.h>

// Custom strlen implementation
size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

// Custom strcmp implementation
int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char *)s1 - *(const unsigned char *)s2;
}

// Function to convert string to uppercase
void to_upper(char *str) {
    while (*str) {
        if (*str >= 'a' && *str <= 'z') {
            *str -= 32;
        }
        str++;
    }
}

// Function to convert string to lowercase
void to_lower(char *str) {
    while (*str) {
        if (*str >= 'A' && *str <= 'Z') {
            *str += 32;
        }
        str++;
    }
}

// Function to capitalize each word
void capitalize(char *str) {
    int capitalize_next = 1;
    while (*str) {
        if (capitalize_next && *str >= 'a' && *str <= 'z') {
            *str -= 32;
            capitalize_next = 0;
        } else if (!capitalize_next && *str >= 'A' && *str <= 'Z') {
            *str += 32;
        }
        if (*str == ' ') {
            capitalize_next = 1;
        }
        str++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: arger [-u | -l | -cap] <text>\n");
        return -1;
    }

    char *option = argv[1];
    char *text = argv[2];

    // Combine all arguments after the option into one string
    for (int i = 3; i < argc; i++) {
        sprintf(text + strlen(text), " %s", argv[i]);
    }

    if (strcmp(option, "-u") == 0) {
        to_upper(text);
    } else if (strcmp(option, "-l") == 0) {
        to_lower(text);
    } else if (strcmp(option, "-cap") == 0) {
        capitalize(text);
    } else {
        printf("Invalid option. Use -u, -l, or -cap\n");
        return -1;
    }

    printf("%s\n", text);
    return 0;
} 