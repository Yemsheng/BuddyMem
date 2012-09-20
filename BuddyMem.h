

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
	*maxMemorySize必须为2的N次幂，blockSize必须为2的N次幂，maxMemorySize必须大于blockSize
	*/
	BuddyMem(unsigned int maxMemorySize, unsigned int blockSize):m_maxMemorySize(maxMemorySize), m_blockSize(blockSize),m_startPos(NULL),m_createFlag(false){}
	bool BuddyMemCreate();
	char* BuddyMemMalloc(const unsigned int);
	void BuddyMemFree();
	void BuddyMemDestroy();
private:
	void CaculateTableLinkListSize();

	//根据block数获取index
	unsigned int getTableLinkListIndexFromBlockAmount(unsigned int blockAmount);
	//根据index获取block数
	unsigned int getBlockAmountFromIndex(unsigned int index);

	//申请和释放table,该table记录index对应的块数
	bool initBlockAmountTable(void);
	void finitBlockAmountTable(void);

	//完整删除链表
	void deleteLinkList(LinkNode* head);

	//对链表节点的插入和删除
	bool insertNodeToTableLinkList(LinkNode **table, unsigned int index, LinkNode *node);
	void deleteFirstNodeFromTableLinkList(LinkNode **table, unsigned int index);

	//根据需要的block数计算出事实上分配出去的block数
	unsigned int getActuallyAllocBlock(unsigned int needBlock);


	unsigned int m_maxMemorySize;
	unsigned int m_blockSize;
	char* m_startPos;
	bool m_createFlag;
	
	//存放每个index对应的block数
	unsigned int *m_blockAmountTable;

	//记录存放链表首地址的数组长度
	unsigned int m_tableLinkListSize;

	//存放链表头地址的数组
	LinkNode **m_emptyTableLinkListArray;
	LinkNode **m_usedTableLinkListArray;

};

#endif//__BUDDY_MEM_H__