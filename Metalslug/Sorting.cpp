#include "Sorting.h"

Sorting::Sorting()
{
	sd = new SortData[sortMax];
	num = 0;
}

Sorting::~Sorting()
{
	delete sd;
}

void Sorting::init()
{
	num = 0;

}

void Sorting::insert(int value)
{
	sd[num].index = num;
	sd[num].value = value;
	num++;
}

void Sorting::update()
{
	for (int i = 0; i < num - 1; i++)
	{
		for (int j = i; j < num; j++)
		{
			if (sd[i].value > sd[j].value)
			{
				SortData t = sd[i];
				sd[i] = sd[j];
				sd[j] = t;
			}
		}
	}
}

int Sorting::get(int index)
{
	return sd[index].index;
}