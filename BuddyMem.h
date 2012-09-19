

#ifndef __BUDDY_MEM_H__
#define __BUDDY_MEM_H__

#include"stdio.h"

typedef struct Node
{
	char* address;
	Node *next;
}LinkNode;

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
	void CaculateTableLinkListSize();
	unsigned int getTableLinkListIndex(unsigned int blockAmount);
	void deleteLinkList(LinkNode* head);

	unsigned int m_maxMemorySize;
	unsigned int m_blockSize;
	char* m_startPos;
	bool m_createFlag;
	
	//��¼��������׵�ַ�����鳤��
	unsigned int m_tableLinkListSize;
	//�������ͷ��ַ������
	LinkNode **m_emptyTableLinkListArray;
	LinkNode **m_usedTableLinkListArray;

};

#endif//__BUDDY_MEM_H__