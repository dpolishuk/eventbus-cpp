#include <iostream>

#include "eventbus.hpp"

using namespace std;

void f(int i);

void test(int i) {
	std::cout << "hello from test\n";
}

int main()
{
	event_bus bus;
	boost::function<void (int)> f = &test;
	bus.subscribe(f);

	bus.post<void (int)>(1);

	return 0;
}

