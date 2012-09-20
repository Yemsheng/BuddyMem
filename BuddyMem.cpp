
#include"BuddyMem.h"

#include<iostream>

bool BuddyMem::BuddyMemCreate()
{
	if(m_maxMemorySize<m_blockSize)
	{
		m_createFlag = false;
		return m_createFlag;
	}

	m_startPos = new char[m_maxMemorySize];
	if(m_startPos==NULL)
	{
		m_createFlag = false;
		return m_createFlag;
	}
	memset(m_startPos,0,m_maxMemorySize*sizeof(char));

	//����empty table��used table
	CaculateTableLinkListSize();
	m_emptyTableLinkListArray = new LinkNode*[m_tableLinkListSize];
	memset(m_emptyTableLinkListArray,0,m_tableLinkListSize*sizeof(LinkNode*));
	m_usedTableLinkListArray = new LinkNode*[m_tableLinkListSize];
	memset(m_usedTableLinkListArray,0,m_tableLinkListSize*sizeof(LinkNode*));
	
	LinkNode *firstNode = new LinkNode;
	firstNode->address = m_startPos;
	firstNode->next = NULL;
	m_emptyTableLinkListArray[m_tableLinkListSize-1] = firstNode;

	//����table��¼index��Ӧ�Ŀ���
	initBlockAmountTable();

	m_createFlag = true;
	return m_createFlag;
}

void BuddyMem::BuddyMemDestroy()
{
	if(!m_createFlag)
	{
		return;
	}

	//�ͷ�������ָ��������ڴ�
	int i = 0;
	for(i=0;i<m_tableLinkListSize;i++)
	{
		deleteLinkList(m_usedTableLinkListArray[i]);
	}
	delete [] m_usedTableLinkListArray;
	m_usedTableLinkListArray = NULL;
	for(i=0;i<m_tableLinkListSize;i++)
	{
		deleteLinkList(m_emptyTableLinkListArray[i]);
	}
	delete [] m_emptyTableLinkListArray;
	m_emptyTableLinkListArray = NULL;

	finitBlockAmountTable();

	//�ͷ�ȫ���ڴ�
	delete [] m_startPos;
	m_startPos = NULL;

	m_createFlag = false;
}

char* BuddyMem::BuddyMemMalloc(const unsigned int size)
{
	unsigned int needBlockAmount = size/m_blockSize;
	if(size%m_blockSize!=0)
	{
		needBlockAmount++;
	}
	
	unsigned int tableIndex = getTableLinkListIndexFromBlockAmount(needBlockAmount);
	unsigned int actuallyBlockAmount = getBlockAmountFromIndex(tableIndex);

	char *pAlloc = NULL;
	int actuallyTableIndex = tableIndex;
	while(1)
	{
		if(actuallyTableIndex>=m_tableLinkListSize)
		{
			//���index����table����������NULL
			return NULL;
		}

		if(m_emptyTableLinkListArray[actuallyTableIndex]!=NULL)
		{
			//1��������ͷ�ڵ����ݵĵ�ַ��ֵ
			pAlloc = m_emptyTableLinkListArray[actuallyTableIndex]->address;
			unsigned int NowBlockAmount = getBlockAmountFromIndex(actuallyTableIndex);

			//2������½ڵ㵽used��
			LinkNode *usedNode = new LinkNode;
			usedNode->address = pAlloc;
			usedNode->next = NULL;
			insertNodeToTableLinkList(m_usedTableLinkListArray,tableIndex,usedNode);

			//3��������ͷ�ڵ㱣����ڴ�ռ��ֳ��µĽڵ㣬��ӵ�empty��
			int i = 0;
			char *pInsert = pAlloc + m_blockSize*getBlockAmountFromIndex(tableIndex);
			LinkNode *emptyNode = NULL;
			for(i=tableIndex;i<actuallyTableIndex;i++)
			{
				emptyNode = new LinkNode;
			    emptyNode->address = pInsert;
				emptyNode->next = NULL;
				insertNodeToTableLinkList(m_emptyTableLinkListArray,i,emptyNode);
				pInsert = pInsert + m_blockSize*getBlockAmountFromIndex(i);
			}

			//4��ɾ�����ͷ�ڵ㡣��4�������ܵ�3��Ӱ�죬��Ϊ�������Ǵ�
			deleteFirstNodeFromTableLinkList(m_emptyTableLinkListArray,actuallyTableIndex);

			return pAlloc;

		}
		actuallyTableIndex++;

	}

	return NULL;
}

unsigned int BuddyMem::getTableLinkListIndexFromBlockAmount(unsigned int blockAmount)
{
	int i = 0;
	for(i=0;i<m_tableLinkListSize;i++)
	{
		if(m_blockAmountTable[i]>=blockAmount)
		{
			break;
		}
	}
	return i;
}

unsigned int BuddyMem::getBlockAmountFromIndex(unsigned int index)
{
	return m_blockAmountTable[index];
}

bool BuddyMem::initBlockAmountTable(void)
{
	m_blockAmountTable = new unsigned int[m_tableLinkListSize];
	if(m_blockAmountTable==NULL)
	{
		return false;
	}
	unsigned int firstBlokcAmount = 1;
	for(int i=0;i<m_tableLinkListSize;i++)
	{
		m_blockAmountTable[i] = firstBlokcAmount<<i;
	}
	return true;
}

void BuddyMem::finitBlockAmountTable(void)
{
	if(m_blockAmountTable!=NULL)
	{
		delete [] m_blockAmountTable;
		m_blockAmountTable = NULL;
	}
}

void BuddyMem::CaculateTableLinkListSize()
{
	unsigned int maxBlock = m_maxMemorySize/m_blockSize;
	int i = 0;
	for(i=0;;i++)
	{
		if((maxBlock>>i)==1)
		{
			m_tableLinkListSize = i+1;
			break;
		}
	}
}

void BuddyMem::deleteLinkList(LinkNode* head)
{
	if(head==NULL)
	{
		return;
	}
	LinkNode *next = head->next;
	delete head;
	head = NULL;
	deleteLinkList(next);
}

bool BuddyMem::insertNodeToTableLinkList(LinkNode **table, unsigned int index, LinkNode *node)
{
	if(table[index]==NULL)
	{
		//������ǿյ�
		table[index] = node;
		return true;
	}

	//nodeCurr�Ǳ�Ȼ���ڵ�
	LinkNode *nodeCurr = table[index];
    LinkNode *nodePre = NULL;

	if((nodeCurr->address)>(node->address))
	{
		//���ڱ�ͷ
		node->next = nodeCurr;
		table[index] = node;
		return true;
	}

	nodePre = nodeCurr;
	nodeCurr = node->next;

	while(true)
	{
		//�����������β��
		if(nodeCurr==NULL)
		{
			nodePre->next = node;
			return true;
		}

		if((nodeCurr->address)>(node->address))
		{
			//�ҵ������
			node->next = nodeCurr;
			nodePre->next = node;
			return true;
		}

		nodePre = nodeCurr;
		nodeCurr = nodeCurr->next;
	}

	return false;
}

void BuddyMem::deleteFirstNodeFromTableLinkList(LinkNode **table, unsigned int index)
{
	if(table[index]==NULL)
	{
		//���������û��Ԫ��
		return;
	}
	LinkNode *secondNode = table[index]->next;
	delete table[index];
	table[index] = secondNode;
}

unsigned int BuddyMem::getActuallyAllocBlock(unsigned int needBlock)
{
	unsigned int count = 0;
	unsigned int pos = 0;
	unsigned int actuallyAllocBlock = needBlock;
	while(true)
	{
		if((needBlock&1)==1)
		{
			count++;
		}
		needBlock = needBlock>>1;
		pos++;
		if((needBlock|0)==0)
		{
			break;
		}
	}

	if(count>1)
	{
		actuallyAllocBlock = 1<<pos;
	}

	return actuallyAllocBlock;
}