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
	void initindex(); //idx배열을 통해 바로 스테이트로 갈 수 있다.
	void idxshow();
	void initff();  //failure function
	void ffshow() const;
	void match(string& str);
};





int main(void)
{
	//트라이생성
	cout << "트라이 생성" << endl;
	trie tr;
	//패턴 트라이에 넣기
	cout << "패턴 넣기" << endl;
	string pat = "aaa";
	tr.push(pat);
	string pat2 = "abaab";
	tr.push(pat2);
	string pat3 = "ababaa";
	tr.push(pat3);
	//패턴 보기 
	cout << "패턴이 들어간 트라이 확인(마지막 숫자는 max_state," <<'\n' << " 숫자두번은 분기점을 뜻함" << endl;
	tr.show();
	//인덱스 배열 생성 및 확인
	cout << "인덱스 배열" << endl;
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
	//일단 일자로 읽으면서 스택에 넣음, 단 분기가있는 노드의 경우 큐에 분기들만 저장
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
	//큐에 있는 분기들도 스택에 넣음
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

	//스택에 제일 마지막 노드부터 꺼내면서 메모리 해제, 마지막에 스타트 노드도 메모리 해제함(처음에 넣어서)
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
	int k = 0;//next를 가리키기위한 변수
	while (i < str.length())
	{
		if (ptr->tag == true) //state이므로 건너뛰는데, 다음 문자가 다를것을 대비해 주소 저장
		{
			temp = &(*ptr);
			ptr = ptr->next[k];
		}
		else {
			if (ptr->type.c == str[i]) //문자가 같으면
			{
				k = 0;
				ptr = ptr->next[0]; //다음문자를 검사한다.
				i++;
			}
			else //문자가 다르면
			{
				//이전 포인터의 자식노드가 몇개인지 확인
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
	queue<node*> frag; //분기점을 저장할 노드
	node* ptr = start;
	for (ptr; ptr != NULL; ptr = ptr->next[0])
	{
		if (ptr->tag) //state 일때
		{
			idx.push_back(ptr);
			if (ptr->ptrnum > 1) //다른분기에도 state node가 존재하니 검사를 위해 큐에 넣는다.
			{
				for (int i = 1; i < ptr->ptrnum; i++)
				{
					frag.push(ptr->next[i]);
				}
			}
		}
	}
	//문자일때는 넣을것이없다.

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
	ff[0] = 0; ff[1] = 0; //0: 문자열길이 0, 1: 문자열 길이1
	queue<char> letter; //문자를 저장할 큐
	queue<node*> frag;  //분기를 저장할 큐

	vector<pair<node*, int>> path; //가던길을 저장
	pair<node*, int> set; //현재 고른거 담을공간
	node* ptr = start; //패턴 비교를 하기 위한 포인터
	int select = 0; //분기점에서 어디로 가는지

	int count = 0; //일치 횟수
	int dif_count = 0; //다른 횟수
	vector<pair<int, int>> temp; //일치횟수와 가야할 state의 idx를 저장
	pair<int, int> now_temp; // temp에 저장할 pair세팅
	int s = 0; //사용할 변수
	queue<node*> done; //분기에서 안지나간곳 저장
	node* tempnode; //직전의 노드를 저장할 것
	int bef = 0; //이전으로 돌아가기위한 인덱스를 저장할것
	

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

		//패턴 따오기가끝나면 비교
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
					s = i; //같은경우는 유일함. 그 경로를 정하기 위해 체크
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
			else {//count가 앞서 같았던이유로 0이아닐때
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
	int p = 0; //현재 스트링문자열을 가르키는 포인터
	node* ptr = start; //트라이를 가리키기 위한 포인터
	node* bef = start; //이전노드를 가리키기위한 포인터
	int* arr = new int[pat_num]; //일치 문자열을 세고 여기 저장
	for (int i = 0; i < pat_num; i++)
		arr[i] = 0;

	cout << arr[0] << arr[1] << arr[2] << endl;
	//finalstate정의
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
	int fnum = 0;//분기점 수
	int count = 0; //일치 수
	int dif_count = 0; //불일치수
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
		else //같을때
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