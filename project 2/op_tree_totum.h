#include<iostream>
using namespace std;

class node{
public:
	node();
	node(int value);
	~node();
	friend void gothrough(node *p);

private:
	node *left, *right; // the left child and the right child pointer
	int number; // record the node's number
	int is_threadl, is_threadr; //the flag to record whether the pointer is a thread or not

	friend class op_tree_totum;//you could access all valuables in class op_tree_totum
};

//ctor
node::node(){
	left = right = NULL;
	is_threadl = 1; ///1 -> thread
	is_threadr = 1;
}

//ctor
node::node(int value){
	number = value;
	left = right = NULL;
	is_threadl = 1;
	is_threadr = 1;
}

//dtor
node::~node(){

}

class op_tree_totum{

public:
	op_tree_totum();
	~op_tree_totum();
	void insertion(int s);
	void deletion(int s);
	void inorder_run();
	void reverseorder_run();
	int size();
	node* find_max(node*);
	node* find_min(node*);

private:
	node *root, *head, *tail;
	int num;//caculate how many nodes in the totum
};

//ctor
op_tree_totum::op_tree_totum(){
	head = new node();
	tail = new node();
	head->right = tail; //initialize the head node to connect to the tail node
	tail->left = head;
	root = NULL;
	num = 0;
}

//dtor
op_tree_totum::~op_tree_totum(){
	node *p = root;
	if(p!=NULL)	gothrough(p);
	num = 0;
	if (head != NULL)delete head;
	if (tail != NULL)delete tail;
}

void gothrough(node *p){
	if (p->is_threadl==0 && p->left!= NULL) gothrough(p->left);
	if (p->is_threadr==0 && p->right!= NULL) gothrough(p->right);
	delete p;
}

void op_tree_totum::insertion(int s){
	//TODO: fill in the code to do the insertion of the node with number s

	node *now;
	node *newnode = new node(s);
	newnode->number = s;
	newnode->is_threadl=1;
	newnode->is_threadr=1;
	if (root==NULL)
	{
		newnode->left=head;
		newnode->right=tail;
		head->right=newnode;
		tail->left=newnode;
		root=newnode;
	}
	else
	{
	    now=root;
		for (;;)
		{
            if (now->number > s)
            {
                if (now->is_threadl)
                    break;
                now = now -> left;
            }
            else if (now->number < s)
            {
                if (now->is_threadr)
                    break;
                now = now-> right;

            }
            else
                break;

		}


		if (now->number > s)	//insert in left
		{
			newnode->right = now;
			newnode->left = now->left;
			now->left = newnode;
			now->is_threadl=0;
			if (newnode->left==head)
			{
				head->right= newnode;
			}
		}
		else
		{
			newnode->right=now->right;
			newnode->left=now;
			now->right=newnode;
			now->is_threadr=0;
			if (newnode->right==tail)
			{
				tail->left=newnode;
			}
		}


	}

	num++;
}

node* op_tree_totum::find_max(node* now)
{
    while(!now->is_threadr)
        now=now->right;
    return now;
}

node* op_tree_totum::find_min(node* now)
{
    while(!now->is_threadl)
        now=now->left;
    return now;
}

void op_tree_totum::deletion(int s){
	//TODO: fill in the code to do the deletion of the node with number s
    node *now;
    node *parent;
    node *substitute;
    int from=0;
    now=root;
    parent=root;
    for (;;)
    {
        if (now->number > s)    ///go to left
        {
            if(now->is_threadl)
                return;
            else
            {
                parent=now;
                now=now->left;
                from=1; ///it is a left child
            }

        }
        else if (now->number < s)   ///go to right
        {
            if (now->is_threadr)
                return;
            else
            {
                parent=now;
                now=now->right;
                from=2; ///it is a right child
            }
        }
        else    ///found
        {
            break;
        }

    }
	 //printf("now : %d from : %d\n",now->number,from);
    if (!now->is_threadl && !now->is_threadr)   ///2children
    {
        if (from==0)    ///root
        {
            node* MAX=find_max(now->left);
            node* MIN=find_min(now->right);
            MIN->left=now->left;
            MAX->right=MIN;
            MIN->is_threadl=0;
            root=now->right;
        }
        else if (from==1)
        {
            node* MAX=find_max(now->left);
            node* MIN=find_min(now->right);
            MIN->left=now->left;
            MAX->right=MIN;
            MIN->is_threadl=0;
            parent->left=now->right;    ///the smallest on the right subtree still larger than left subtree
            ///take the smallest to adopt the whole left subtree
        }
        else if (from==2)
        {
            node* MAX=find_max(now->right);
            node* MIN=find_min(now->left);
            MIN->left=now->left;
            MAX->right=MIN;
            MIN->is_threadl=0;
            parent->right=now->right;
        }
    }
    else if (!now->is_threadl && now->is_threadr)   ///only left child
    {
        if (from==0)
        {
            node *MAX=find_max(now->left);
            MAX->right=now->right;
            root=now->left;
            if (MAX->right==tail)
                tail->left=MAX;
        }
        else if (from==1)
        {
            node *MAX=find_max(now->left);
            parent->left = now->left;
            MAX->right = now->right;
        }
        else if (from==2)
        {
            node *MAX=find_max(now->left);
            MAX->right=now->right;
            parent->right=now->left;
            if (MAX->right==tail)
                tail->left=MAX;
        }
    }
    else if (now->is_threadl && !now->is_threadr)   ///only right child
    {
        if (from==0)
        {
            node *MIN=find_min(now->right);
            MIN->left=now->left;
            root=now->right;
            if (MIN->left==head)
                head->right=MIN;
        }
        if (from==1)
        {
            node *MIN=find_min(now->right);
            parent->left=now->right;
            MIN->left=now->left;
            if (MIN->left==head)
                head->right=MIN;
        }
        if (from==2)
        {
            node *MIN=find_min(now->right);
            parent->right=now->right;
            MIN->left=now->left;
        }
    }
    else if (now->is_threadl && now->is_threadr)    ///no child
    {
        if (from==0)
        {
            root=NULL;
            head->right=tail;
            tail->left=head;
        }
        else if (from==1)
        {
            parent->left=now->left;
            parent->is_threadl=1;
            if (parent->left==head)
                head->right=parent;
        }
        else if (from==2)
        {
            parent->right=now->right;
            parent->is_threadr=1;
            if (parent->right==tail)
                tail->left=parent;
        }
    }
    delete now;
    num--;
}

void op_tree_totum::inorder_run(){
	//TODO: travel the tree from the head node to the tail node and output the values
	//You should NOT use stack or recursion
    bool print=0;   ///if the previous one is thread -> it can be printed
    node *now;
    now = head->right;
    while(now!=tail)
    {

        if (!print)
        {
            if (!now->is_threadl)
            {
                now=now->left;
                print=0;
            }
            else
            {
                cout << now->number << " ";
                print=now->is_threadr;
                now=now->right;
            }
        }
        else
        {
            cout << now->number << " ";
            print=now->is_threadr;
            now=now->right;
        }
    }
}

void op_tree_totum::reverseorder_run(){
	//TODO: travel the tree from the tail node to the head node and output the values
	//You should NOT use stack or recursion
    bool print=0;   ///if the previous one is thread -> it can be printed
    node *now;
    now = tail->left;
    while(now!=head)
    {
        if (!print)
        {
            if (!now->is_threadr)
            {
                now=now->right;
                print=0;
            }
            else
            {
                cout << now->number << " ";
                print=now->is_threadl;
                now=now->left;
            }
        }
        else
        {
            cout << now->number << " ";
            print=now->is_threadl;
            now=now->left;
        }
    }
}

int op_tree_totum::size(){
	return num;
}
