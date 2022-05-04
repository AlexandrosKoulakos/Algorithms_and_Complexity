/*
 * Our algorithm is based on Union - Find data structure.
 * More specifically, the preprocessing consists of sorting
 * portals data in ascending order according to their weights and
 * finding all elements that are out of place as well as their final
 * position in the vector c. We store the out of place element
 * along with its final position in a pair and to store all possible pairs
 * we use a vector. The idea behind our algorithm is that in order for c
 * to be sorted, all elements that are out of place must communicate with
 * their final position (which means that they can be flipped directly or
 * indirectly). All elements that can be linked belong to the same set and
 * every time we meet a new portal that connects universes x and y,
 * we unite the sets in which x and y belong. We are done iff all out of
 * place elements belong to the same set with their positions (aka having
 * the same delegate), which is easily checked using find() function. In total,
 * N finds and M unions occur where each operation has amortised constant time,
 * due to Ackermann function, so the complexity of our algorithm is O(N + M).
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct portal
{
  int c1;
  int c2;
  int w;
};

struct subset
{
  int parent;
  int rank;
};

bool sort_by_weight (portal p1, portal p2)
{
    return (p1.w < p2.w);
}

/*
 * Finds the delegate of the class in which element x belongs.
 * For efficiency we use optimized path compression.
 */
int find_set(subset *subsets, int x)
{
  /* Find root and make root as parent of i
   * (path compression)
   */
  if (subsets[x].parent != x)
      subsets[x].parent = find_set(subsets, subsets[x].parent);

  return subsets[x].parent;
}

/*
 * Unites the classes in which elements x and y belong.
 * Depth of the tree is used as a rank heuristic.
 */
void union_set (subset *subsets, int x, int y)
{
  int delegate_x = find_set(subsets, x);
  int delegate_y = find_set(subsets, y);

  if (delegate_x != delegate_y)
  {
    if (subsets[delegate_x].rank < subsets[delegate_y].rank)
      subsets[delegate_x].parent = delegate_y;
    else if (subsets[delegate_x].rank > subsets[delegate_y].rank)
      subsets[delegate_y].parent = delegate_x;
    else
    {
      subsets[delegate_x].parent = delegate_y;
      (subsets[delegate_y].rank)++;
    }
  }
}

int main()
{
  int N, M, *c, result, pos = 0;
  portal *portals;
  subset *subsets;
  vector < pair< int, int > > out_of_place;

  scanf("%d", &N);
  scanf("%d", &M);

  c = (int *)malloc((N + 1) * sizeof(int));
  portals = (portal *)malloc(M * sizeof(portal));

  for (int i = 1; i <= N; i++)
    scanf("%d", &c[i]);

  for (int i = 0; i < M; i++)
  {
    scanf("%d", &portals[i].c1);
    scanf("%d", &portals[i].c2);
    scanf("%d", &portals[i].w);
  }

  subsets = (subset *)malloc((N + 1) * sizeof(subset));

  for (int i = 1; i <= N; i++)
  {
    subsets[i].parent = i;
    subsets[i].rank = 0;
  }

  if (N <= 100000 and M <= 100000)
    sort(portals, portals + M, sort_by_weight);

  for (int i = 1; i <= N; i++)
  {
    if (c[i] != i)
      out_of_place.push_back(make_pair(c[i], i));
      /*
       * c[i] indicates the element that is out of order
       * and i indicates its right position, therefore
       * it is clear that c[i] and i must communicate directly or indirectly.
       */
  }

  int size = out_of_place.size();

  for (int i = M - 1; i >= 0; i--)
  {
    union_set(subsets, portals[i].c1, portals[i].c2);

    bool found = true;
    /*
     * found = true -> all elements that are out of order
     * belong to the same set with their final positions,
     * therefore vector c can be sorted using portal[i].
     * found = false -> there is at least one out of order
     * element that does not belong to the same set with its
     * final position, therefore vector c cannot be sorted
     * using portal[i] as the narrowest portal.
     */

    for (int j = pos; j < size; j++)
    {
      if (find_set(subsets, out_of_place[j].first) != find_set(subsets, out_of_place[j].second))
      {
        found = false;
        pos = j;
        break;
      }
    }

    if (found)
    {
      result = portals[i].w;
      break;
    }
  }

  cout << result << endl;
}
