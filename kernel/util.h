#pragma once

#include <stdint.h>
#include <stdbool.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

int string_length(char s[]);

void reverse(char s[]);

void itoa(int n, char str[]);

bool backspace(char s[]);

void append(char s[], char n);

int compare_string(char s1[], char s2[]);

int string_index_of(char str[] ,char ch);

int  string_to_int(char str[]);

void sub_string(char str[],int start,int size,char ns[]);

long atoi(const char *S);