#pragma once
#include "Cost.h"
#include <iostream>
#include <vector>
#include <set>
using namespace std;

class vertex {
  int number;
  bool visited;

  friend class graph;

  public:
  vertex() : visited(false) {}
  vertex(int number)
  : number(number), visited(false) {}

  friend bool operator==(vertex, vertex);
};

bool operator==(vertex v1, vertex v2)
{return v1.number == v2.number;}

class edge {
  cost weight;
  vertex departure, destination;

  friend class graph;
public:
  edge() : weight() {}
  edge(vertex departure, vertex destination, cost weight)
  : departure(departure), destination(destination), weight(weight)
  {}

  friend int operator==(edge, edge);
};

int operator==(edge e1, edge e2)
{
  if(e1.departure == e2.departure
  && e1.destination == e2.destination
  ||
    e1.departure == e2.destination
  && e1.destination == e2.departure) {
    if(e1.weight == e2.weight) return 1; 
    return -1; //-1의 반환은 cost(weight)만 다르다는 뜻
  }

  return 0;
}


class graph {
  vector<edge> Edges;

  public:
  graph(){}
  graph(int capacity){Edges.reserve(capacity);}

  graph& push(edge element) {
    enum {onlyCostDiffrent = -1, noExist = 0, alreadyExist = 1};

    switch(alreadyExistOronlyCostDiff(element)) {
      case alreadyExist:
      return *this;

      case onlyCostDiffrent: {
      int i = findIndex(element);
      Edges[i].weight = element.weight;
      }
      return *this;
      
      case noExist:
      Edges.push_back(element);
      return *this;

      default:
      cout << "What happended" << endl;
      return *this;
    }
  }

  private:
  int alreadyExistOronlyCostDiff(edge element) {
    for(edge e : Edges)
      if (e == element) return e == element;
    
    return 0;
  }

  int findIndex(edge element) {
    int index = 0;

    for (edge e : Edges) {
      if(e == element) return index;
      ++index;
    }

    return -1; //혹시 없으면 -1 반환
  }

  public:
  graph MST() { //Minimum Spanning Tree
    graph tempGraph;
    vertex current = tempGraph.Edges[0].departure; //탐색 시작점은 그냥 대충 정하기

    edge minEdge;

    do
    {
    for(edge e : this->Edges) {
      if(current == e.departure) { //출발지점 찾음
        if(minEdge.weight > e.weight) {
          minEdge = e;
        }
      }
      else if(current == e.destination) {
        if(minEdge.weight > e.weight) {
          minEdge = e;
        }
      }
    }
    
    tempGraph.push(minEdge);
    current.visited = true;

    if(current == minEdge.departure) current = minEdge.departure;
    else current = minEdge.destination;
    } while(allVertexVisited());
  }

  bool allVertexVisited()
  {
    //set 사용법 정리 : https://blockdmask.tistory.com/79
    set<vertex> V;

    for (edge e : Edges) {
      V.insert(e.departure);
      V.insert(e.destination);
    }
    
    for (vertex v : V) {
      if(v.visited == false) return false;
    }

    return true;
  }
};