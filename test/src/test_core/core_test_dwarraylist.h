
//////////////////////////////////////////////////////////////////////////
// DwArrayList
//////////////////////////////////////////////////////////////////////////

//������֤�������ʱ�Ƿ�����ȷִ�й�������������
namespace ArrayListTest
{
	class ObjTest
	{
	public:
		ObjTest() : ptr(NULL)
		{

		}

		ObjTest(const ObjTest& other)
		{
			++i;
		}

		~ObjTest()
		{
			i--;
		}

		static int i;
		void* ptr;		//ռλ����������֤�������С������һ��ָ��ʱ�������Ƿ�ִ��placement new
	};	
}

int ArrayListTest::ObjTest::i = 0;

TEST(DwArrayList, DwArrayList)
{
	const int KCount = 100;
	//int
	DwArrayList<int> list1;
	for(int i = KCount; i >= 1; --i)
	{
		list1.prepend(i);
	}
	DwArrayList<int> list2(list1);	
	EXPECT_EQ((1 + 100) * 100 / 2, sumOfContainer(list2));
	EXPECT_EQ(1, list2.front());
	list2.pop_front();
	EXPECT_EQ(2, list2.front());
	EXPECT_EQ(100, list2.back());
	list2.pop_back();
	EXPECT_EQ(99, list2.back());
	EXPECT_EQ(KCount - 2, list2.count());
	EXPECT_EQ(100 * 101 / 2 - 1 - 100, sumOfContainer(list2));
	EXPECT_EQ((1 + 100) * 100 / 2, sumOfContainer(list1));
	//ObjTest
	DwArrayList<ArrayListTest::ObjTest> objlist;
	ArrayListTest::ObjTest mai;
	for(int i = 1; i <= KCount; ++i)
	{
		objlist.push_back(mai);
	}
	EXPECT_EQ(100, ArrayListTest::ObjTest::i);
	for(int i = 0; i < KCount; ++i)
	{
		objlist.pop_front();
	}
	EXPECT_EQ(0, ArrayListTest::ObjTest::i);
}

TEST(DwArrayList, headTailOperation)
{
	const int KCount = 100;
	DwArrayList<int> list;
	EXPECT_TRUE(list.isEmpty());
	EXPECT_EQ(0,list.count());
	DwArrayList<int> intListIter;
	EXPECT_TRUE(list.begin() == list.end());
	for (int i = 1; i < KCount / 2; ++i)
	{
		list.push_back(i);
	}
	for (int i = KCount / 2; i <= KCount; ++i)
	{
		list.append(i);
	}
	int i=1;
	for (DwArrayList<int>::const_iterator it = list.begin(); it != list.end(); ++it)
	{
		EXPECT_EQ(i, *it);
		++i;
	}
	for(int i = KCount ; i > KCount/2; --i)
	{
		list.push_front(i);
	}
	for(int i = KCount / 2; i >=1 ; --i)
	{
		list.prepend(i);
	}
	EXPECT_EQ(2 * KCount, list.count());
	int sum=0;
	for (unsigned int i = 0; i < list.count(); i++)
	{
		sum += list[i];
	}
	EXPECT_EQ(5050 * 2, sum);
	for(int i = 1; i <= KCount / 2; ++i)
	{
		list.pop_front();
	}
	EXPECT_EQ(51, list.front());
	for(int i = 1; i <= KCount / 2; ++i)
	{
		list.pop_back();
	}
	EXPECT_EQ(50, list.back());
}

TEST(DwArrayList, insert)
{
	const int KCount = 511 * 3 - 2;
	DwArrayList<int> list;
	for (int i = 1; i <= KCount; i++)
	{
		list.push_back(i);
	}
	//������λ���ϲ���1��9
	list.insert(1530, 1);	//�����һ��Ԫ��֮ǰ����1
	EXPECT_EQ(1, list[1530]);
	list.insert(1520, 2);	//����������
	list.remove(513);		//ֵΪ514
	list.insert(1021, 3);	//�ҵ���������㣬������Ӧ�ڵڶ������β������(ǰ����ѭ�����г���Ϊ511)
	list.insert(515, 4);	//Ϊ���Ӵ��븲���ʶ����õĲ���λ��
	list.insert(1020, 5);	//ɾ�����һ��Ԫ��
	list.removeOne(1530);
	list.insert(0, 6);
	list.insert(12, 7);
	list.insert(1537, 8);	//�����ڶ���
	list.insert(1539, 9);	//���һ��֮��
	EXPECT_EQ(9, list[1537]);
	EXPECT_EQ(511 * 3 - 2 + 9 - 2, list.count());
	EXPECT_EQ((1 + (511 * 3 - 2)) * (511 * 3 - 2) / 2 + 1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 - 514 - 1530, sumOfContainer(list));
	DwArrayList<int>::iterator it;
	it = list.insert(it, 10);//ʹ��δ��ʼ������Ч������ʱ���صĵ�����������Ϊ-1
	EXPECT_EQ(-1, it.index());
	list.pop_front();		//��Ϊ���Ӵ��븲���ʣ�ɾ��ǰ��һ�����ֻ��һ��Ԫ��
	list.erase(list.end() - 1);//��Ϊ���Ӵ��븲���ʣ�ɾ��ǰ���һ�����ֻ��һ��Ԫ��
	EXPECT_EQ(511, list[511]);	//����searchAfter���븲����
	EXPECT_EQ(3, list[1535 - 511]);	//����searchBefore���븲����
	for(int i = 0; i < 511 - 256; ++i)	//��Ϊ����halveNode���븲���ʣ�����ǰ��һ�������256��Ԫ��
	{
		list.prepend(0);
	}
	list.insert(1, 10);
	EXPECT_EQ(10, list[1]);
	list.insert(1790, 11);
	EXPECT_EQ(11, list[1790]);
	for (int i = 0; i < 256; ++i)
	{
		list.insert(list.count() - 260, 0);	//��Ϊ����halveNode���븲���ʣ��ڵ����ڶ�������ϲ���
	}
	EXPECT_EQ(8, list[list.count() - 1]);
}

TEST(DwArrayList, remove)
{
	const int KCount = 511 * 3;				//DequeList��һ����㣨ѭ�����У��ĳ���Ϊ511������3����������
	DwArrayList<int> intList;
	for (int i = 1; i <= KCount; i++)
	{
		intList << i;
	}
	DwArrayList<int>::iterator intIt;
	for (int i = 1; i <= 511; i++)			//ɾ���м��������Ԫ�أ��м���ĵ�һ��Ԫ��Ϊ512
	{
		intIt = intList.erase(intList.begin() + 511);
		EXPECT_EQ(512 + i, *intIt);
	}

	DwArrayList<std::string> strList;
	strList.push_back("Duo");
	strList.push_back("Wan");
	strList.push_back("-");
	strList.push_back("Duo");
	strList.push_back("YY");
	strList.push_back("-");
	strList.push_back("China");
	strList.push_back("Duo");				//strList = DuoWan-DuoYY-ChinaDuo
	EXPECT_EQ(8, strList.count());
	EXPECT_EQ(3, strList.removeAll("Duo"));	//strList = Wan-YY-China
	EXPECT_EQ(0, strList.removeAll("Duo"));
	strList.push_back("Duo");
	strList.push_front("Duo");				//strList = DuoWan-YY-ChinaDuo
	std::string str;
	for (DwArrayList<std::string>::const_iterator it = strList.begin(); it != strList.begin() + 2; ++it)
	{
		str += *it;
	}
	EXPECT_STREQ("DuoWan", str.c_str());
	str.clear();
	DwArrayList<std::string>::iterator newIt = strList.erase(strList.begin() + 2);	//ɾ����-���󷵻����¸�Ԫ�ء�YY���ĵ�����
	EXPECT_EQ("YY", *newIt);
	EXPECT_EQ(6, strList.count());			//strList = DuoWanYY-ChinaDuo
	for (DwArrayList<std::string>::const_iterator it = strList.begin(); it != strList.end() - 3; ++it)
	{
		str += *it;
	}
	EXPECT_STREQ("DuoWanYY",str.c_str());
	newIt = strList.erase(strList.createIterator(strList.indexOf("Duo")));
	EXPECT_EQ("Wan", *newIt);
	EXPECT_EQ(5, strList.count());			//strList = WanYY-ChinaDuo
	str.clear();
	for (DwArrayList<std::string>::const_iterator it = strList.begin(); it != strList.end(); ++it)
	{
		str += *it;
	}
	EXPECT_EQ(5, strList.count());
	EXPECT_STREQ("WanYY-ChinaDuo", str.c_str());
	str.clear();
	EXPECT_TRUE(strList.removeOne("Duo"));	//strList = WanYY-China
	EXPECT_EQ(4, strList.count());
	for (DwArrayList<std::string>::const_iterator it = strList.begin() + 1; it != strList.end(); ++it)
	{
		str += *it;
	}
	EXPECT_STREQ("YY-China", str.c_str());
	EXPECT_EQ(0, strList.removeAll("Duo"));	//strList = WanYY-China
	DwArrayList<std::string>::const_iterator it = strList.createIterator(6);	//������Χ��ָ��end()
	EXPECT_TRUE(strList.end() == it);
	DwArrayList<std::string>::iterator eraseIt = strList.erase(strList.begin(), strList.end() - 1);
	EXPECT_TRUE(strList.begin() == eraseIt);
	EXPECT_EQ(1, strList.count());
	strList.pop_back();						//��pop_backɾ�����һ��Ԫ��
	EXPECT_TRUE(strList.isEmpty());
	EXPECT_EQ(0, strList.count());
}

TEST(DwArrayList, iterator)
{
	const int KCount = 100;
	DwArrayList<int> list;
	for (int i = 1; i <= KCount; i++)
	{
		list.push_back(i);
	}
	DwArrayList<int>::iterator it1;							//����Ĭ�Ϲ��캯��
	it1 = list.begin() + 10;								//����operator+(int n)��ָ���11��Ԫ�أ�����10
	EXPECT_EQ(10, it1.index());	
	//end������Ϊ100��ָ���91��Ԫ�أ�����90
	DwArrayList<int>::iterator it2 = list.end() - 10;		//���Ը��ƹ��캯����operator-(int n)
	EXPECT_EQ(90, it2.index());								//����index
	EXPECT_TRUE(it1 != it2);								//����operator!=
	EXPECT_TRUE(it1 < it2);									//����operator<
	EXPECT_TRUE(it1 <= it2);								//����operator<=
	EXPECT_EQ(80, it2 - it1);								//����operator-(const Iterator&)
	EXPECT_EQ(11, *it1);									//����operator*
	DwArrayList<int>::iterator it3 = it1;
	EXPECT_TRUE(it1 == it3);								//����operator==
	EXPECT_TRUE(it3 >= it1);								//����operator>=
	it3++;													//����operator++(int)
	EXPECT_TRUE(it3 > it1);									//����operator>
	EXPECT_TRUE(it3 == ++it1);								//����operator++()
	it1--;
	--it3;
	EXPECT_TRUE(it1 == it3);								//����operator--(int)��operator--()
	it1 += 80;
	EXPECT_TRUE(it2 == it1);								//����operator+=
	//*it2 == 91
	EXPECT_TRUE(*it2 == it3[80]);							//����operator[]
	DwArrayList<int>::iterator it4 = list.begin() + 0;		//�߽����
	EXPECT_EQ(0, it4.index());
	it4 = list.begin() + 100;								//�պ�Խ��
	EXPECT_EQ(100, it4.index());							//ָ��end��idΪ100
	it4 = list.begin() + 200;								//Խ��
	EXPECT_EQ(100, it4.index());							//ָ��end��idΪ100
	EXPECT_TRUE(++it4 == list.end());
	it4 = list.begin() - 200;								//Խ��
	EXPECT_EQ(0, it4.index());								//ָ��begin��idΪ0	
	EXPECT_TRUE(--it4 == list.begin());
	DwArrayList<int>::iterator it5 = list.createIterator(15);
	EXPECT_TRUE(it5 - 1 == list.begin() + 14);
	list.clear();
	DwArrayList<int>::iterator it6 = list.begin();			//��Ϊ���Ӵ��븲����
	EXPECT_EQ(-1, it6.index());
	it6 = list.end();
	EXPECT_EQ(-1, it6.index());
	DwArrayList<int>::const_iterator it7 = list.begin();
	EXPECT_EQ(-1, it7.index());
	it7 = list.end();
	EXPECT_EQ(-1, it7.index());
	DwArrayList<int>::iterator it8 = list.createIterator(-1);
	EXPECT_EQ(-1, it8.index());
}

TEST(DwArrayList, sort)
{
	const int KCount = 1000;
	DwArrayList<int> list;
	for (int i = 0; i < KCount; i++)
	{
		list.push_back(dwRand() % KCount);
	}
	int sum = sumOfContainer(list);
	std::sort(list.begin(), list.end());
	EXPECT_EQ(sum, sumOfContainer(list));
	for (unsigned int i = 1; i < list.count(); i++)
	{
		EXPECT_TRUE(list[i] >= list[i - 1]);
	}
}
