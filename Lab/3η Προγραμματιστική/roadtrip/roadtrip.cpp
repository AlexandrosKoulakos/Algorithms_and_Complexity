#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

/* Represents information about an edge.
 * First element of the pair denotes the vertex
 * to which the edge goes and the second element of the pair
 * denotes the weight of this edge.
 */
struct data
{
    vector<pair<int, int>> edge;
};

// To represent Disjoint Sets
struct DisjointSets
{
    int *parent, *rnk;
    int n;

    // Constructor.
    DisjointSets(int n)
    {
        // Allocate memory
        this->n = n;
        parent = new int[n + 1];
        rnk = new int[n + 1];

        // Initially, all vertices are in different sets and have rank 0.
        for (int i = 0; i <= n; i++)
        {
            rnk[i] = 0;

            // every element is parent of itself
            parent[i] = i;
        }
    }

    // Find the parent of a node 'u'
    // Path Compression
    int find(int u)
    {
        /* Make the parent of the nodes in the path
           from u--> parent[u] point to parent[u] */
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }

    // Union by rank
    void merge(int x, int y)
    {
        x = find(x), y = find(y);

        /* Make tree with smaller height
           a subtree of the other tree  */
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else // If rnk[x] <= rnk[y]
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

// Implementation of Kruskal algorithm
void Kruskal(vector<pair<int, pair<int, int>>> &edges, data *tree, int n)
{
    int edge_count = 0;

    // Sort edges in increasing order on basis of cost
    sort(edges.begin(), edges.end());

    // Create disjoint sets
    DisjointSets ds(n);

    // Iterate through all sorted edges
    vector<pair<int, pair<int, int>>>::iterator it;
    for (it = edges.begin(); it != edges.end(); it++)
    {
        if (edge_count >= n)
            break;

        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        // Check if the selected edge is creating
        // a cycle or not (Cycle is created if u
        // and v belong to same set)
        if (set_u != set_v)
        {
            // Current edge will be in the MST

            edge_count++;

            tree[u].edge.push_back(make_pair(v, it->first));
            tree[v].edge.push_back(make_pair(u, it->first));

            // Merge two sets
            ds.merge(set_u, set_v);
        }
    }
}

// Function to traverse the nodes using the Depth-First Search Traversal
void dfs_lca(int *level, int **dp, int **mx, data *v, int a, int par, int lev)
{
    dp[0][a] = par;
    level[a] = lev;
    for (int i = 0; i < v[a].edge.size(); i++)
    {
        // Condition to check if it's equal to its parent then skip
        if (v[a].edge[i].first == par)
            continue;
        mx[0][v[a].edge[i].first] = v[a].edge[i].second;

        // DFS Recursive Call
        dfs_lca(level, dp, mx, v, v[a].edge[i].first, a, lev + 1);
    }
}

// Function to find the ansector
void find_ancestor(int **dp, int **mx, int LG, int n)
{
    // Loop to set every 2^i distance
    for (int i = 1; i < LG; i++)
    {
        // Loop to calculate for
        // each node in the N-ary tree
        for (int j = 1; j <= n; j++)
        {
            dp[i][j] = dp[i - 1][dp[i - 1][j]];

            // Storing maximum edge
            mx[i][j] = max(mx[i - 1][j],
                           mx[i - 1][dp[i - 1][j]]);
        }
    }
}

// Find most costly edge among the paths a - LCA and LCA - b
int getMax(int *level, int **dp, int **mx, int a, int b)
{
    // Swaping if node a is at more depth
    // than node b because we will
    // always take at more depth
    if (level[b] < level[a])
        swap(a, b);

    int ans = 0;

    // Difference between the depth of
    // the two given nodes
    int diff = level[b] - level[a];
    while (diff > 0)
    {
        int log = log2(diff);
        ans = max(ans, mx[log][b]);

        // Changing Node B to its
        // parent at 2 ^ i distance
        b = dp[log][b];

        // Subtracting distance by 2^i
        diff -= (1 << log);
    }

    // Take both a, b to its
    // lca and find maximum
    while (a != b)
    {
        int i = log2(level[a]);

        // Loop to find the 2^ith
        // parent that is different
        // for both a and b i.e below the lca
        while (i > 0 && dp[i][a] == dp[i][b])
            i--;

        // Updating ans
        ans = max(ans, mx[i][a]);
        ans = max(ans, mx[i][b]);

        // Changing value to its parent
        a = dp[i][a];
        b = dp[i][b];
    }
    return ans;
}

// Function to compute the Least common Ansector
void compute_lca(int *level, int **dp, int **mx, data *v, int LG, int n)
{
    dfs_lca(level, dp, mx, v, 1, 0, 0);
    find_ancestor(dp, mx, LG, n);
}

int main()
{
    int N, M, Q, *level, **dp, **mx;
    const int LG = 20;

    vector<pair<int, pair<int, int>>> edges;
    vector<pair<int, int>> queries;
    data *tree;

    scanf("%d %d", &N, &M);

    tree = (data *)malloc(sizeof(data) * (N + 1));
    level = (int *)malloc(sizeof(int) * N);
    dp = (int **)malloc(sizeof(int *) * LG);
    mx = (int **)malloc(sizeof(int *) * LG);

    for (int i = 0; i < LG; i++)
    {
        // Declare a memory block of size N + 1
        dp[i] = (int *)malloc(sizeof(int) * (N + 1));
        mx[i] = (int *)malloc(sizeof(int) * (N + 1));
    }

    for (int i = 0; i < LG; i++)
    {
        for (int j = 1; j <= N; j++)
        {
            dp[i][j] = 0;
            mx[i][j] = 0;
        }
    }

    for (int i = 0; i < M; i++)
    {
        int v1, v2, w;

        scanf("%d %d %d", &v1, &v2, &w);

        edges.push_back(make_pair(w, make_pair(v1, v2)));
        edges.push_back(make_pair(w, make_pair(v2, v1)));
    }

    scanf("%d", &Q);

    for (int i = 0; i < Q; i++)
    {
        int v1, v2;

        scanf("%d %d", &v1, &v2);

        queries.push_back(make_pair(v1, v2));
    }

    Kruskal(edges, tree, N);

    compute_lca(level, dp, mx, tree, LG, N);

    for (int i = 0; i < Q; i++)
        printf("%d\n", getMax(level, dp, mx, queries[i].first, queries[i].second));

    return 0;
}
