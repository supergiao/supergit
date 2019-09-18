#include <iostream>
#include <string.h>

using namespace std;

class RefBase {
private:
	int count;
public:
	RefBase() : count(0) {}
	void incStrong()
	{
		count++;
	}

	void decStrong()
	{
		count--;
	}

	int getStrongCount()
	{
		return count;
	}

};

class Person : public RefBase {
public:
	Person() {
		cout << "Person()" << endl;
	}

	~Person() {
		cout << "~Person()" << endl;
	}

	void printInfo(void)
	{
		cout << "just a test" << endl;
	}
};

template<typename T>
class sp {
private:
	T *p;
public:
	sp() : p(0) {}

	sp(T *other)
	{
		cout << "sp(const T *other)" << endl;
		p = other;
		p->incStrong();
	}

	sp(const sp &other)
	{
		cout << "sp(const sp &other)" << endl;
		p = other.p;
		p->incStrong();
	}

	~sp()
	{
		cout << "~sp(const T *other)" << endl;
		if (p)
		{
			p->decStrong();
			if (p->getStrongCount() == 0)
			{
				delete p;
				p = NULL;
			}
		}
	}

	T *operator->()
	{
		return p;
	}

	T& operator*()
	{
		return *p;
	}
};

template<typename T>
void test_func(sp<T> &other)
{
	sp<T> s = other;
	cout << "In call test_func: " << other->getStrongCount() << endl;

	s->printInfo();
}

void main(void)
{
	sp<Person> other = new Person();

	(*other).printInfo();

	cout << "Before call test_func: " << other->getStrongCount() << endl;

	test_func(other);
	cout << "After call test_func: " << other->getStrongCount() << endl;
}
