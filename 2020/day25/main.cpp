#include <cassert>
#include <iostream>
#include <fstream>

long part1(long key1, long key2)
{
  long value = 1, loopsize = 0;;
  for (;value != key2;++loopsize)
  {
    value *= 7;
    value = value % 20201227;
  }

  value = 1;
  for (int i = 0;i < loopsize;++i)
  {
    value *= key1;
    value = value % 20201227;
  }
  return value;
}

int main()
{

  assert(part1(5764801, 17807724) == 14897079);
  assert(part1(17807724, 5764801) == 14897079);

  long key1, key2;
  std::ifstream infile("data.txt");
  assert(infile >> key1 >> key2);
  std::cout << part1(key1, key2) << std::endl; // 11576351

  return 0;
}