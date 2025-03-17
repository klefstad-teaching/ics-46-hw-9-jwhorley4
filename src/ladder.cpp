#include "ladder.h"
#define my_assert(e) {cout << #e << ((e) ? " passed": " failed") << endl;}

void error(string word1, string word2, string msg)
{
    cout << "Error: " << msg << " from " << word1 << " to " << word2 << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d)
{
    int m = str1.size();
    int n = str2.size();
    if (abs(m - n) > d) 
        return false;

    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));

    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0) 
                dp[i][j] = j;
            else if (j == 0)
                dp[i][j] = i;
            else if (str1[i - 1] == str2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else 
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
        }
    }
    return dp[m][n] <= d;
}

bool is_adjacent(const string& word1, const string& word2)
{
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list)
{
    if(begin_word == end_word)
    {
        error(begin_word, end_word, "Start and end words cannot be the same");
        return{};
    }

    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    set<string> visited;
    visited.insert(begin_word);

    while(!ladder_queue.empty())
    {
        vector<string> ladder = ladder_queue.front();
        ladder_queue.pop();

        string last_word = ladder.back();

        for(const string& word : word_list)
        {
            if(is_adjacent(last_word, word) && !visited.count(word))
            {
                visited.insert(word);

                vector<string> new_ladder = ladder;
                new_ladder.push_back(word);

                if(word == end_word)
                    return new_ladder;
                ladder_queue.push(new_ladder);
            }
        }
    }
    return{};
}

void load_words(set<string> & word_list, const string& file_name)
{
    ifstream file(file_name);
    if(!file)
        return;

    string word;
    while(file >> word)
        word_list.insert(word);
    file.close();
}

void print_word_ladder(const vector<string>& ladder)
{
    if(ladder.empty())
        return;
    for(size_t i = 0; i < ladder.size(); ++i)
    {
        if(i > 0)
            cout << " -> ";
        cout << ladder[i];
    }
    cout << endl;
}

void verify_word_ladder() 
{
    set<string> word_list;
    load_words(word_list, "src/words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}