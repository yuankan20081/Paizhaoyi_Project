#pragma once
#include <pthread.h>
#include <queue>
using std::queue;


class DataPool
{
public:
	DataPool();
	~DataPool();
	bool Write();
	bool Read();
private:
};