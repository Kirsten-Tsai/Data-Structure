#include<iostream>
#include<vector>
#include <limits.h>
using namespace std;


class Person{
public:
	Person();
	Person(int i, int e);
	~Person();
	int low_energy (int);
	void change_sibling(Person*);
	void change_child(Person*);
	void change_parent(Person*);
	void calc_num();
	Person *find(int);

private:
	Person *parent, *child, *sibling; // the pointers to his parent, child, sibling
	int id; // record Person's id
	int energy; // record Person's energy
	int degree;// record how many levels of this Person's children
	int num;
	friend class Promenade;//you could access all valuables in class Promenade
	
};

//ctor
Person::Person(){
	parent = child = sibling = NULL;
	id = 0;
	energy = 0;
	degree = 0;
	num = 1;
}

//ctor
Person::Person(int i, int e){// define a new Person with id = i and energy = e
	parent = child = sibling = NULL;
	id = i;
	energy = e;
	degree = 0;
	num = 1;
}

//dtor
Person::~Person(){

}

class Promenade{

public:
	Promenade();
	~Promenade();
	void one_person_joined(int id, int energy);
	void one_group_joined(Promenade *g);
	void absorb_energy(int id, int energy);
	void keep_in_def();
	int  calculate_people_below(int energy);
	int  get_weakest_person();
	int  size();
	void find_min();
	void merge(Person&);
	Person* get_head() {return head;}

private:
	Person *head;
	int num;//caculate how many people in the promenade
	Person *min;
};

//ctor
Promenade::Promenade(){
	head = NULL;
	num = 0;
	min = NULL;
}

//dtor
Promenade::~Promenade(){

}

void Promenade::one_person_joined(int id, int energy){
	//TODO: fill in the code to simulate a person with his id and energy to join the Promenade
    Person *man = new Person(id,energy);
    merge(*man);
    find_min();
}

void Promenade::one_group_joined(Promenade *g){
	//TODO: fill in the code to simulate one group to join the full promenade
	Person *pre = g->head;
	Person *cur = g->head;
	//this->num = this->num + g->num;
	while (cur!=NULL)
	{
		pre = cur;
		cur = cur->sibling;
		merge(*pre);
	}
	keep_in_def();
	find_min();
}


void Promenade::merge(Person &g_merge)
{
	if (min!=NULL)
	{
		if (min->energy > g_merge.energy)
			min = &g_merge;
	}
	if (num==0)		//this is empty B-heap
	{
		head = &g_merge;
		num = g_merge.num;
		return;
	}
	Person *cur = head;
	Person *pre = head;

	while (cur!=NULL)	//find where to insert
	{
		if (cur->degree < g_merge.degree || ( cur->degree == g_merge.degree && cur->energy > g_merge.energy) )
		{
			pre = cur;
			cur = cur->sibling;
		}
		else
			break;
	}

	if (cur==pre) //insert at the beginning
	{
		g_merge.change_sibling(head);
		head = &g_merge;
	}
	else
	{
		pre->change_sibling(&g_merge);
		g_merge.change_sibling(cur);
	}
	num += g_merge.num;
	return;
}

void Person::change_sibling(Person *new_sibling)
{
	sibling = new_sibling;
}

void Person::change_child(Person *new_child)
{
	child = new_child;
	degree = new_child->degree + 1;
	num *= 2;
}

void Person::change_parent(Person *new_parent)
{
	parent = new_parent;
}

void Person::calc_num(){
	num = 1;
	for (int i=0; i<degree; i++){
		num *= 2;
	}
}

void Promenade::keep_in_def()
{
	Person *cur = head;
	Person *pre = head;

	while(cur->sibling!=NULL)
	{
		bool change = false;
		Person *next = cur->sibling;

		if(cur->degree==next->degree)
		{
			cur->change_sibling(next->child);
			next->change_child(cur);
			cur->change_parent(next);
			num = num - 2*cur->num;	//take out the heap
			if (cur==head)
			{
				head = next ->sibling;
				cur = head;
				pre = head;
				change = true;
			}
			else
			{
				pre->change_sibling(next->sibling);
			}
			merge(*next);
			if (change)
			{
				cur = head;
				pre = head;
				next = cur->sibling;
			}
			else
			{
				cur = pre->sibling;
			}
		}
		else
		{
			pre = cur;
			cur = cur->sibling;
		}
	}
}

void Promenade::absorb_energy(int id, int energy){
	//TODO: Elizabeth absord the energy of the person with this id, this Person's energy will decrease by this number.
	//		If his energy becomes lower than or equal to zero, he will die and should be thrown out automatically
	if (head == NULL)
	{
		return;
	}
	Person *pre_root = NULL;
	Person *cur_root = head;
	Person *target = NULL;
	while (cur_root != NULL)
	{
		target = cur_root->find(id);
		if (target)
			break;
		else
		{
			pre_root = cur_root;
			cur_root = cur_root->sibling;
		}
	}
	if (target==NULL)
		return;
	target->energy -= energy;
	Person *cur = target;
	Person *parent = cur->parent;
	while (parent != NULL && cur->energy < parent->energy)
	{
		int tmp;
		tmp = cur->energy;
		cur->energy = parent->energy;
		parent->energy = tmp;

		tmp = cur->id;
		cur->id = parent->id;
		parent->id = tmp;

		cur = parent;
		parent = cur->parent;
	}
	target = cur;
	if (target->energy <= 0)
	{
		if (pre_root==NULL)
		{
			this->head = target->sibling;
		}
		else
			pre_root->sibling = target->sibling;
		if (target->child != NULL)
		{
			Person *it = target->child;
			vector<Person*> ready_to_merge;
			while (it!=NULL)
			{
				ready_to_merge.push_back(it);
				num -= it->num;
				it = it->sibling;
			}
			for (int i = 0; i<ready_to_merge.size();i++)
			{
				merge(*(ready_to_merge[i]));
			}
			keep_in_def();
			find_min();
		}
		delete target;
		num--;

	}


}

Person* Person::find(int ID)
{
	if (ID==id)
		return this;
	else
	{
		Person *cur = child;
		while (cur!=NULL)
		{
			Person *target = cur->find(ID);
			if (target)
				return target;
			else
				cur = cur->sibling;
		}
	}
	return NULL;
}

int Promenade::calculate_people_below(int energy){
	//TODO: You should calculate how many people's energy are below the number (include the number) and return it
    int people = 0;
    Person *cur = head;
    while (cur!=NULL)
    {
        people = people + cur->low_energy(energy);
        cur = cur->sibling;
    }
	return people;
}

int Person::low_energy(int energy)
{
    int people = 0;
    Person *cur;
    if (this->energy > energy)
        return 0;
    else
    {
        people = 1;
        cur = this->child;
        while (cur != NULL)
        {
            people = people + cur->low_energy(energy);
            cur = cur->sibling;
        }
        return people;
    }
}

int Promenade::get_weakest_person(){
	//TODO: return the id of the one with the least energy left. If there are more than one solution, just return one of them
	return min->id;
}

void Promenade::find_min()
{
	Person *cur = head;
	Person *pre = head;
	int minimum = INT_MAX;
	while (cur!=NULL)
	{
		if (cur->energy < minimum)
		{
			minimum = cur->energy;
			min = cur;
		}
		pre = cur;
		cur = cur->sibling;
	}
}

int Promenade::size(){

	return num;
}
