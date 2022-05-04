/*
 * Ορίζουμε τη μεταβλητή min_dist = min{d_i}, i = 1, 2, ..., N.
 * Έτσι, είναι φανερό ότι όλοι οι σταθμοί s = k * min_dist, k = 1, 2, ..., είναι προσβάσιμοι.
 * Άρα, κάθε τέτοιος σταθμός μπορεί να αποτελεί αναδρομικά μια νέα αφετηρία για το πρόβλημά μας.
 * Δηλαδή, αν δίνεται ένας σταθμός q, τότε αρκεί να ελέγξουμε αν ο q είναι προσβάσιμος με αφετηρία
 * κάποιο ακέραιο πολλαπλάσιο του min_dist που δεν ξεπερνά το q, ή ισοδύναμα αρκεί
 * να ελέγξουμε αν ο σταθμός (q % min_dist) είναι προσβάσιμος με αφετηρία το σταθμό 0.
 *
 * Ορίζουμε τον πίνακα response ο οποίος έχει min_dist θέσεις. Τότε, θα ισχύει
 * response[k] = l <=> Ο πρώτος σταθμός που βρίσκεται σε απόσταση k από κάποια
 * αφετηρία και στον οποίο γίνεται στάση είναι ο l. Έτσι, ο έλεγχος της προσβασιμότητας είναι
 * response[i % min_dist] = j <=> ο σταθμός i είναι προσβάσιμος αν j <= i,
 * αλλιώς ο i δεν είναι προσβάσιμος
 */

#include <iostream>
#include <set>

using namespace std;

void find_stations(set<pair<int, int>> stations, int *response, int *d, int N, int min_dist)
{
    while (!stations.empty())
    {
        pair<int, int> myPair = *stations.begin();

        int dist = myPair.first;
        int node = myPair.second;

        stations.erase(myPair);

        for (int i = 0; i < N; i++)
        {
            int new_node = (node + d[i]) % min_dist;
            int new_dist = dist + d[i];

            if (new_dist < response[new_node])
            {
                stations.erase(make_pair(response[new_node], new_node));
                response[new_node] = new_dist;
                stations.insert(make_pair(response[new_node], new_node));
            }
        }
    }
}

int main()
{
    int N, Q, *d, *s, *response, min_dist = 10000;
    set<pair<int, int>> stations;

    scanf("%d %d", &N, &Q);

    d = (int *)malloc(N * sizeof(int));
    s = (int *)malloc(Q * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        scanf("%d", &d[i]);

        if (d[i] < min_dist)
            min_dist = d[i];
    }

    for (int i = 0; i < Q; i++)
        scanf("%d", &s[i]);

    response = (int *)malloc(min_dist * sizeof(int));

    for (int i = 0; i < min_dist; i++)
        response[i] = 1000000001;

    response[0] = 0;

    stations.insert(make_pair(response[0], 0));

    find_stations(stations, response, d, N, min_dist);

    for (int i = 0; i < Q; i++)
    {
        if (response[s[i] % min_dist] <= s[i])
            printf("%s\n", "YES");
        else
            printf("%s\n", "NO");
    }

    return 0;
}
