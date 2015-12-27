#include <iostream>
#include <map>

int r0 = 4;
int r1 = 1;
int r7 = 1;

std::map<std::pair<int, int>, std::pair<int, int> > map;

std::pair<int, int> fun(int a, int b)
{
	int orig_a = a;
	int orig_b = b;

	if(map.find(std::make_pair<int, int>(a, b)) != map.end())
	{
		return map[std::make_pair<int, int>(a, b)];
	}

	if(a == 0)
	{
		int res = (b + 1) % 32768;
		std::pair<int, int> ret = std::make_pair<int, int>(res, r7);
		map.insert(std::make_pair(std::make_pair(orig_a, orig_b), ret));
		return ret;
	}
	else if(b == 0)
	{
		a = (a + 32767) % 32768;
		std::pair<int, int> res = fun(a, r7);
		std::pair<int, int> ret = std::make_pair<int, int>(res.first, b);
		map.insert(std::make_pair(std::make_pair(orig_a, orig_b), ret));
		return ret;
	}
	else
	{
		b = (b + 32767) % 32768;
		b = fun(a, b).first;
		a = (a + 32767) % 32768;
		std::pair<int, int> res = fun(a, b);
		std::pair<int, int> ret = std::make_pair<int, int>(res.first, b);
		map.insert(std::make_pair(std::make_pair(orig_a, orig_b), ret));
		return ret;
	}
}

int main()
{
	for(r7=1; r7 < 32768; r7++)
	{
		std::pair<int, int> res = fun(4, 1);
		
		if(res.first == 6)
		{
			std::cout << "FOUND 6: " << r7 << std::endl;
			std::cout << "r0: " << 6 << std::endl;
			std::cout << "r1: " << res.second << std::endl;
			return 0;
		}
		else if(r7 % 100 == 0)
		{
			std::cout << "NOT UP TO " << r7 << std::endl;
		}
		map.clear();
	}

	return 1;
}