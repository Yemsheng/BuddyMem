
#include<iostream>

using namespace std;

#include"BuddyMem.h"

int main()
{
	BuddyMem buddyMem(1024,1);
	buddyMem.BuddyMemCreate();
	char *p1 = buddyMem.BuddyMemMalloc(3);
	buddyMem.BuddyMemDestroy();

	return 0;
}