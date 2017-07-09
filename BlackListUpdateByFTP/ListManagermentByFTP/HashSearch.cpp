#include "stdafx.h"
#include "HashSearch.h"

CHashSearchOperator::CHashSearchOperator(void)
: m_nHashTableVaildLen(0)
{

}

CHashSearchOperator::~CHashSearchOperator(void)
{
	ClearTable();
}


int CHashSearchOperator::GetBKDRHash(const char *str)
{
	//BKDRhash函数
	unsigned int seed = 131;       //可取值 1313   13131   131313  等
	unsigned int hash = 0;
	while(*str != '\n' &&  *str != 0)
		hash = hash * seed + (*str++);

	return hash % HashTableLen;
}


void CHashSearchOperator::ResetHashTble()
{
	for(int i= 0; i< HashTableLen; i++)
	{
		HashNode *p = &m_HashTable[i];
		m_HashTable[i].used= false;
		while (p->next != NULL)
		{
			p= p->next;
			p->used= false;
		}
	}
	return;
}


void CHashSearchOperator::ClearTable()
{
	for(int i= 0; i< HashTableLen; i++)
	{
		HashNode *p = &m_HashTable[i];
		p= p->next;
		while (p != NULL)
		{
			HashNode *q = p;          //赋值给q
			p = p->next;
			delete  q;
			q = NULL;
		}
	}
	return;
}

int  CHashSearchOperator::EstablishHashTable(const TCHAR * chFilename)
{
	HashNode  *phashNode;
	int key;
	string  Linestr;
	ifstream ifs(chFilename,ios_base::binary|ios_base::in);
	//ifs.seekg(3,ios::beg);                // utf8文件空出前3个字节
	while( ifs>>Linestr)
	{
		if (Linestr.length() != 18)
		{
			continue;
		}
		key=GetBKDRHash(Linestr.c_str());
		if(m_HashTable[key].used==false)
		{
			strcpy_s(m_HashTable[key].str, Linestr.c_str());
			m_HashTable[key].used=true;
			m_nHashTableVaildLen++;  
		}
		else//冲突处理
		{
			phashNode=&m_HashTable[key];
			while(phashNode->next!=NULL)
			{
				if(phashNode->next->used == true)
					phashNode = phashNode->next;
				else
				{
					phashNode->next->used=true;
					strcpy_s(phashNode->next->str, Linestr.c_str());
					m_nHashTableVaildLen++;
					break;
				}
			}
			if(phashNode->next == NULL )
			{
				phashNode->next = new HashNode();
				phashNode->next->used=true;
				strcpy_s(phashNode->next->str, Linestr.c_str());
				m_nHashTableVaildLen++;
			}
		}
	}
	return m_nHashTableVaildLen;        // 返回读入的数据个数
}

bool CHashSearchOperator::FindStr(const char*  str)
{
	HashNode * phashNode;
	int key = 0;

	key=GetBKDRHash(str);
	if(m_HashTable[key].used==false)
	{
		return false;
	}
	else
	{
		phashNode = &m_HashTable[key];
		while(phashNode != NULL)
		{
			if(!strcmp(phashNode->str,str)) 
			{
				return true;
			}
			else
			{
				phashNode =phashNode->next;
			}
			if(phashNode == NULL || phashNode->used == false) 
				return false;
		}
	}
	return false;
}

//增
bool CHashSearchOperator::InsertData(HashNode* pNode)
{
	HashNode * phashNode;
	int key = GetBKDRHash(pNode->str);

	phashNode=&m_HashTable[key];
	if(phashNode->used == false)
	{
		phashNode->used=true;
		strcpy_s(phashNode->str, pNode->str);
		m_nHashTableVaildLen ++;
		return true;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true)
			phashNode = phashNode->next;
		else
		{
			phashNode->next->used=true;
			strcpy_s(phashNode->next->str, pNode->str);
			m_nHashTableVaildLen++;
			return true;
		}
	}
	if(phashNode->next == NULL )
	{
		phashNode->next=new HashNode();
		phashNode->next->used=true;
		strcpy_s(phashNode->next->str, pNode->str);
		m_nHashTableVaildLen++;
		return true;
	}
	return false;
}

//删
bool CHashSearchOperator::DeleteData(const char* strKey)
{
	HashNode * phashNode;
	int key = GetBKDRHash(strKey);

	phashNode=&m_HashTable[key];
	
	if(phashNode->used == true && strcmp(phashNode->str,strKey)==0)
	{
		phashNode->used=false;
		m_nHashTableVaildLen --;
		return true;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true && strcmp(phashNode->next->str,strKey)==0)
		{
			phashNode->next->used = false;
			m_nHashTableVaildLen --;
			return true;
		}
		else
		{
			phashNode = phashNode->next;
		}
	}

	if(phashNode->next == NULL )
	{
		return false;
	}
	return false;
}

//改
bool CHashSearchOperator::UpdateData(const char* strKey,HashNode* pNode)
{
	//此例中没有身份的其他数据，故没有具体实现改
	HashNode * phashNode;
	int key = GetBKDRHash(strKey);

	phashNode=&m_HashTable[key];

	if(phashNode->used == true && strcmp(phashNode->str,strKey)==0)
	{
		//修改其他数据
		return true;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true && strcmp(phashNode->next->str,strKey)==0)
		{
			//修改其他数据
			return true;
		}
		else
		{
			phashNode = phashNode->next;
		}
	}

	if(phashNode->next == NULL )
	{
		return false;
	}
	return false;
}

//查
HashNode* CHashSearchOperator::FindData(const char* strKey)
{
	HashNode * phashNode;
	int key = GetBKDRHash(strKey);

	phashNode=&m_HashTable[key];

	if(phashNode->used == true && strcmp(phashNode->str,strKey)==0)
	{
		return phashNode;
	}

	while(phashNode->next!=NULL)
	{
		if(phashNode->next->used == true && strcmp(phashNode->next->str,strKey)==0)
		{
			return phashNode->next;
		}
		else
		{
			phashNode = phashNode->next;
		}
	}

	if(phashNode->next == NULL )
	{
		return NULL;
	}
	return NULL;
}