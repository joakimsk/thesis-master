#ifndef __CHEAP_FUNCTIONS_H_INCLUDED__
#define __CHEAP_FUNCTIONS_H_INCLUDED__
#include <iostream>

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
bool String2Int(const std::string& str, int& result);
bool String2Float(const std::string& str, float& result);
void clear();

#endif