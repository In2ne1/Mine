#pragma once
#include<vector>
#include<iostream>
using namespace std;

template <class T>
class HushBucketNode  //�ڵ�
{
	T m_val;  //ֵ
	HashBucketNode<T> * m_next;  //��ϣͰָ��

	HushBucketNode(const T& val = T()):  //����
		m_val(val),
		m_next(nullptr)
	{}

	template<class K, class V, class KeyofVal, class HF>
	friend class HashBucket;
	// K:�ؼ�������
	// V:��ͬ����V�����Ͳ�ͬ�������unordered_map��V����һ����ֵ�ԣ������unordered_set,V Ϊ K
	// KeyOfValue: ��ΪV�����Ͳ�ͬ��ͨ��valueȡkey�ķ�ʽ�Ͳ�ͬ����ϸ��unordered_map/set��ʵ��
	// HF: ��ϣ�����º����������ͣ���ϣ����ʹ�ó�������������Ҫ��Keyת��Ϊ�������ֲ���ȡģ
};

class dealInt
{
public:
	int operator()(int n)  //�º���
	{
		return n;
	}
};

template<class K, class V, class KeyofValue, class HF = dealInt>
class HashBucket  //��ϣ��
{
	vector<HashBucketNode<V>*> m_data;  //ָ�������
	size_t m_size;

	static long long s_m_primeTable[30];  //���ݹ����
	int m_primPos;  //�˿������������������е�λ��

public:
	HashBucket(size_t capacity = s_m_primeTable[0]) :  //��ʼ����capacity
		m_data(capacity, nullptr),
		m_size(0),
		m_primePos(0)
	{}

	class iterator  //���������ڲ��ࣩ
	{
	public:
		HashBucket<K, V, KeyofValue, HF>* m_hb;  //��ϣͰָ��
		HashBucketNode<V>* m_node;  //�ڵ�ָ��

		iterator(HashBucketNode<V>* node = nullptr,
			HashBucket<K, V, KeyofValue, HF>* hbpos = nullptr) :  //����
			m_node(node),
			m_hb(hbpos)
		{}

		iterator(const iterator& it) :  //��������
			m_node(it.m_node),
			m_hb(it.m_hb)
		{}

		V& operator*()  //���ؽ�����
		{
			return m_node->m_val;
		}

		V* operator->()  //���ؼ�ͷ
		{
			return &m_node->m_val;
		}

		iterator operator++()  //����ǰ��++
		{
			int val = m_node->m_val;
			m_node = m_node->m_next;  //������������һ����ֱ�Ӹ�ֵnext
			if (!m_node)  //������������next������Ͱ��++
			{
				int bucketno = m_hb->hashFunc(KeyofValue()(val)) + 1;  //����val�ĺ�һ��Ͱ��
				for (; buckteno < m_hb->capacity(); bucketno++)  //����һ��Ͱ��Ϊ�գ�����һ��Ͱ��
				{
					if (m_hb->m_data[bucketno])  //����Ͱ���ڲ�Ϊ��ʱ�������ҵ���һ���ڵ�
					{
						m_node = m_hb->m_data[bucketno];  //��Ͱ�ŵ�ͷָ��
						break;
					}
				}
			}
			return *this;
		}

		iterator operator++(int)
		{
			iterator<K, V, KeyofValue, HF> tmp = *this;  //���浱ǰλ��
			++(*this);  //����ǰ��++
			return tmp;  //��󷵻�++ǰ��tmp
		}

		//--������

		bool operator==(const iterator& data) const  //const�������ݲ���
		{
			return m_node == data.m_node && m_hb == data.m_hb;
		}

		bool operator!=(const iterator& data) const  //const�������ݲ���
		{
			return m_node != data.m_node || m_hb != data.m_hb;
		}
	};

private:
	int hashFunc(const K& key)  //����valֵ����val������Ͱ��
	{
		HF func;
		return func(key) % capacity();
	}

	void checkCapacity()  //����Ƿ���Ҫ����
	{
		if (m_size == capacity())  //��ʱ����
		{
			int mcapa = capacity();
			vector<HashBucketNode<V>*> tmp(s_m_primTable[++m_primePos], nullptr);  //tmpΪ���ݺ��Ͱ
			m_data.swap(tmp);  //�����ݰ�ԭ������tmp��
			m_size = 0;  //�轫�������¼��㲢���������ݺ��Ͱ�У�δ����֮ǰsize����Ϊ0

			int i;
			HashBucketNode<V>* cur;
			for (i = 0; i < mcapa; i++)  //�������
			{
				for (cur = tmp[i]; cur; cur = cur->next)  //�������
				{
					insert(cur->m_val);  //��insert���¼������ݲ����������ݺ��Ͱ��
				}
			}
		}
	}

public:
	iterator begin()
	{
		int bucketno = 0;
		for (; bucketno < capacity(); bucketno++)  //Ͱ�ű���
		{
			if (m_data[bucketno])  //����Ͱλ�ò�Ϊ�գ���Ϊ�ҵ���һ���ڵ�
			{
				return iterator(m_data[bucketno], this);  //���ص�����
			}
		}
		return iterator(nullptr, this);  //ͰΪ�գ�û�е�һ���ڵ�
	}

	iterator end()
	{
		return iterator(nullptr, this);
	}

	iterator insert(const V & val)  //����
	{
		checkCapacity();  //����Ƿ���Ҫ����

		int hashnum = hashFunc(KeyofValue()(val));  //�ù�ϣ���������Ӧ��keyֵ
		HashBucketNode<V> * tmp;

		if (m_data[hashnum])  //����keyֵ��Ͱ��Ϊ��
		{
			for (tmp = m_data[hashnum]; tmp; tmp = tmp->m_next)  //���Ͱ����û�и�Ԫ��
			{
				if (tmp->m_val == val)  //���������ʧ�ܲ�����
				{
					return end();
				}
			}
		}

		tmp = new HashBucketNode<V>(val);  //ΪҪ����Ԫ�ؿ��ٿռ�

		tmp->m_next = m_data[hashnum];  //ͷ��
		m_data[hashnum] = tmp;

		m_size++;
		return iterator(m_data[hashnum], this);
	}

	iterator erase(const V & val)  //ɾ��Ԫ��
	{
		int hashnum = hashFunc(KeyofValue()(val));  //�ù�ϣ���������Ӧ��keyֵ
		HashBucketNode<V> * tmp;

		if (!m_data[hashnum])  //�����keyֵ���ڵ�ͰΪ�գ���˵��û�и�Ԫ��
		{
			return end();
		}

		if (m_data[hashnum]->m_val == val)  //��ɾ����Ԫ����Ͱ�������ͷ�ڵ�
		{
			iterator res(m_data[hashnum], this);  //����res����ͷ�ڵ��Ľڵ�
			++res;

			tmp = m_data[hashnum];  //ͷɾ
			m_data[hashnum] = tmp->m_next;
			delete tmp;

			m_size--;  //ɾ��һ��Ԫ�غ� sizeҪ��һ
			return res;  //������ͷ
		}
		else  //��ɾ����Ԫ�ز���Ͱ�������ͷ�ڵ�
		{
			for (tmp = m_data[hashnum]; tmp->m_next; tmp = tmp->m_next)  //�������
			{
				if (tmp->m_next->m_val == val)  //���ҵ�Ҫɾ��Ԫ��ʱ����tmp��next
				{
					iterator res(tmp->m_next, this);  //����Ҫɾ��Ԫ�ص���һ���ڵ�
					++res;

					HashBucketNode<V> * cur;
					cur = tmp->m_next;  //��ɾ
					tmp->m_next = cur->m_next;
					delete cur;

					m_size--;
					return res;
				}
			}
			return end();
		}
	}

	iterator find(const V & val)  //����val�Ƿ���Ͱ��
	{
		int hashnum = hashFunc(KeyofValue()(val));  //�ù�ϣ���������Ӧ��keyֵ

		HashBucketNode<V> * cur;
		for (cur = m_data[hashnum]; cur; cur = cur->m_next)  //������keyֵ��Ͱ
		{
			if (cur->m_val == val)  //�ҵ��򷵻ص�����
			{
				return iterator(cur, this);
			}
		}
		return iterator(nullptr, this);  //û�ҵ�˵�������ڣ����ؿյ�����
	}

	void clear()  //���
	{
		HashBucketNode<V> * tmp;
		for (auto & head : m_data)
		{
			while (head)  //ͷɾ
			{
				tmp = head;
				head = head->m_next;
				delete tmp;
			}
		}
		m_size = 0;
	}

	size_t capacity()  //����
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

