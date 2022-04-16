#include <stdint.h>
#include <stdbool.h>

int string_length(char s[])
{
    int i = 0;
    while (s[i] != '\0')
        ++i;
    return i;
}

long atoi(const char *S)
{
    long num = 0;
    int i = 0;
    while (S[i] && (S[i] >= '0' && S[i] <= '9'))
    {
        num = num * 10 + (S[i] - '0');
        i++;
    }

    return num;
}

void strrev(char *arr, int start, int end)
{
    char temp;

    if (start >= end)
        return;

    temp = *(arr + start);
    *(arr + start) = *(arr + end);
    *(arr + end) = temp;

    start++;
    end--;
    strrev(arr, start, end);
}

void reverse(char s[])
{
    int c, i, j;
    for (i = 0, j = string_length(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int n, char str[])
{
    int i, sign;
    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do
    {
        str[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);

    if (sign < 0)
        str[i++] = '-';
    str[i] = '\0';

    reverse(str);
}

void append(char s[], char n)
{
    int len = string_length(s);
    s[len] = n;
    s[len + 1] = '\0';
}

bool backspace(char s[])
{
    int len = string_length(s);
    if (len > 0)
    {
        s[len - 1] = '\0';
        return true;
    }
    else
    {
        return false;
    }
}

int compare_string(char s1[], char s2[])
{
    int i;
    for (i = 0; s1[i] == s2[i]; i++)
    {
        if (s1[i] == '\0')
            return 0;
    }
    return s1[i] - s2[i];
}

int string_index_of(char str[], char ch)
{
    int i = 0;
    for (i = 0; i < string_length(str); i++)
    {
        if (str[i] == ch)
            return i;
    }
    return -1;
}
int string_to_int(char str[])
{
    return 0;
}
void sub_string(char str[], int start, int size, char ns[])
{
    int i = 0;
    if (size == -1)
    {
        size = string_length(str) - start;
    }
    for (i = 0; i < size; i++)
    {
        ns[i] = str[start + i];
    }
}