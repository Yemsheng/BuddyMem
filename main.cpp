
#include<iostream>

using namespace std;

#include"BuddyMem.h"

int main()
{
	int a = 5;
	int i = 0;
	for(i=1;;i++)
	{
		if((a>>i)==0)
		{
			break;
		}
	}

	cout<<i<<endl;

	BuddyMem buddyMem(1024,32);
	buddyMem.BuddyMemCreate();
	buddyMem.BuddyMemDestroy();


	return 0;
}