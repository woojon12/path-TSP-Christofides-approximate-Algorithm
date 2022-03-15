#pragma once
#include "Cost.h"
#include <iostream>
#include <vector>
#include <set>
#include <list>
using namespace std;

//같은 선이 여러개 중복되는 걸 허용한 그래프로 만듦

class vertex {
  int number;
  bool visited;
  int degree; //그래프에서 성공적인 push가 일어날 때마다 해당 정점의 차수 증가시키기

  friend class graph;

  public:
  vertex() : visited(false), degree(0) {}
  vertex(int number)
  : number(number), visited(false), degree(0) {}

  friend bool operator==(const vertex&, const vertex&);
  friend bool operator!=(const vertex&, const vertex&);
  friend bool operator<(const vertex&, const vertex&);
  friend ostream& operator<<(ostream& o, const vertex&);
};

bool operator==(const vertex& v1, const vertex& v2)
{return v1.number == v2.number;}

bool operator!=(const vertex& v1, const vertex& v2)
{return v1.number != v2.number;}

bool operator<(const vertex& v1, const vertex& v2)
{return v1.number < v2.number;}

ostream& operator<<(ostream& o, const vertex& v){
  o << v.number;
  return o;
}

class edge {
  cost weight;
  vertex departure, destination;

  friend class graph;

  bool isVertexSame(edge e) {
    return departure == e.departure
    && destination == e.destination;
  }

public:
  edge() : weight() {}
  edge(vertex departure, vertex destination, cost weight)
  : departure(departure), destination(destination), weight(weight)
  {}

  friend int operator==(edge, edge);

  edge reverse() {
    return edge(destination, departure, weight);
  }
};

int operator==(edge e1, edge e2)
{
  if(e1.departure == e2.departure
  && e1.destination == e2.destination) {
    if(e1.weight == e2.weight) return 1;
    return -1;
  }
  if(e1.departure == e2.destination
  && e1.destination == e2.departure) {
    if(e1.weight == e2.weight) return 1;
    return -1;
  }

  return 0;
}


class graph {
  vector<edge> Edges;
  set<vertex> Vertices;

  public:
  graph(){}
  graph(int capacity){Edges.reserve(capacity);}
  graph(int size, int **arrayGraph) {
      Edges.reserve(size * (size - 1));

      //들어오는 배열이 무조건 정사각형이라 상정한다.
      //동적 배열의 메모리 크기를 아는 함수 malloc.h 의 _msize 함수가 있는데
      //어째선지 여기선 동작하지 않음.

      for(int i = 0; i < size; ++i) {
        for(int j = 0; j < size; ++j) {
          if(i != j) push(edge(i, j, arrayGraph[i][j]));
        }
      }
  }

  graph(vector<edge> Edges) { //수정사항 : degree를 위해 그냥 일일히 push하는 걸로 바꿈
    for(edge e : Edges) {
      this->push(e);
    }
  }

  graph& push(edge element) {
    /* 수정사항 : 중복 허용을 위해 아예 무조건 넣음
    enum {onlyCostDiffrent = -1, noExist = 0, alreadyExist = 1};

    switch(alreadyExistORonlyCostDiff(element)) {
      case alreadyExist:
      return *this;

      case onlyCostDiffrent: {
        int i = findIndex(element);
        Edges[i].weight = element.weight;
      return *this;
      }

      case noExist: {
      Edges.push_back(element);
      Vertices.insert(element.departure);
      Vertices.insert(element.destination);
      return *this;
      }
      default:
      cout << "What happended" << endl;
      return *this;
    }
    */

    Edges.push_back(element);
    Vertices.insert(element.departure);
    Vertices.insert(element.destination);

    const_cast<vertex&>(*Vertices.find(element.departure)).degree++;
    const_cast<vertex&>(*Vertices.find(element.destination)).degree++;
    return *this;
  }

  int findIndex(edge element) {
    int index = 0;

    for (edge e : Edges) {
      if(e.isVertexSame(element)) return index;
      ++index;
    }

    return -1; //혹시 없으면 -1 반환
  }

  cost findWeight(vertex v1, vertex v2) {
    return Edges[findIndex(edge(v1, v2, cost()))].weight;
  }

  cost findWeight_undirected(vertex v1, vertex v2) {
    int v1v2 = findIndex(edge(v1, v2, 0));
    if(v1v2 != -1)return Edges[v1v2].weight;
    
    int v2v1 = findIndex(edge(v2, v1, 0));
    if(v2v1 != -1)return Edges[v2v1].weight;

    return cost();
  }

  private:
  int alreadyExistORonlyCostDiff(edge element) {
    for(edge e : Edges)
      if (e == element) return e == element;
    
    return 0;
  }

  void all_vertices_unvisiting() {
    for(set<vertex>::iterator itr = Vertices.begin(); itr != Vertices.end(); ++itr)
      const_cast<vertex&>(*itr).visited = false;
  }

  public:
  graph MST() { //Minimum Spanning Tree
    graph exploreGraph;
    graph resultGraph;
    set<vertex> visitedVertices;
    
    visitedVertices.insert(this->Edges[0].departure);
    //탐색 시작점은 그냥 대충 정하기
    
    int theNumberOfVertices = allVerticesCount();

    while(resultGraph.Edges.size() < theNumberOfVertices - 1) {
      edge minEdge;
      exploreGraph.Edges = 
      this->theEdgesIncludingTheseVertices(visitedVertices);

      subtractGraph(exploreGraph, resultGraph);

      for(edge e : exploreGraph.Edges) {
        if(!(isBothEndsVertices(e, visitedVertices))
        && minEdge.weight > e.weight)
          minEdge = e;
      }

      visitedVertices.insert(minEdge.departure);
      visitedVertices.insert(minEdge.destination);
      resultGraph.push(minEdge);
    }

    return resultGraph;
  }


  //일단 눈에 보이는 아무 간선 하나 잡고 그 간선의 출발점으로 다시 돌아올 때까지
  //여행을 계속하다가 돌아오면 그 경로를 하나의 순회 덩어리로 저장함
  //이미 사용된 건 제외하고 남은 간선에서 아무거나 하나 잡고 반복
  //모든 간선을 다 썼다면 발견한 순회들을 알아서 잘 합침
  graph EulerianCircuit() {
    for(vertex v : Vertices)
      if(v.degree & 1) {cout << "오일러 '회로' 미존재" << endl; return *this;}
    
    vector<list<edge>> partial_circuits;
    vector<edge> exploreEdges = this->Edges;

    size_t push_count = 0;
    
    for(int i = 0; true; ++i) {
    
    partial_circuits.push_back(list<edge>());
    partial_circuits[i].push_back(exploreEdges[0]); ++push_count;
    vertex circuit_desti = exploreEdges[0].departure;
    vertex current = exploreEdges[0].destination;

    exploreEdges.erase(exploreEdges.begin());

    while (current != circuit_desti) {
    for(vector<edge>::iterator itr = exploreEdges.begin();
    itr != exploreEdges.end(); ++itr) {
      if(itr->departure == current)
      {current = itr->destination; ++push_count;
      partial_circuits[i].push_back(edge(itr->departure, itr->destination, itr->weight)); 

      exploreEdges.erase(itr--); break;}
      if(itr->destination == current)
      {current = itr->departure; ++push_count;
      partial_circuits[i].push_back(edge(itr->destination, itr->departure, itr->weight));
      exploreEdges.erase(itr--); break;}
    }
    //TODO : 만약 엣지 미발견 시 오일러 회로는 존재하지 않음
    //      제아무리 모든 정점의 차수가 짝수여도 잘린 그래프면 안되니까.
      
    //아 생각해보니까 1-2 다음에 또 1-2 엣지가 있으면 이게 1-2-1 인지 2-1-2인지 어케 앎
    //어쩔 수 없이 그냥 순서맞게 변형해서 넣기

    }
      
    if(push_count == Edges.size()) {
      list_Eulerian_sort(partial_circuits); //발견한 순회들을 알아서 잘 합치기
      vector<edge> result;
      for(list<edge>::iterator itr = partial_circuits[0].begin();
      itr != partial_circuits[0].end(); ++itr)
        result.push_back(*itr);

      return graph(result);
    }
    }
  }

  void list_Eulerian_sort(vector<list<edge>>& partial_circuits)
  {
    int visit_bit = 1;

    for(list<edge>::iterator itr = partial_circuits[0].begin();
    itr != partial_circuits[0].end(); ++itr) {
      
      for(vector<list<edge>>::iterator vitr = partial_circuits.begin();
      vitr != partial_circuits.end(); ++vitr) {
        size_t search = vitr - partial_circuits.begin();
        if(1 << search & visit_bit) continue; //already consumed list
        for(list<edge>::iterator litr = vitr->begin();
        litr != vitr->end(); ++litr) {

          if(itr->departure == litr->departure) {
            list<edge>::iterator proper_end = --vitr->end();
            partial_circuits[0].splice(itr, *vitr, litr, vitr->end());
            partial_circuits[0].splice(++proper_end, *vitr);
            visit_bit += 1 << search;
            itr = litr; //이렇게 itr을 잘라붙이기 한 덩어리의 첫 장소로 바꿔야 할 듯?
            break;
          }
        }
      }
    }
  }

  void subtract_explore_range(vector<edge>& exploreEdges, const list<edge>& resultList) {
    for(vector<edge>::iterator expitr = exploreEdges.begin();
    expitr != exploreEdges.end(); ++expitr)
      for(list<edge>::const_iterator resitr = resultList.begin();
      resitr != resultList.end(); ++resitr)
        if(*expitr == *resitr) {exploreEdges.erase(expitr--); break;}
        //위에서 바로 ++expitr 해버리니 --를 한 번 해줘야할 듯
  }

  vector<edge> theEdgesIncludingTheseVertices(set<vertex> V)
  {
    vector<edge> resultEdges;

    for(edge e : Edges) {
      for(vertex v : V) {
        if(e.departure == v || e.destination == v)
          resultEdges.push_back(e);
      }
    }

    return resultEdges;
  }

  void subtractGraph(graph& exploreGraph, graph& resultGraph) {
    for(vector<edge>::iterator itr = exploreGraph.Edges.begin();
      itr != exploreGraph.Edges.begin(); ++itr) {
        vector<edge>::iterator itrR;
        for(itrR = resultGraph.Edges.begin();
        itrR != resultGraph.Edges.end(); ++itrR) {
          if(*itr == *itrR) {itr = --exploreGraph.Edges.erase(itr); break;}
        }
    }
  }

  int allVerticesCount()
  {
    //set 사용법 정리 : https://blockdmask.tistory.com/79
    return Vertices.size();
  }

  bool isBothEndsVertices(edge e, set<vertex> V) {
    if(V.find(e.departure) != V.end()
    && V.find(e.destination) != V.end()) return true;
    return false;
  }

  void showEdges() {
    for(edge& e : Edges) {
      cout << "(" << e.departure.number << ", " << e.destination.number << ") - "
      << e.weight << endl;
    }
  }

  void showTheEdge(int index) {
    cout << "(" << Edges[index].departure.number << ", " << Edges[index].destination.number << ") - " << Edges[index].weight.weight << endl;
  }

  const vertex& vtxat(int index) {
    set<vertex>::iterator itr = Vertices.begin();
    for(int i = 0; i < index; ++i) {
      ++itr;
    }

    return *itr;
  }

  int vertexindex(const vertex& v)
  {
    int N = Vertices.size();
    for(int i = 0; i < N; ++i)
      if(vtxat(i) == v) return i;
    
    return -1;
  }

  graph degree_correcting_min_perfect_matching(vertex, const graph&);
  graph Eulerian_trail(vertex start);
  void shortcut_TSP_print(const graph& initial_graph);
  void debug_all_degree();
  void debug_all_vertices_v(const vector<vertex>&);
  void debug_all_vertices_s(const set<vertex>&);
  void debug_all_edges(vector<edge>&);

private:
  edge min_edge_from_to_vv(const vector<vertex>&, const graph& initial_graph, int from, int to);
  edge min_edge_vfrom_vto(const graph& initial_graph, vertex from, vertex to);
  vector<edge> min_matching(const vector<vertex>& matching_vertices, const graph& initial_graph, int visit_bit, vector<vector<edge>>& ve, vector<edge>& req, cost& min_cost, cost& inf_count, int level = 0);
  int from_right_to_left_nth_zerobit(const int& visit_bit, const size_t& size, int n);
  int the_number_of_zero_bit(const int& bit, const size_t& search_range);
  vector<edge>::iterator longest_edge_at_with(vector<edge> E, vertex involve);
  //direc::edge min_edge_from_to(direc::vertex from, direc::vertex to); 아 vertex도 direc으로 들어가네
};


//잘못된 차수를 갖는 정점끼리 매칭한 간선까지 더해서 반환하는 함수.
//시작지점만 정해진 걸로 상정하고 만듦
//아직 안만든 오일러 '경로' 찾기 전 과정임
//즉 all짝수 차수가 되면...일단은 임시로 제일 긴 간선 하나를 지우는 걸로
//근데 같은 길이인 간선도 많아서 사실 하나씩 지워본 경우의 수를 다 탐색하는 게 맞음
//근데 사실 그렇게 따지면 애초에 spanning tree 찾을 때 minimum 하나로 퉁 치는 것도 불완전한건데
graph graph::degree_correcting_min_perfect_matching(vertex start, const graph& initial_graph) {
  graph result = this->Edges;
  vector<vertex> wrong_degree_vtx;
  for(set<vertex>::iterator itr = Vertices.begin(); itr != Vertices.end(); ++itr) {
    int degree_check = itr->degree;
    if(*itr == start) ++degree_check;
    if(degree_check & 1) wrong_degree_vtx.push_back(*itr);
  }
  //start 지점만 정했다보니 wrong정점 개수가 홀수개로 나와서
  //한 정점 제외를 한 모든 경우의 수 각각의 minumum tree 중에서 또 minimum을 찾아야 함
  //DONE : 제외하는 게 시작점이면 시작점에 붙은 간선 중 가장 긴 거 지우는 작업을 하고 비교
  //        아 근데 그러면 아예 모든 간선의 가중치 총합으로 비교해야 되네
  //        귀찮으니까 그냥 스타트 지점이 걸린 경우 추가선 비용 합 - 뺀 선 비용으로 계산 후 비교
  
  //흠 혹시 드는 생각인데 반환으로 답을 쌓아가는 재귀함수가
  //필수불가결은 아니어도 더 효율적이긴 하려나
  //답을 재귀함수 끝에서 완성하려면 자꾸 인자를 전달해야하잖아
  //근데 반환쌓기는 인자를 덜 전달하니 더 좋은거 아닌가
  vector<edge> min_matching_edges(wrong_degree_vtx.size() / 2);
  vector<edge> req; //있으면 더 좋을 수도 있는 간선들 요청할 거 저장할 공간
  vector<edge>::iterator erase_start_itr;
  cost previous_min;
  cost previous_inf_count;
  bool ntp_start_min = false; //not take part (참여하지 않은)

  cout << "debug : wrong_degree_vtx list" << endl;
  for(const vertex& v : wrong_degree_vtx) cout << v << endl;
  
  for(vector<vertex>::iterator itr = wrong_degree_vtx.begin();
    itr != wrong_degree_vtx.end(); ++itr) { //여기서 정해지는 itr은 제외하는 거 정하는 거
    int visit_bit = 1 << (itr - wrong_degree_vtx.begin());
      //근데 이제와서 드는 생각인데 나 정점 개수 최소 40개쯤은 될텐데
      //32개의 정점만 받는 4비트 메모리는 좀 부족하지 않나?
      //그래서 8비트(size_t)로 가서 64개까지도 받는 걸 만들어야 하나
    vector<vector<edge>> ve(2); //어디까지나 함수 인자가 레퍼런스라서 임시 생성한 것들
    ve[0] = vector<edge>((wrong_degree_vtx.size() - 1) / 2); //원래 계산은 이건데
    ve[1] = vector<edge>(wrong_degree_vtx.size() / 2); //어차피 결과가 정수니깐 이래도됨
    cost tempcost(true);
    cost temp_inf_count(true);
      //debug
    for(edge& e : ve[0]) {
      cout << "아무것도 안한 상태 (" << e.departure << ", " << e.destination << ") - " << e.weight << endl;
    }

    vector<edge> temp_matching = min_matching(wrong_degree_vtx, initial_graph, visit_bit, ve, req, tempcost, temp_inf_count);

    //debug
    cout << "↓후보 탐색 후" << endl;
    cout << "(" << visit_bit << ")------------" << endl;
    debug_all_edges(temp_matching);
    cout << "----------------" << endl;
      
      cost temp_total_cost = 0;
    for(vector<edge>::iterator itr = temp_matching.begin();
      itr != temp_matching.end(); ++itr)
      temp_total_cost = itr->weight + temp_total_cost;
      
    if(*itr == start) {
      erase_start_itr = longest_edge_at_with(result.Edges, start);
      temp_total_cost = temp_total_cost - erase_start_itr->weight;
    }
      
    if(previous_min >= temp_total_cost && previous_inf_count > temp_inf_count) {
      previous_min = temp_total_cost;
      min_matching_edges = temp_matching;
      previous_inf_count = temp_inf_count;
      
      if(*itr == start)
        ntp_start_min = true;
      else ntp_start_min = false;
    }
  }

  for(const edge& e : req)
    cout << "(" << e.departure << ", " << e.destination << ")" << "edge better define" << endl;

  cout << "----추가되는 간선----" << endl;
  for(vector<edge>::iterator itr = min_matching_edges.begin();
    itr != min_matching_edges.end(); ++itr) {
    cout << "(" << itr->departure << ", " << itr->destination << ") - " << itr->weight << endl;
    result.push(*itr);
  }
  cout << "--------------------" << endl;

  if(ntp_start_min) result.Edges.erase(erase_start_itr);

  return result;
}

edge graph::min_edge_from_to_vv(const vector<vertex>& vts, const graph& initial_graph, int from, int to) {
  edge minedge(vts[from], vts[to], cost());

  for(const edge& e : initial_graph.Edges) {
        if(e.departure == vts[from]
          && e.destination == vts[to]) {
          if(minedge.weight > e.weight) {minedge = e;}
        }
        if(e.destination == vts[from] //무방향 그래프 기준
          && e.departure == vts[to]) {
          if(minedge.weight > e.weight) {minedge = e;}
        }
  }
  
  return minedge;
}

vector<edge> graph::min_matching(const vector<vertex>& matching_vertices, const graph& initial_graph, int visit_bit, vector<vector<edge>>& ve, vector<edge>& req, cost& min_cost, 
cost& inf_count, int level) {
  const size_t& vsize = matching_vertices.size();
  int num_of_zerobit = the_number_of_zero_bit(visit_bit, vsize);
  
  for(int j = 0; j < num_of_zerobit - 1; ++j) {  
  int current = from_right_to_left_nth_zerobit(visit_bit, vsize, j);
  int tmp_vb = visit_bit;
  tmp_vb += 1 << current;
  
  for(int i = j; i < num_of_zerobit - 1; ++i) {
    int tmp2_vb = tmp_vb;
    int desti = from_right_to_left_nth_zerobit(tmp2_vb, vsize, i);
    tmp2_vb += 1 << desti;
    ve[1][level] = min_edge_from_to_vv(matching_vertices, initial_graph, current, desti);
    
    //DONE : 만약 여기서 edge(0,0,cost()) 가 나오면...요청해야함
    //근데 0, 0으로 하면 명백하게 오류나니깐
    //일단 거리만 모르게 from(current)과 to(desti)로 하는 게 나을 듯
    const edge& request = ve[1][level];
    if(request.weight == cost()) {
      bool already_exist = false;
      for(const edge& e : req)
        if(e.departure == request.departure
          && e.destination == request.destination) {
          already_exist = true;
          break;
        }

      if(!already_exist) req.push_back(request);
    }

    if(tmp2_vb == ((1 << vsize) - 1)) {//is all bit true
      cost total_cost = 0;
      cost current_inf_count = 0; //inf 개수가 되도록 적은걸로 선정할거임
      for(int i = 0; i < vsize / 2; ++i) {
        if(ve[1][i].weight == cost(true)) {++current_inf_count.weight; continue;}
        total_cost = total_cost + ve[1][i].weight;
      }
      if(min_cost >= total_cost && inf_count > current_inf_count) {
        ve[0] = ve[1]; //인덱스 0번 자리가 최종결과 넣는 곳. 1번자리는 임시자리임
        min_cost = total_cost;
        inf_count = current_inf_count;
      }
      continue;
    }
    
    min_matching(matching_vertices, initial_graph, tmp2_vb, ve, req, min_cost, inf_count, level + 1);
  }
  }

  return ve[0];
}

int graph::from_right_to_left_nth_zerobit(const int& visit_bit, const size_t& size, int n){
  int count = 0;
  for(int firstzero = 0; firstzero < size; ++firstzero) {
    if((visit_bit & (1 << firstzero)) == 0) {
        if(count++ == n) return firstzero;
    }
  }
  //cout << "visit bit : " << visit_bit << " 에서" << n << "번 무시하여 -1행" << endl;
  //getchar();
  return -1;
}

int graph::the_number_of_zero_bit(const int& bit, const size_t& search_range) {
  int count = 0;

  for(int i = 0; i < search_range; ++i) {
    if(!(bit & (1 << i))) ++count;
  }
  
  return count;
}

void graph::debug_all_degree() {
  for(set<vertex>::iterator itr = Vertices.begin(); itr != Vertices.end(); ++itr) {
    cout << *itr << "의 차수 : " << itr->degree << endl;
  }
}

void graph::debug_all_edges(vector<edge>& ve) {
  for(vector<edge>::iterator itr = ve.begin(); itr != ve.end(); ++itr) {
    cout << "(" << itr->departure << ", " <<
      itr->destination << ") - " << itr->weight << endl;
  }
}

void graph::debug_all_vertices_v(const vector<vertex>& vts) {
  cout << "정점 출력!" << endl;
  for(vector<vertex>::const_iterator itr = vts.begin(); itr != vts.end(); ++itr) {
    cout << itr->number << endl;
  }
}

void graph::debug_all_vertices_s(const set<vertex>& vts) {
  cout << "정점 출력!" << endl;
  for(set<vertex>::iterator itr = vts.begin(); itr != vts.end(); ++itr) {
    cout << itr->number << endl;
  }
}

graph graph::Eulerian_trail(vertex start) {
  //반드시 홀수 차수 정점이 두 개만 있다고 상정
  if(((Vertices.find(start.number))->degree & 1) == 0) {cout << "오일러 경로 없음!(start)" << endl; return *this;}
  
  vertex second_oddv;
  
  set<vertex>::iterator startitr;
  for(set<vertex>::iterator itr = Vertices.begin(); itr != Vertices.end(); ++itr) {
    if(itr->degree & 1) {
      if(*itr != start) second_oddv = *itr;
      else startitr = itr;
    }
  }
  //TODO : 만약 홀수 차수가 3개 이상이라면 또 오일러 경로 없다고 출력
  
  graph temp = this->Edges;

  temp.push(edge(start, second_oddv, 0)); //어차피 자를 간선이라 가중치는 상관없음
  
  temp = temp.EulerianCircuit();

  graph result;
  
  vector<edge>::iterator cutitr = temp.Edges.begin();
  for(; cutitr != temp.Edges.end(); ++cutitr) {
    if((*cutitr == edge(start, second_oddv, 0)) == 1) {break;}
  }

  cout << "↓----정렬 전 회로----↓" << endl;
  for(edge e : temp.Edges) { //debug
    cout << "-(" << e.departure << ", " << e.destination << ") - " << e.weight << endl;
  }
  cout << "↑----정렬 전 회로----↑" << endl;
  
  //오일러 경로 제작과정에서 엣지 순서가 뒤바뀔 수 있는데 그렇게 되면 지정한 스타트 기준으로 경로가 거꾸로 나옴
  if(cutitr->departure == start) {
    vector<edge>::reverse_iterator cutitr_r = temp.Edges.rbegin();
    for(; cutitr_r != temp.Edges.rend(); ++cutitr_r) {
      if((*cutitr_r == edge(start, second_oddv, 0)) == 1) {break;}
    }
    for(vector<edge>::reverse_iterator itr = ++cutitr_r; itr != temp.Edges.rend(); ++itr)
      result.push(itr->reverse());

    --cutitr_r;
    
    for(vector<edge>::reverse_iterator itr = temp.Edges.rbegin(); itr != cutitr_r; ++itr)
      result.push(itr->reverse());

    return result;
  }
  
  if(cutitr == temp.Edges.begin()) return temp;
  //바로 앞에 있으면 바로 답임. 사실 이거 안해도 알아서 되는데 좀 빨리 리턴하기 위함.
  
  for(vector<edge>::iterator itr = ++cutitr; itr != temp.Edges.end(); ++itr)
    result.push(*itr);

  --cutitr;
  
  for(vector<edge>::iterator itr = temp.Edges.begin(); itr != cutitr; ++itr)
    result.push(*itr);

  return result;
}

void graph::shortcut_TSP_print(const graph& initial_graph) {
  //첫 엣지의 시작점을 무조건 출발점으로 상정하고 출력
  set<vertex> visited;
  bool shortcutting = false;
  vertex shortcut_departure;
  vector<edge> shortcut;
  cost total_weight = 0;
  
  visited.insert(this->Edges[0].departure);
  cout << Edges[0].departure;
  
  for(vector<edge>::iterator itr = Edges.begin(); itr != Edges.end(); ++itr) {
    //cout << "//" <<  itr - Edges.begin() + 2 << "번 쨰 방문 지점 : (" << itr->departure << ", " << itr->destination << ") - " << itr->weight << endl;
    if(visited.find(itr->destination) == visited.end()) {
      visited.insert(itr->destination);
      cout << " - " << itr->destination;
      //cout << endl;
      //for(vertex v : visited) { cout << v << ", " << endl;}
      if(shortcutting) { //만약 지금 shortcut 하는 도중이라면
        shortcutting = false;
        edge shortcut_edge = min_edge_vfrom_vto(initial_graph, shortcut_departure, itr->destination);
        if((shortcut_edge.weight == cost()))
          shortcut.push_back(edge(shortcut_departure, itr->destination, cost()));
        else {total_weight = total_weight + shortcut_edge.weight;}
      }
      else {
        if(itr->weight == cost(true))
          shortcut.push_back(edge(itr->departure, itr->destination, cost()));
        else
          total_weight = total_weight + itr->weight;
      }
    }
      
    else {
      if(!shortcutting) {
        shortcut_departure = itr->departure;
        shortcutting = true;
      }
    }
  }

  cout << endl << "총 거리 : " << total_weight;

  if(shortcut.size() != 0) {
    cout << "+α" << endl << "---α에 해당하는 임시간선---" << endl;
    for(edge e : shortcut) {
      cout << "(" << e.departure << ", " << e.destination << ") - "
        << e.weight << endl;
    }
  }  
  else {cout << endl;}
}

edge graph::min_edge_vfrom_vto(const graph& initial_graph, vertex from, vertex to) {
 edge minedge(from, to, cost());

  for(edge e : initial_graph.Edges) {
        if(e.departure == from
          && e.destination == to) {
          if(minedge.weight > e.weight) {minedge = e;}
        }
        if(e.destination == from //무방향 그래프 기준
          && e.departure == to) {
          if(minedge.weight > e.weight) minedge = e;
        }
  }

  return minedge; 
}

vector<edge>::iterator graph::longest_edge_at_with(vector<edge> E, vertex involve) {
  edge max_edge(0, 0, cost(false));
  vector<edge>::iterator max_itr;
  
  for(vector<edge>::iterator itr = E.begin(); itr != E.end(); ++itr)
    if(itr->departure == involve || itr->destination == involve) 
      if(max_edge.weight < itr->weight) {
        max_edge = *itr;
        max_itr = itr;
      }
  
  return max_itr;
}