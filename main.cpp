#include <iostream>
#include "Ovelapgraph.h"

int main()
{
  /*
  edge e1(1, 2, 5);
  edge e2(1, 4, 5);
  edge e3(1, 5, 4);
  //edge e3(2, 5, 4);
  
  edge e4(2, 3, 3);
  edge e5(2, 5, 2);
  
  edge e6(3, 6, 1);
  edge e7(4, 5, 2);
  edge e8(5, 6, 3);

  graph g(8);

  g.push(e1);
  g.push(e2);
  g.push(e3);
  g.push(e4);
  g.push(e5);
  g.push(e6);
  g.push(e7);
  g.push(e8);
*/

  
  graph g(40);
  
  g.push(edge(0, 1, 1));
    g.push(edge(1, 2, 1)); g.push(edge(1, 8, 3));
    g.push(edge(2, 3, 1));

    g.push(edge(0, 4, 1));
    //g.push(edge(1, ?, 1));
    g.push(edge(2, 5, 1));
    g.push(edge(3, 6, 1));

    g.push(edge(4, 5, 2));
    g.push(edge(5, 6, 1));

    g.push(edge(4, 7, 1));
    g.push(edge(5, 9, 1));
    g.push(edge(6, 10, 1));

    g.push(edge(7, 8, 1));
    g.push(edge(8, 9, 1));
    g.push(edge(9, 10, 1));
  
    g.push(edge(7, 11, 1));
    g.push(edge(8, 12, 1));
    g.push(edge(9, 13, 1));
    g.push(edge(10, 14, 1));
    
    g.push(edge(11, 12, 1));
    g.push(edge(12, 13, 1));
    g.push(edge(13, 14, 1)); //g.push(edge(13, 20, 3));
    
    g.push(edge(11, 15, 1)); g.push(edge(15, 16, 10));
    g.push(edge(12, 16, 1)); g.push(edge(16, 17, 5));
    g.push(edge(14, 17, 1));
    
    g.push(edge(15, 18, 1));
    g.push(edge(16, 19, 1));
    g.push(edge(17, 21, 1));
    
    g.push(edge(18, 19, 1));
    g.push(edge(19, 20, 1));
    g.push(edge(20, 21, 1)); g.push(edge(20, 24, 10));
    
    g.push(edge(18, 22, 1));
    g.push(edge(19, 23, 1));
    g.push(edge(21, 25, 1));

    g.push(edge(22, 23, 1));
    g.push(edge(23, 24, 1));
    g.push(edge(24, 25, 1));
  

  /*
  graph g;

  g.push(edge(0, 1, 1));
  g.push(edge(1, 2, 1));
  g.push(edge(2, 3, 1));
  g.push(edge(0, 4, 1));
  */

  //g.debug_all_degree();
  vertex start(4);

  cout << "---최소 스패닝 트리---" << endl;
  graph g1 = g.MST();
  g1.showEdges();

  cout << endl << "---홀수 차수 정점 최소 퍼펙트 매칭---" << endl;
  graph g2 = g1.degree_correcting_min_perfect_matching(start, g);
  g2.showEdges();

  //이제는 오일러 경로를 구해야함. 회로는 또 무한루프임
  cout << endl << "---오일러 경로---" << endl;
  graph g3 = g2.Eulerian_trail(start);
  cout << endl << "↓----정렬한 후----↓" << endl;
  g3.showEdges();
  cout << endl << "↑----정렬한 후----↑" << endl;

  cout << endl << "---TSP 경로---" << endl;
  g3.shortcut_TSP_print(g);

  cout << "program end" << endl;

  return 0;
}