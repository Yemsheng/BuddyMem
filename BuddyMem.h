

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

	//����block����ȡindex
	unsigned int getTableLinkListIndexFromBlockAmount(unsigned int blockAmount);
	//����index��ȡblock��
	unsigned int getBlockAmountFromIndex(unsigned int index);

	//������ͷ�table,��table��¼index��Ӧ�Ŀ���
	bool initBlockAmountTable(void);
	void finitBlockAmountTable(void);

	//����ɾ������
	void deleteLinkList(LinkNode* head);

	//������ڵ�Ĳ����ɾ��
	bool insertNodeToTableLinkList(LinkNode **table, unsigned int index, LinkNode *node);
	void deleteFirstNodeFromTableLinkList(LinkNode **table, unsigned int index);

	//������Ҫ��block���������ʵ�Ϸ����ȥ��block��
	unsigned int getActuallyAllocBlock(unsigned int needBlock);


	unsigned int m_maxMemorySize;
	unsigned int m_blockSize;
	char* m_startPos;
	bool m_createFlag;
	
	//���ÿ��index��Ӧ��block��
	unsigned int *m_blockAmountTable;

	//��¼��������׵�ַ�����鳤��
	unsigned int m_tableLinkListSize;

	//�������ͷ��ַ������
	LinkNode **m_emptyTableLinkListArray;
	LinkNode **m_usedTableLinkListArray;

};

#endif//__BUDDY_MEM_H__