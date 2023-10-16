#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

class node
{
friend class trie;
private:
	bool tag;
	union type {
		int idx;
		char c;
	}type;
	int ptrnum;
	node** next;
public:
	node() {};
	node(bool x, int idx);
	node(bool x, char c);
	node(bool x, int idx, int ptrnum);
	~node()
	{
		delete next;
	}
	void extensing()
	{
		ptrnum++;
		node** newNode = new node*[ptrnum];
		for (int i = 0; i < ptrnum - 1; i++)
			newNode[i] = next[i];
		delete next;
		next = newNode;
	}

};


class trie
{
private:
	node* start;
	int max_state;
	vector<node*> idx;
	int* ff;
	int pat_num;
public:
	trie():max_state(0),pat_num(0) { start = new node(true, (int)0); }
	~trie();
	bool isNull() { return start->next[0] == NULL; }
	void push(string& str);

	void show() const;
	void initindex(); //idx�迭�� ���� �ٷ� ������Ʈ�� �� �� �ִ�.
	void idxshow();
	void initff();  //failure function
	void ffshow() const;
	void match(string& str);
};





int main(void)
{
	//Ʈ���̻���
	cout << "Ʈ���� ����" << endl;
	trie tr;
	//���� Ʈ���̿� �ֱ�
	cout << "���� �ֱ�" << endl;
	string pat = "aaa";
	tr.push(pat);
	string pat2 = "abaab";
	tr.push(pat2);
	string pat3 = "ababaa";
	tr.push(pat3);
	//���� ���� 
	cout << "������ �� Ʈ���� Ȯ��(������ ���ڴ� max_state," <<'\n' << " ���ڵι��� �б����� ����" << endl;
	tr.show();
	//�ε��� �迭 ���� �� Ȯ��
	cout << "�ε��� �迭" << endl;
	tr.initindex();
	tr.idxshow();
	cout << "FailureFunction" << endl;
	tr.initff();
	tr.ffshow();
	cout << "match" << endl;
	string str = "aaaabcdababaaababaab";
	tr.match(str);

	return 0;
}




//node
node::node(bool x, int index) :tag(x), ptrnum(1)
{
	type.idx = index;
	next = new node*[1];
	next[0] = NULL;
}

node::node(bool x, int index, int ptrnum) :tag(x), ptrnum(ptrnum)
{
	type.idx = index;
	next = new node*[ptrnum];
	next[0] = new node[ptrnum];

}
node::node(bool x, char data) :tag(x), ptrnum(1)
{
	type.c = data;
	next = new node*[1];
	next[0] = NULL;
}


//trie
trie::~trie()
{
	stack<node*> del;
	queue<node*> frag;
	node* ptr = start;
	//�ϴ� ���ڷ� �����鼭 ���ÿ� ����, �� �бⰡ�ִ� ����� ��� ť�� �б�鸸 ����
	for (ptr; ptr != NULL; ptr = ptr->next[0])
	{
		if (ptr->ptrnum == 1)
		{
			del.push(ptr);
		}
		else
		{
			del.push(ptr);
			for (int i = 1; i < ptr->ptrnum; i++)
			{
				frag.push(ptr->next[i]);
			}
		}
	}
	//ť�� �ִ� �б�鵵 ���ÿ� ����
	while(!frag.empty())
	{
		ptr = frag.front();
		frag.pop();
		for (ptr; ptr != NULL; ptr = ptr->next[0])
		{
			if (ptr->ptrnum == 1)
				del.push(ptr);
			else
			{
				del.push(ptr);
				for (int i = 1; i < ptr->ptrnum; i++)
					frag.push(ptr->next[i]);
			}
		}
	}

	//���ÿ� ���� ������ ������ �����鼭 �޸� ����, �������� ��ŸƮ ��嵵 �޸� ������(ó���� �־)
	while (!del.empty())
	{
		delete del.top();
		del.pop();
	}

	delete ff;
}

void trie::push(string& str)
{
	if (isNull())
	{
		int i;
		node** temp = &start->next[0];
		for (i = 0; i < str.length(); i++)
		{
			node* nwD = new node(false, str[i]);
			node* nwS = new node(true, i + 1);
			nwD->next[0] = nwS;
			*temp = nwD;
			temp = &nwS->next[0];
		}
		max_state = i;
		pat_num++;
		return;
	}
	
	node* ptr = start;
	node* temp = start->next[0];
	int i = 0;
	int k = 0;//next�� ����Ű������ ����
	while (i < str.length())
	{
		if (ptr->tag == true) //state�̹Ƿ� �ǳʶٴµ�, ���� ���ڰ� �ٸ����� ����� �ּ� ����
		{
			temp = &(*ptr);
			ptr = ptr->next[k];
		}
		else {
			if (ptr->type.c == str[i]) //���ڰ� ������
			{
				k = 0;
				ptr = ptr->next[0]; //�������ڸ� �˻��Ѵ�.
				i++;
			}
			else //���ڰ� �ٸ���
			{
				//���� �������� �ڽĳ�尡 ����� Ȯ��
				if ((temp)->ptrnum == 1)
					break;
				else
				{
					if(k < ((temp)->ptrnum) - 1)
					{
						k++;
						ptr = temp;
					}
					else {
						k = 0;
						break;
					}
				}
			}
		}
	}


	if (i != str.length())
	{
		int cap = (temp)->ptrnum;
		int capacity = cap + 1;
		(temp)->extensing();

		node** temp2 = &(temp)->next[cap];

		node* nwD = new node(false, str[i]);
		max_state++;
		node* nwS = new node(true, max_state);
		nwD->next[0] = nwS;
		temp->next[cap] = nwD;
		temp = nwS;
		i++;
		for (int j = i; j < str.length(); j++)
		{
			node* nwD = new node(false, str[j]);
			max_state++;
			node* nwS = new node(true, max_state);
			nwD->next[0] = nwS;
			temp->next[0] = nwD;
			temp = nwS;
		}
		pat_num++;
		return;
	}
	
}

void trie::show() const
{
	queue<node*> div[10];
	for (node* ptr = start; ptr != NULL; ptr = ptr->next[0])
	{
		if (ptr->tag)
		{
			cout << ptr->type.idx << " ";
			if (ptr->ptrnum > 1)
			{
				for (int i = 1; i < ptr->ptrnum; i++)
				{
					cout << ptr->type.idx << ' ';
					div->push((ptr->next[i]));
				}
			}
		}
		else
		{
			cout << ptr->type.c << " ";
		}
	}
	cout << endl;
	while (!div->empty())
	{
		node* ptr = (div->front());
		div->pop();
		
		for (ptr; ptr != NULL; ptr = ptr->next[0])
		{
			if (ptr->tag)
			{
				cout << ptr->type.idx << " ";
				if (ptr->ptrnum > 1)
				{
					for (int i = 1; i < ptr->ptrnum; i++)
					{
						cout << ptr->type.idx << " ";
						div->push((ptr->next[i]));
					}
				}
			}
			else
				cout << ptr->type.c << " ";
		}
		cout << endl;
	}

	cout << endl;
	cout <<"maxstate: "<< max_state << " patnum: " << pat_num << endl;
}

void trie::initindex()
{
	queue<node*> frag; //�б����� ������ ���
	node* ptr = start;
	for (ptr; ptr != NULL; ptr = ptr->next[0])
	{
		if (ptr->tag) //state �϶�
		{
			idx.push_back(ptr);
			if (ptr->ptrnum > 1) //�ٸ��б⿡�� state node�� �����ϴ� �˻縦 ���� ť�� �ִ´�.
			{
				for (int i = 1; i < ptr->ptrnum; i++)
				{
					frag.push(ptr->next[i]);
				}
			}
		}
	}
	//�����϶��� �������̾���.

	while (!frag.empty())
	{
		ptr = frag.front();
		frag.pop();
		for (ptr; ptr != NULL; ptr = ptr->next[0])
		{
			if (ptr->tag)
			{
				idx.push_back(ptr);
				if (ptr->ptrnum > 1)
				{
					for (int i = 1; i < ptr->ptrnum; i++)
					{
						frag.push(ptr->next[i]);
					}
				}
			}
		}
	}
}

void trie::idxshow()
{
	for (int i = 0; i < idx.size(); i++)
		cout << (idx[i])->type.idx << " ";
	cout << endl;
}

bool compareCount(pair<int,int> r1, pair<int, int> r2)
{
	return r1.first > r2.first;
}

void trie::initff() //n = pat_num
{
	ff = new int[max_state + 1];
	ff[0] = 0; ff[1] = 0; //0: ���ڿ����� 0, 1: ���ڿ� ����1
	queue<char> letter; //���ڸ� ������ ť
	queue<node*> frag;  //�б⸦ ������ ť

	vector<pair<node*, int>> path; //�������� ����
	pair<node*, int> set; //���� ���� ��������
	node* ptr = start; //���� �񱳸� �ϱ� ���� ������
	int select = 0; //�б������� ���� ������

	int count = 0; //��ġ Ƚ��
	int dif_count = 0; //�ٸ� Ƚ��
	vector<pair<int, int>> temp; //��ġȽ���� ������ state�� idx�� ����
	pair<int, int> now_temp; // temp�� ������ pair����
	int s = 0; //����� ����
	queue<node*> done; //�б⿡�� ���������� ����
	node* tempnode; //������ ��带 ������ ��
	int bef = 0; //�������� ���ư������� �ε����� �����Ұ�
	

	for (int i = 2; i <= max_state; i++)
	{
		ptr = start;
		select = 0;
		while (ptr->type.idx !=i)
		{
			if (ptr->next[0] != NULL)
			{
				set.first = ptr; set.second = select;
				path.push_back(set);
				ptr = ptr->next[select]->next[0];
				select = 0;
			}
			else
			{
				while (set.first->ptrnum == 1)
				{
					set.first = path.back().first; set.second = path.back().second;
					path.pop_back();
				}

				ptr = set.first;
				select = set.second + 1;
			}
		}
		path.erase(path.begin());

		//���� �����Ⱑ������ ��
		select = 0;
		count = 0;
		dif_count = 0;
		ptr = start;
		tempnode = start;
		while (!path.empty())
		{
			for (int i = 0; i < ptr->ptrnum; i++)
			{
				if (ptr->next[i]!=NULL && ptr->next[i]->type.c == path.front().first->next[path.front().second]->type.c)
				{
					count++;
					s = i; //�������� ������. �� ��θ� ���ϱ� ���� üũ
				}
				else {
					dif_count++;
				}
			}

			if (count == 0)
			{
				ptr = start; dif_count = 0;
				path.erase(path.begin());
			}
			else {//count�� �ռ� ���Ҵ������� 0�̾ƴҶ�
				if (dif_count == ptr->ptrnum) //
				{
					dif_count = 0; count = 0;
					bef = tempnode->type.idx;
					ptr = idx[ff[bef]];
				}
				else
				{
					tempnode = path.front().first;
					ptr = ptr->next[s]->next[0];
					dif_count = 0; s = 0;
					path.erase(path.begin());
				}
			}

		}
		

		if (count == 0)
			ff[i] = 0;
		else
		{
			ff[i] = ptr->type.idx;
		}
		cout << ff[i] << " ";
		cout << endl;
		temp.clear();


	}


}

void trie::ffshow() const
{
	for (int i = 0; i <= max_state; i++)
		cout << ff[i] << " ";
	cout << endl;
}

void trie::match(string& str)
{
	int len = str.length();
	int p = 0; //���� ��Ʈ�����ڿ��� ����Ű�� ������
	node* ptr = start; //Ʈ���̸� ����Ű�� ���� ������
	node* bef = start; //������带 ����Ű������ ������
	int* arr = new int[pat_num]; //��ġ ���ڿ��� ���� ���� ����
	for (int i = 0; i < pat_num; i++)
		arr[i] = 0;

	cout << arr[0] << arr[1] << arr[2] << endl;
	//finalstate����
	int* finalstate = new int[pat_num];
	for (int i = 0; i < pat_num; i++)
		finalstate[i] = 0;
	cout << finalstate[0] << finalstate[1] << finalstate[2] << endl;
	queue<node*> fragment;
	for (ptr; ptr->next[0] != NULL; ptr = ptr->next[0])
	{
		if (ptr->ptrnum > 1)
		{
			for (int i = 1; i < ptr->ptrnum; i++)
				fragment.push(ptr->next[i]);
		}
	}
	finalstate[0] = ptr->type.idx;
	cout << finalstate[0] << endl;
	while (!fragment.empty())
	{
		p = 1;
		ptr = fragment.front();
		fragment.pop();
		for (ptr; ptr->next[0] != NULL; ptr = ptr->next[0])
		{
			if (ptr->ptrnum > 1)
			{
				for (int i = 1; i < ptr->ptrnum; i++)
					fragment.push(ptr->next[i]);
			}
		}
		finalstate[p] = ptr->type.idx;
		cout << finalstate[p] << endl;
		p++;
	}

	p = 0;
	int s = 0;
	int fnum = 0;//�б��� ��
	int count = 0; //��ġ ��
	int dif_count = 0; //����ġ��
	ptr = start;
	bool x = false;
	while (p < len)
	{
		x = false;
		bef = ptr;
		if (ptr->next[0] != NULL && str[p] != ptr->next[0]->type.c && ptr->ptrnum == 1)
		{
			if (ptr == start)
				p++;
			ptr = idx[ff[bef->type.idx]];
		}
		else if (ptr->next[0] != NULL && str[p] != ptr->next[0]->type.c && ptr->ptrnum > 1 )
		{
			for (int i = 0; i < ptr->ptrnum; i++)
			{
				if (str[p] == ptr->next[i]->type.c)
				{
					if (ptr->next[i]->next[0] == NULL)
					{
						//finalstate
						x = true;
						p++;
						for (int i = 0; i < pat_num; i++)
						{
							if ((finalstate[i]) == (ptr->type.idx))
							{
								arr[i] += 1;
							}
						}
						ptr = idx[ff[(ptr->type.idx)]];

						
						break;
					}
					else
					{
						x = true;
						p++;
						ptr = ptr->next[i]->next[0];
						break;
					}
				}
			}

			if(x == false)
			{
				ptr = idx[ff[(bef->type.idx)]];
			}
		}
		else //������
		{
			p++;
			if (ptr->next[0] == NULL)
			{
				//finalstate
				for (int i = 0; i < pat_num; i++)
				{
					if (finalstate[i] == (ptr->type.idx))
					{
						arr[i] = arr[i] + 1;
					}
				}
				ptr = idx[ff[ptr->type.idx]];
				x = true;
			}
			if(x == false)
			{
				ptr = ptr->next[0]->next[0];
			}

		}

		
	}

	for (int i = 0; i < pat_num; i++)
		printf("%d ", finalstate[i]);
	cout << endl;
	for (int i = 0; i < pat_num; i++)
		cout << arr[i] << " ";
	cout << endl;
}