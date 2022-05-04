#include <iostream>
#include <vector>

using namespace std;

struct merchant
{
  vector < pair < int, unsigned long int > > armor;
  vector < pair < int, unsigned long int > > sword;
  vector < pair < int, unsigned long int > > shield;
};

int min(int a, int b)
{
  if (b < a)
    return b;
  else
    return a;
}

unsigned long int min(unsigned long int a, unsigned long int b)
{
  if (b < a)
    return b;
  else
    return a;
}

/*
 * Returns the total number of units of an item
 * that can be bought from the same merchant.
 */
int get_max_quantity(vector < pair < int, unsigned long int > > v)
{
  int q = 0;

  for (int i = 0; i < v.size(); i++)
    q += v[i].first;

  return q;
}

/*
 * knapsack implents arr(i, w), which describes
 * the minimum cost for the purchase of w units
 * of an item (A or B or C) from the same merchant
 * (1 or 2 or 2) who makes i offers.
 */
unsigned long int * knapsack(vector < pair < int, unsigned long int > > v, int q)
{
  int v_size = v.size();
  /*
   * Variable w stores the total number of units of
   * an item that can be bought from the same merchant
   */

  unsigned long int **arr = (unsigned long int **)malloc(sizeof(unsigned long int *) * (v_size + 1));
  // arr has (v_size + 1) rows

  for (int i = 0; i <= v_size; i++)
    arr[i] = (unsigned long int *)malloc(sizeof(unsigned long int) * (q + 1));
  // arr has (w + 1) columns

  for (int i = 0; i <= v_size; i++)
    arr[i][0] = 0;

  for (int i = 1; i <= q; i++)
    arr[0][i] = 0;

  for (int i = 1; i <= v_size; i++)
  {
    int quantity = v[i - 1].first;
    unsigned long int value = v[i - 1].second;

    for (int j = 1; j <= q; j++)
    {
      /*
       * Item (quantity, value), which is equivalent to
       * offer i, is sufficient for the purchase of j units.
       */
      if (j <= quantity)
      {
        /*
         * Unable to purchase j units using only the
         * first (i - 1) offers. Then, those units
         * can be purchased by using offer i alone.
         */
        if (arr[i - 1][j] == 0)
          arr[i][j] = value;
        /*
         * If the first (i - 1) offers suffice for the
         * purchase of j units, then we have the options
         * to use these offers or use offer i. Therefore,
         * we have to select the minimum cost of those
         * two options.
         */
        else
          arr[i][j] = min(arr[i - 1][j], value);
      }
      /*
       * Item (quantity, value), which is equivalent to
       * offer i, is NOT sufficient for the purchase of j units.
       * Therefore, we need to consider those offers that minimise
       * the cost of purchasing (j - quantity) units.
       */
      else
      {
        /*
         * First (i - 1) offers do NOT suffice for the purchase
         * of j units. Therefore, we have to check whether those
         * offers suffice for the purchase of (j - quantity) units.
         */
        if (arr[i - 1][j] == 0)
        {
          /*
           * First (i - 1) offers do NOT suffice for the purchase
           * of (j - quantity) units. Therefore, first i offers,
           * in general, are NOT sufficient for the purchase of
           * j units.
           */
          if (arr[i - 1][j - quantity] == 0)
            arr[i][j] = 0;
          else
            /*
             * First (i - 1) offers suffice for the purchase
             * of (j - quantity) units.
             */
            arr[i][j] = arr[i - 1][j - quantity] + value;
        }
        /*
         * First (i - 1) offers suffice for the purchase
         * of j units.
         */
        else
        {
          /*
           * First (i - 1) offers do NOT suffice for the purchase
           * of (j - quantity) units.
           */
          if (arr[i - 1][j - quantity] == 0)
            arr[i][j] = arr[i - 1][j];
          /*
           * First (i - 1) offers suffice for the purchase
           * of (j - quantity) units. Then, we have the options
           * to use these offers alone for the purchase of j
           * units or use these offers for the purchase of
           * (j - quantity) units along with offer i for the
           * purchase of quantity units. So, we have to select
           * the minimum cost of those two options.
           */
          else
            arr[i][j] = min(arr[i - 1][j], arr[i - 1][j - quantity] + value);
        }
      }
    }
  }

  return arr[v.size()];
}

/*
 * Returns the minimum cost for the purchase
 * of all possible equipment sets from the
 * same merchant.
 */
unsigned long int * price_full_set(unsigned long int *prices_A, unsigned long int *prices_B, unsigned long int *prices_C, int quantity)
{
  unsigned long int *prices = (unsigned long int *)malloc(sizeof(unsigned long int) * (quantity + 1));

  for (int i = 0; i <= quantity; i++)
    prices[i] = prices_A[i] + prices_B[i] + prices_C[i];

  return prices;
}

int main()
{
  int N, M;
  merchant *merch = (merchant *)malloc(4 * sizeof(merchant));

  scanf("%d" "%d", &N, &M);

  for (int i = 0; i < M; i++)
  {
    char item;
    int m, A;
    unsigned long int P;

    scanf("%d %c %d %lu", &m, &item, &A, &P);

    switch (item)
    {
      case 'A':
        merch[m].armor.push_back(make_pair(A, P));
        break;
      case 'B':
        merch[m].sword.push_back(make_pair(A, P));
        break;
      case 'C':
        merch[m].shield.push_back(make_pair(A, P));
        break;
    }
  }

  int max_quantity_1A = get_max_quantity(merch[1].armor);
  int max_quantity_1B = get_max_quantity(merch[1].sword);
  int max_quantity_1C = get_max_quantity(merch[1].shield);
  int max_quantity_2A = get_max_quantity(merch[2].armor);
  int max_quantity_2B = get_max_quantity(merch[2].sword);
  int max_quantity_2C = get_max_quantity(merch[2].shield);
  int max_quantity_3A = get_max_quantity(merch[3].armor);
  int max_quantity_3B = get_max_quantity(merch[3].sword);
  int max_quantity_3C = get_max_quantity(merch[3].shield);

  // maximum number of full equipment sets that can be bought from merchant 1
  int q1 = min(max_quantity_1A, min(max_quantity_1B, max_quantity_1C));

  // maximum number of full equipment sets that can be bought from merchant 2
  int q2 = min(max_quantity_2A, min(max_quantity_2B, max_quantity_2C));

  // maximum number of full equipment sets that can be bought from merchant 3
  int q3 = min(max_quantity_3A, min(max_quantity_3B, max_quantity_3C));

  // Not enough equipment sets
  if (q1 + q2 + q3 < N)
  {
    printf("%d\n", -1);
    return 0;
  }
  else
  {
    unsigned long int *prices_1A = knapsack(merch[1].armor, max_quantity_1A);
    unsigned long int *prices_1B = knapsack(merch[1].sword, max_quantity_1B);
    unsigned long int *prices_1C = knapsack(merch[1].shield, max_quantity_1C);
    unsigned long int *prices_2A = knapsack(merch[2].armor, max_quantity_2A);
    unsigned long int *prices_2B = knapsack(merch[2].sword, max_quantity_2B);
    unsigned long int *prices_2C = knapsack(merch[2].shield, max_quantity_2C);
    unsigned long int *prices_3A = knapsack(merch[3].armor, max_quantity_3A);
    unsigned long int *prices_3B = knapsack(merch[3].sword, max_quantity_3B);
    unsigned long int *prices_3C = knapsack(merch[3].shield, max_quantity_3C);

    unsigned long int *full_set_1 = price_full_set(prices_1A, prices_1B, prices_1C, q1);
    unsigned long int *full_set_2 = price_full_set(prices_2A, prices_2B, prices_2C, q2);
    unsigned long int *full_set_3 = price_full_set(prices_3A, prices_3B, prices_3C, q3);

    /*
     * Variable result is initialised to the maximum cost
     * that guarantees the purchase of N full equipment sets.
     */
    unsigned long int result = full_set_1[q1] + full_set_2[q2] + full_set_3[q3];

    for (int i = 0; i <= q1; i++)
    {
      for (int j = 0; j <= q2; j++)
      {
        if (N - i - j >= 0 && N - i - j <= q3)
        {
          unsigned long int temp = full_set_1[i] + full_set_2[j] + full_set_3[N - i - j];

          if (temp < result)
            result = temp;
        }
      }
    }

    printf("%lu\n", result);
    return 0;
  }
}
