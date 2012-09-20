
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

	//创建empty table和used table
	CaculateTableLinkListSize();
	m_emptyTableLinkListArray = new LinkNode*[m_tableLinkListSize];
	memset(m_emptyTableLinkListArray,0,m_tableLinkListSize*sizeof(LinkNode*));
	m_usedTableLinkListArray = new LinkNode*[m_tableLinkListSize];
	memset(m_usedTableLinkListArray,0,m_tableLinkListSize*sizeof(LinkNode*));
	
	LinkNode *firstNode = new LinkNode;
	firstNode->address = m_startPos;
	firstNode->next = NULL;
	m_emptyTableLinkListArray[m_tableLinkListSize-1] = firstNode;

	//申请table记录index对应的块数
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

	//释放两个表及指向链表的内存
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

	//释放全部内存
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
			//如果index大于table容量，返回NULL
			return NULL;
		}

		if(m_emptyTableLinkListArray[actuallyTableIndex]!=NULL)
		{
			//1、把链表头节点内容的地址赋值
			pAlloc = m_emptyTableLinkListArray[actuallyTableIndex]->address;
			unsigned int NowBlockAmount = getBlockAmountFromIndex(actuallyTableIndex);

			//2、添加新节点到used表
			LinkNode *usedNode = new LinkNode;
			usedNode->address = pAlloc;
			usedNode->next = NULL;
			insertNodeToTableLinkList(m_usedTableLinkListArray,tableIndex,usedNode);

			//3、从链表头节点保存的内存空间拆分成新的节点，添加到empty表
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

			//4、删除这个头节点。第4步不会受第3步影响，因为都比他们大
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
		//如果表是空的
		table[index] = node;
		return true;
	}

	//nodeCurr是必然存在的
	LinkNode *nodeCurr = table[index];
    LinkNode *nodePre = NULL;

	if((nodeCurr->address)>(node->address))
	{
		//插在表头
		node->next = nodeCurr;
		table[index] = node;
		return true;
	}

	nodePre = nodeCurr;
	nodeCurr = node->next;

	while(true)
	{
		//如果到了链表尾部
		if(nodeCurr==NULL)
		{
			nodePre->next = node;
			return true;
		}

		if((nodeCurr->address)>(node->address))
		{
			//找到插入点
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
		//如果该链表没有元素
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