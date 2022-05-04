#include <iostream>

using namespace std;

struct data
{
  int first;
  int second;
};

data create_data(int f, int s)
{
  data foo;

  foo.first = f;
  foo.second = s;

  return foo;
}

struct my_data
{
  data d1, d2;
};

int find_length(my_data lis, my_data lds)
{
  int cand_1 = 0, cand_2 = 0, cand_3 = 0, cand_4 = 0;

  if (lis.d1.second < lds.d1.second)
    cand_1 = lis.d1.first + lds.d1.first;
  if (lis.d1.second < lds.d2.second)
    cand_2 = lis.d1.first + lds.d2.first;
  if (lis.d2.second < lds.d1.second)
    cand_3 = lis.d2.first + lds.d1.first;
  if (lis.d2.second < lds.d2.second)
    cand_4 = lis.d2.first + lds.d2.first;

  return max(cand_1, max(cand_2, max(cand_3, cand_4)));
}

int find_lis_index(int value, int right, int left, int *arr)
{
  while (right - left > 1)
  {
    int median = left + (right - left) / 2;

    if (arr[median] == value)
      return median;
    else if (arr[median] > value)
      right = median;
    else
      left = median;
  }

  return right;
}

void find_lis(int size, int *arr, my_data *lis)
{
  int length = 1, *aux;
  aux = (int *)malloc(size * sizeof(int));
  data max = create_data(1, arr[0]);

  aux[0] = arr[0];
  lis[0].d1 = max;
  lis[0].d2 = max;

  // Variable length denotes the active lenght of array aux,
  // that is the number of increasing subsequences stored so far.
  // length is increased by 1 every time we come across a number
  // arr[i] which is larger than the end elements of all subsequences
  // stored in aux.

  for (int i = 1; i < size; i++)
  {
    // minimum element found so far
    if (arr[i] <= aux[0])
    {
      lis[i].d1 = max;
      aux[0] = arr[i];
      lis[i].d2 = create_data(1, arr[i]);
    }

    // max element found so far, array aux must be extended
    else if (arr[i] > aux[length - 1])
    {
      aux[length++] = arr[i];
      max = create_data(length, arr[i]);
      lis[i].d2 = max;
      lis[i].d1 = max;
    }

    // arr[i] is in between some of the end elements stored in aux.
    // In this case, we must find the largest end element in aux that is
    // smaller than arr[i]
    else
    {
      int pos = find_lis_index(arr[i], length - 1, -1, aux);

      aux[pos] = arr[i];
      lis[i].d1 = max;
      lis[i].d2 = create_data(pos + 1, arr[i]);
    }
  }
}

int find_lds_index(int value, int right, int left, int *arr)
{
  while (right - left > 1)
  {
    int median = left + (right - left) / 2;

    if (arr[median] == value)
      return median;
    else if (arr[median] > value)
      left = median;
    else
      right = median;
  }

  return right;
}

void find_lds(int size, int *arr, my_data *lds)
{
  int length = 1, *aux;
  aux = (int *)malloc(size * sizeof(int));
  data max = create_data(1, arr[size - 1]);

  aux[0] = arr[size - 1];
  lds[size - 1].d1 = max;
  lds[size - 1].d2 = max;

  // Variable length denotes the active lenght of array aux,
  // that is the number of increasing subsequences stored so far.
  // length is increased by 1 every time we come across a number
  // arr[i] which is larger than the end elements of all subsequences
  // stored in aux.

  for (int i = size - 2; i >= 0; i--)
  {
    // minimum element found so far, array aux must be extended
    if (arr[i] < aux[length - 1])
    {
      aux[length++] = arr[i];
      max = create_data(length, arr[i]);
      lds[i].d2 = max;
      lds[i].d1 = max;
    }

    // max element found so far
    else if (arr[i] >= aux[0])
    {
      lds[i].d1 = max;
      aux[0] = arr[i];
      lds[i].d2 = create_data(1, arr[i]);
    }

    // arr[i] is in between some of the end elements stored in aux.
    // In this case, we must find the largest end element in aux that is
    // smaller than arr[i]
    else
    {
      int pos = find_lds_index(arr[i], length - 1, -1, aux);

      aux[pos] = arr[i];
      lds[i].d1 = max;
      lds[i].d2 = create_data(pos + 1, arr[i]);
    }
  }
}

int main()
{
  int N, K, *p, *modified, max_length = 0;
  my_data *lis, *lds;

  scanf("%d", &N);
  scanf("%d", &K);
  p = (int *)malloc(N * sizeof(int));

  for (int i = 0; i < N; i++)
    scanf("%d", &p[i]);

  modified = (int *)malloc(N * sizeof(int));
  lis = (my_data *)malloc(N * sizeof(my_data));
  lds = (my_data *)malloc(N * sizeof(my_data));

  for (int i = 0; i < N; i++)
    modified[i] = p[i] + K;

  find_lis(N, p, lis);
  find_lds(N, modified, lds);

  for (int i = 0; i < N - 1; i++)
    max_length = max(max_length, find_length(lis[i], lds[i + 1]));

  cout << max_length << '\n';

  return 0;
}
