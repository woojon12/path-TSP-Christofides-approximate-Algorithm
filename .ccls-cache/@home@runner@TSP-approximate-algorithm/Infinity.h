#pragma once

/*
무한의 개념을 확립한 클래스.
이 클래스의 인스턴스와 아무 숫자를 비교하게 되면
무조건 이 클래스의 인스턴스가 크다고 판단한다.

기본 사용법으로 inf 인스턴스 생성 시 아무런 인자도 주지 말아야 하며
그래야 absoulteInf가 true가 되어 무한이라고 판단한다.

double num을 인자로 받는 생성자는 왜 만들었냐면
연산자 오버로딩의 인자를 둘 다 inf로 빋게 작성해서
숫자 > inf
inf > 숫자
어느 쪽이든 컴파일러가 숫자를 자동으로 inf로 변환하여
연산자 오버로딩 함수로 가게 하기 위함.
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
