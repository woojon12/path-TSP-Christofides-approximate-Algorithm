#pragma once
#include <iostream>

/*
무한의 개념을 확립하려고 만든 클래스인데 어쩌다보니
가중치의 개념으로 바뀌어버림.

인스턴스 생성 시 아무것도 안 주면 무한으로 취급.
숫자를 주면 그 숫자로 취급.

덧셈뺄셈 오버로딩은 필요하면 만들겠음.

나중에 또 생각을 해봤는데
'무한 + 무한'이 그냥 '무한'보다 크다란 개념도 해야겠더라
그래서 무한이 몇번 들어갔는지 카운트 하는 게 필요함.
0번이면 무한 아니고 1번 이상이면 무한이고 -1번 이하면 음의 무한이고
*/

class cost {
  int weight;
  bool absoluteInf;
  bool absoluteMInf;

public:
  cost() : absoluteInf(true), absoluteMInf(false) {}
  cost(bool plus_minus_inf) {
    if(plus_minus_inf) {absoluteInf = true; absoluteMInf = false;}
    else {absoluteInf = false; absoluteMInf = true;}
  }
  cost(int weight) : weight(weight), absoluteInf(false) {}

  friend class graph;

  friend bool operator<(cost, cost);
  friend bool operator>(cost, cost);
  friend bool operator<=(cost, cost);
  friend bool operator>=(cost, cost);
  friend bool operator==(cost, cost);
  friend bool operator!=(cost, cost);
  friend cost operator+(cost, cost);
  friend cost operator-(cost, cost);
  friend std::ostream& operator<<(std::ostream&, cost&);
};

bool operator<(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return false;
  else if (t1.absoluteInf) return false;
  else if (t2.absoluteInf) return true;
  else if (t1.absoluteMInf) return true;
  else if (t1.absoluteMInf) return false;

  return t1.weight < t2.weight;
}

bool operator>(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return false;
  else if (t1.absoluteInf) return true;
  else if (t2.absoluteInf) return false;
  else if (t1.absoluteMInf) return false;
  else if (t1.absoluteMInf) return true;

  return t1.weight > t2.weight;
}

bool operator<=(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return true;
  if(t1.absoluteMInf && t2.absoluteMInf) return true;
  return t1 < t2;
  //사실 이러면 약간의 불필요한 if 연산이 일어나지만 쉬운 코드 이해를 위해 희생
}

bool operator>=(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return true;
  if(t1.absoluteMInf && t2.absoluteMInf) return true;
  return t1 > t2;
}

bool operator==(cost t1, cost t2)
{
  if(t1.absoluteInf && t2.absoluteInf) return true;
  else if(t1.absoluteMInf && t2.absoluteMInf) return true;
  else if(t1.weight == t2.weight) return true;
  return false;
}

bool operator!=(cost t1, cost t2)
{
  return !(t1 == t2);
}

cost operator+(cost c1, cost c2)
{
  if(c1.absoluteInf || c2.absoluteInf) return cost(true);
  if(c1.absoluteMInf || c2.absoluteMInf) return cost(false);
  return c1.weight + c2.weight;
}

cost operator-(cost c1, cost c2)
{
  if(c1.absoluteInf && c2.absoluteInf) return 0;
  if(c1.absoluteMInf && c2.absoluteMInf) return 0;
  if(c1.absoluteInf) return cost(true);
  if(c2.absoluteInf) return cost(false);
  if(c1.absoluteMInf) return cost(false);
  if(c2.absoluteMInf) return cost(true);
  
  return c1.weight - c2.weight;
}

std::ostream& operator<<(std::ostream& o, cost& c)
{
  if(c.absoluteInf) {o << "+Infinite"; return o;}
  if(c.absoluteMInf) {o << "-Infinite"; return o;}
  o << c.weight;
  return o;
}