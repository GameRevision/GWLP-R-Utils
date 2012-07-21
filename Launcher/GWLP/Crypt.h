#include <Windows.h>

void GrabPWHook(BYTE* addr);
void InjectPWHook(BYTE* addr,bool disableencryption);