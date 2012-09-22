
#include<iostream>

using namespace std;

#include"BuddyMem.h"

int main()
{
	BuddyMem buddyMem(1024,1);
	buddyMem.BuddyMemCreate();
	char *p1 = buddyMem.BuddyMemMalloc(3);
	buddyMem.BuddyMemFree(p1);
	//char *p2 = buddyMem.BuddyMemMalloc(33);
	//char *p3 = buddyMem.BuddyMemMalloc(33);
	buddyMem.BuddyMemDestroy();

	return 0;
}