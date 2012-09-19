
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
	
	unsigned int tableIndex = getTableLinkListIndex(needBlockAmount);


	return NULL;
}

unsigned int BuddyMem::getTableLinkListIndex(unsigned int blockAmount)
{
	int i = 0;
	for(i=1;;i++)
	{
		if((blockAmount>>1)==0)
		{
			break;
		}
	}
	return i;
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