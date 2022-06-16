#pragma once

#define sortMax 100

struct SortData
{
	int index;
	int value;
};

class Sorting
{
public:
	SortData* sd;
	int num;

	Sorting();
	~Sorting();

	void init();
	void insert(int value);
	void update();
	int get(int index);
};

