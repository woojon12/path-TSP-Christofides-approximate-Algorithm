#pragma once

/*
무한의 개념을 확립하려고 만든 클래스인데 어쩌다보니
가중치의 개념으로 바뀌어버림.

인스턴스 생성 시 아무것도 안 주면 무한으로 취급.
숫자를 주면 그 숫자로 취급.

덧셈뺄셈 오버로딩은 필요하면 만들겠음.
*/

class cost {
  double weight;
  bool absoulteInf;

public:
  cost() : absoulteInf(true) {}
  cost(double weight) : weight(weight), absoulteInf(false) {}

  friend bool operator<(cost, cost);
  friend bool operator>(cost, cost);
  friend bool operator<=(cost, cost);
  friend bool operator>=(cost, cost);
  friend bool operator==(cost, cost);
};

bool operator<(cost t1, cost t2)
{
  if(t1.absoulteInf && t2.absoulteInf) return false;
  else if (t1.absoulteInf) return false;
  else if (t2.absoulteInf) return true;

  return t1.weight < t2.weight;
}

bool operator>(cost t1, cost t2)
{
  if(t1.absoulteInf && t2.absoulteInf) return false;
  else if (t1.absoulteInf) return true;
  else if (t2.absoulteInf) return false;

  return t1.weight > t2.weight;
}

bool operator<=(cost t1, cost t2)
{
  if(t1.absoulteInf && t2.absoulteInf) return true;
  return t1 < t2;
  //사실 이러면 약간의 불필요한 if 연산이 일어나지만 쉬운 코드 이해를 위해 희생
}

bool operator>=(cost t1, cost t2)
{
  if(t1.absoulteInf && t2.absoulteInf) return true;
  return t1 > t2;
}

bool operator==(cost t1, cost t2)
{
  if(t1.absoulteInf && t2.absoulteInf) return true;
  else if(t1.weight == t2.weight) return true;
  return false;
}
