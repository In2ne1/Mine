#pragma once
#include<vector>
#include<iostream>
using namespace std;

template <class T>
class HushBucketNode  //节点
{
	T m_val;  //值
	HashBucketNode<T> * m_next;  //哈希桶指针

	HushBucketNode(const T& val = T()):  //构造
		m_val(val),
		m_next(nullptr)
	{}

	template<class K, class V, class KeyofVal, class HF>
	friend class HashBucket;
	// K:关键码类型
	// V:不同容器V的类型不同，如果是unordered_map，V代表一个键值对，如果是unordered_set,V 为 K
	// KeyOfValue: 因为V的类型不同，通过value取key的方式就不同，详细见unordered_map/set的实现
	// HF: 哈希函数仿函数对象类型，哈希函数使用除留余数法，需要将Key转换为整形数字才能取模
};

class dealInt
{
public:
	int operator()(int n)  //仿函数
	{
		return n;
	}
};

template<class K, class V, class KeyofValue, class HF = dealInt>
class HashBucket  //哈希表
{
	vector<HashBucketNode<V>*> m_data;  //指针数组表
	size_t m_size;

	static long long s_m_primeTable[30];  //扩容规则表
	int m_primPos;  //此刻容量所处于素数表中的位置

public:
	HashBucket(size_t capacity = s_m_primeTable[0]) :  //初始容量capacity
		m_data(capacity, nullptr),
		m_size(0),
		m_primePos(0)
	{}

	class iterator  //迭代器（内部类）
	{
	public:
		HashBucket<K, V, KeyofValue, HF>* m_hb;  //哈希桶指针
		HashBucketNode<V>* m_node;  //节点指针

		iterator(HashBucketNode<V>* node = nullptr,
			HashBucket<K, V, KeyofValue, HF>* hbpos = nullptr) :  //构造
			m_node(node),
			m_hb(hbpos)
		{}

		iterator(const iterator& it) :  //拷贝构造
			m_node(it.m_node),
			m_hb(it.m_hb)
		{}

		V& operator*()  //重载解引用
		{
			return m_node->m_val;
		}

		V* operator->()  //重载箭头
		{
			return &m_node->m_val;
		}

		iterator operator++()  //重载前置++
		{
			int val = m_node->m_val;
			m_node = m_node->m_next;  //若该链存在下一个则直接赋值next
			if (!m_node)  //若该链不存在next，则还需桶号++
			{
				int bucketno = m_hb->hashFunc(KeyofValue()(val)) + 1;  //计算val的后一个桶号
				for (; buckteno < m_hb->capacity(); bucketno++)  //若下一个桶内为空，则看下一个桶号
				{
					if (m_hb->m_data[bucketno])  //当该桶号内不为空时，则算找到下一个节点
					{
						m_node = m_hb->m_data[bucketno];  //该桶号的头指针
						break;
					}
				}
			}
			return *this;
		}

		iterator operator++(int)
		{
			iterator<K, V, KeyofValue, HF> tmp = *this;  //保存当前位置
			++(*this);  //调用前置++
			return tmp;  //最后返回++前的tmp
		}

		//--的重载

		bool operator==(const iterator& data) const  //const保持数据不变
		{
			return m_node == data.m_node && m_hb == data.m_hb;
		}

		bool operator!=(const iterator& data) const  //const保持数据不变
		{
			return m_node != data.m_node || m_hb != data.m_hb;
		}
	};

private:
	int hashFunc(const K& key)  //根据val值计算val所处的桶号
	{
		HF func;
		return func(key) % capacity();
	}

	void checkCapacity()  //检查是否需要扩容
	{
		if (m_size == capacity())  //此时满了
		{
			int mcapa = capacity();
			vector<HashBucketNode<V>*> tmp(s_m_primTable[++m_primePos], nullptr);  //tmp为扩容后的桶
			m_data.swap(tmp);  //将数据按原样放入tmp中
			m_size = 0;  //需将数据重新计算并插入新扩容后的桶中，未插入之前size先置为0

			int i;
			HashBucketNode<V>* cur;
			for (i = 0; i < mcapa; i++)  //数组遍历
			{
				for (cur = tmp[i]; cur; cur = cur->next)  //链表遍历
				{
					insert(cur->m_val);  //用insert重新计算数据并插入新扩容后的桶中
				}
			}
		}
	}

public:
	iterator begin()
	{
		int bucketno = 0;
		for (; bucketno < capacity(); bucketno++)  //桶号遍历
		{
			if (m_data[bucketno])  //若该桶位置不为空，即为找到第一个节点
			{
				return iterator(m_data[bucketno], this);  //返回迭代器
			}
		}
		return iterator(nullptr, this);  //桶为空，没有第一个节点
	}

	iterator end()
	{
		return iterator(nullptr, this);
	}

	iterator insert(const V & val)  //插入
	{
		checkCapacity();  //检查是否需要扩容

		int hashnum = hashFunc(KeyofValue()(val));  //用哈希函数算出对应的key值
		HashBucketNode<V> * tmp;

		if (m_data[hashnum])  //若该key值的桶不为空
		{
			for (tmp = m_data[hashnum]; tmp; tmp = tmp->m_next)  //检查桶内有没有该元素
			{
				if (tmp->m_val == val)  //若有则插入失败并返回
				{
					return end();
				}
			}
		}

		tmp = new HashBucketNode<V>(val);  //为要插入元素开辟空间

		tmp->m_next = m_data[hashnum];  //头插
		m_data[hashnum] = tmp;

		m_size++;
		return iterator(m_data[hashnum], this);
	}

	iterator erase(const V & val)  //删除元素
	{
		int hashnum = hashFunc(KeyofValue()(val));  //用哈希函数算出对应的key值
		HashBucketNode<V> * tmp;

		if (!m_data[hashnum])  //如果该key值所在的桶为空，则说明没有该元素
		{
			return end();
		}

		if (m_data[hashnum]->m_val == val)  //若删除的元素是桶内链表的头节点
		{
			iterator res(m_data[hashnum], this);  //先用res保存头节点后的节点
			++res;

			tmp = m_data[hashnum];  //头删
			m_data[hashnum] = tmp->m_next;
			delete tmp;

			m_size--;  //删除一个元素后 size要减一
			return res;  //返回新头
		}
		else  //若删除的元素补是桶内链表的头节点
		{
			for (tmp = m_data[hashnum]; tmp->m_next; tmp = tmp->m_next)  //链表遍历
			{
				if (tmp->m_next->m_val == val)  //当找到要删除元素时，即tmp的next
				{
					iterator res(tmp->m_next, this);  //保存要删除元素的下一个节点
					++res;

					HashBucketNode<V> * cur;
					cur = tmp->m_next;  //后删
					tmp->m_next = cur->m_next;
					delete cur;

					m_size--;
					return res;
				}
			}
			return end();
		}
	}

	iterator find(const V & val)  //查找val是否在桶中
	{
		int hashnum = hashFunc(KeyofValue()(val));  //用哈希函数算出对应的key值

		HashBucketNode<V> * cur;
		for (cur = m_data[hashnum]; cur; cur = cur->m_next)  //遍历该key值的桶
		{
			if (cur->m_val == val)  //找到则返回迭代器
			{
				return iterator(cur, this);
			}
		}
		return iterator(nullptr, this);  //没找到说明不存在，返回空迭代器
	}

	void clear()  //清空
	{
		HashBucketNode<V> * tmp;
		for (auto & head : m_data)
		{
			while (head)  //头删
			{
				tmp = head;
				head = head->m_next;
				delete tmp;
			}
		}
		m_size = 0;
	}

	size_t capacity()  //容量
	{
		return s_m_primeTable[m_primePos];
	}
};

template<class K, class V, class KeyofValue, class HF>
long long HashBucket<K, V, KeyofValue, HF>::s_m_primeTable[30] = {
	11, 23, 47, 89, 179,
	353, 709, 1409, 2819, 5639,
	11273, 22531, 45061, 90121, 180233,
	360457, 720899, 1441807, 2883593, 5767169,
	11534351, 23068673, 46137359, 92274737, 184549429,
	369098771, 738197549, 1476395029, 2952790016u, 4294967291u
};

