/*
 * The main idea behind our algorithm is that we divide array p[]
 * into two subarrays. We name y the point of division. That is,
 * the subarrays that occur are p[0...(y - 1)] and p[y...(N - 1)].
 * It is clear that there is no overlap between any subarray of these
 * two subarrays. We also use an auxiliary array sums[] with K positions
 * such that sums[i] contains the minimum length of a subarray included
 * in p[y...(N - 1)] and sums up to i. For each division, that is for every
 * different value of y, we calculate the sums of all subarrays that end at
 * position (y - 1) and for each sum we check whether (K - sum) exists in sums[],
 * securing that there is no overlap, thanks to the division. So, for a single
 * fixation of y, variable result stores the minimun length of two subarrays that sum up to K,
 * where one subarray belongs to the left of y and the other to the right of y.
 * We repeat the same process for all possible values of y (for all possible divisions),
 * where y is initialised to (N - 1) and goes to the left. It is obvious that y can have
 * N possible values (one for every cell of array p[]) and in each fixation (N - y)
 * new sums are created to the right subarray and y new sums are created to the left
 * subarray, so in each fixation of y N new sums are totally evaluated from both subarrays.
 * However, N fixations of y happen, so the complexity of our algorithm is O(N^2).
 */

#include <iostream>

using namespace std;

int min(int a, int b)
{
  if (a < b)
    return a;
  else
    return b;
}

/*
 * Updates the sums that appear in subarray p[y...(size - 1)],
 * such that sums[i] contains the minimum length of a subarray
 * included in p[y...(size - 1)] that sums up to i.
 */
void go_right(int y, int limit, int size, int *p, int *sums)
{
  int sum = 0;

  for (int k = y; k < size; k++)
  {
    sum += p[k];

    if (sum > limit)
      break;
    /*
     * k - y + 1 corresponds to the length of the
     * subarray that sums up to sum.
     */
    else if (sums[sum] == -1 || k - y + 1 < sums[sum])
        sums[sum] = k - y + 1;
  }
}

/*
 * Finds the sums of all subarrays of type p[x...y],
 * when x is a variable and y is fixed. For each sum
 * we check if its complementary in terms of variable K
 * exists in array sums[]. If it exists, we update variable result,
 * so that for fixed y it contains the minimum length of two subarrays
 * that sum up to K.
 */
void go_left(int y, int limit, int &result, int *p, int *sums)
{
  int sum = 0;

  for (int x = y; x >= 0; x--)
  {
    sum += p[x];

    if (sum > limit)
      break;
    /*
     * y - x + 1 corresponds to the length of the
     * subarray that sums up to sum.
     */
    else if (sum == limit)
      result = min(result, y - x + 1);
    else if (sums[limit - sum] != -1)
      result = min(result, sums[limit - sum] + y - x + 1);
  }
}

int main()
{
  int N, K, *p, *sums;

  scanf("%d", &N);
  scanf("%d", &K);

  p = (int *)malloc(sizeof(int) * N);
  sums = (int *)malloc(sizeof(int) * (K + 1));

  for (int i = 0; i < N; i++)
    scanf("%d", &p[i]);

  for (int i = 0; i <= K; i++)
    sums[i] = -1;

  int result = N + 1;

  for (int i = N - 1; i >= 0; i--)
  {
    go_right(i, K, N, p, sums);
    go_left(i - 1, K, result, p, sums);
  }

  if (result == N + 1)
    result = -1;

  cout << result << '\n';

  return 0;
}
