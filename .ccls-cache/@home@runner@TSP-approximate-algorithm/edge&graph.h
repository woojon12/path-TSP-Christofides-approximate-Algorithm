#pragma once
#include "infinity.h"

class graph;

class vertex {
  int number;
  bool visited;
  
  public:
  vertex(int number)
  : number(number), visited(false) {}
};


class edge {
private:
  double cost;
  vertex departure, destination;

  friend class graph;
public:
  edge(){}
  edge(vertex departure, vertex destination, double cost)
  : departure(departure), destination(destination), cost(cost)
  {}
};

class graph {
  edge* edges;
  int count;
  int capacity;


public:
  graph(int capacity) : capacity(capacity), count() {
    edges = new edge[capacity];
  }

  ~graph() {
    delete[] edges;
  }

  graph& push(int departure, int destination, double cost){
    edge element(departure, destination, cost);

    if(alreadyExist(element)) return *this;
    
    edges[count++] = element;

    return *this;
  }

  graph& push(edge element){
    return push(element.departure, element.destination, element.cost);
  }

private:
  bool alreadyExist(edge element) {
    for(int i = 0; i < count; ++i) {
      if(allEqual(edges[i], element)) return true;

      edge reverseEdge(element.destination, element.departure, element.cost);
      if(allEqual(edges[i], reverseEdge)) return true;
      //방향 그래프가 아니므로 출발지와 도착지가 뒤바뀌어도 같은 엣지로 침.
    }
    return false;
  }

  bool allEqual(edge e1, edge e2) {
    if(e1.departure == e2.departure && e1.destination == e2.destination && e1.cost == e2.cost) {return true;}
    //사실 코스트가 다르면 해당엣지에 수정을 가하는 식을 생각했는데 만들기 귀찮음.
    //그냥 개발자가 처음부터 잘 입력하기를 기대.
    return false;
  }

  friend int main();
  
  
  graph MST()
  {
    graph g(count);
    int currentIndex = 0;
    double minCost;
    int minIndex = 0;

    for (int i = 0; i < count; ++i){
      if(edges[i].departure == edges[currentIndex].departure
      || edges[i].destination == edges[currentIndex].departure) { //거꾸로된 엣지 고려
        if(minCost > edges[i].cost) {
          minCost = edges[i].cost;
          minIndex = i;
        }
      }
    }
    visited[currentIndex] = true; //여기서부터 코드가 너무 복잡해져서 포기.
    currentIndex = minIndex;
    g.push(edge(edges[minIndex].departure, edges[minIndex].destination, minCost));

    return g;
  }

};  