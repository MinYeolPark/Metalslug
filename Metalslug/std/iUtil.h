#pragma once

// ================
// FILE IO
// ================
void saveFile(const char* fileName, char* buf, int len);
char* loadFile(const char* fileName, int& len);
bool existFile(const char* fileName);
void removeFile(const char* fileName);

#define _sin(degree) sinf((float)M_PI / 180 * (degree))
#define _cos(degree) cosf((float)M_PI / 180 * (degree))

