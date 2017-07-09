#pragma once
/*********************************************************************************
Description: 以下是实现哈希算法的一个例子，KEY是字符串，所以提供了BKDRhash函数计算
             哈希键值，处理大数据量时用哈希能实现快速查找等。另外stl库中有hash_map
             类和 map类类似。本例使用18身份证号作为键值，作为快速查找身份证使用，
			 其他类似以字符串为键值的hash表可以参考此程序。
Author: LGY
Date:   2015/09/25
**********************************************************************************/

//哈希表长度，依据数据量的大小设置合理的长度
const int HashTableLen=100000;


struct HashNode 
{
	HashNode()
	{
		used=false;
		next=NULL;
	}
	HashNode(char* STR)
	{
		strcpy_s(str, STR);
	}
	bool           used; //当前空间是否被占用
	char           str[19];
	HashNode*      next;
};

//关键字重复的冲突处理是采用链表方式
class CHashSearchOperator
{
public:
	CHashSearchOperator(void);
	~CHashSearchOperator(void);
private:
	//哈希表中有效数据的长度
	int        m_nHashTableVaildLen;
	HashNode   m_HashTable[HashTableLen];

	//获取哈希函数的值(BKDRHash算法)
	int       GetBKDRHash( const char* str);
	//清理hashtable,采用链表方式，不适用此hashtable时要调用此方法
	void      ClearTable();

public:
	//重置hashtable中的节点为可用状态
	void      ResetHashTble();
	//输入一个按行存储的文件，加载文件中的内容到hash表中
	int       EstablishHashTable( const TCHAR * Filename);
	//查找hashtable中的是否存在此字符串
	bool      FindStr(const char*  str);

	//增
	bool      InsertData(HashNode* pNode);
	//删
	bool      DeleteData(const char* strKey);
	//改
	bool      UpdateData(const char* strKey,HashNode* pNode);
	//查
	HashNode* FindData(const char* strKey);

	inline int GetHashTableVaildLen(){return m_nHashTableVaildLen;}
};