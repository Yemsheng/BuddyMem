

#ifndef __BUDDY_MEM_H__
#define __BUDDY_MEM_H__

#include"stdio.h"

class BuddyMem
{
public:
	/*
	*maxMemorySize����Ϊ2��N���ݣ�blockSize����Ϊ2��N���ݣ�maxMemorySize�������blockSize
	*/
	BuddyMem(unsigned int maxMemorySize, unsigned int blockSize):m_maxMemorySize(maxMemorySize), m_blockSize(blockSize),m_startPos(NULL),m_createFlag(false){}
	bool BuddyMemCreate();
	char* BuddyMemMalloc(const unsigned int);
	void BuddyMemFree();
	void BuddyMemDestroy();
private:
	unsigned int m_maxMemorySize;
	unsigned int m_blockSize;
	char* m_startPos;
	bool m_createFlag;
	unsigned int m_tableLinkListSize;
	void CaculateTableLinkListSize();
};

#endif//__BUDDY_MEM_H__