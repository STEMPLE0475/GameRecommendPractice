#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

int main()
{
    // 1. 유저별로 플레이한 게임 목록
    unordered_map<string, unordered_set<int>> user_game_data = {
        {"kim", {1, 2, 3, 4} },
        {"part", {1, 2, 5}},
        {"lee", {2, 4, 6}},
        {"jeong", {3, 4, 6}},
        {"hong", {4, 5, 6}}
    };
    
    // 2. 추천 받을 유저의 이름을 입력
    string client_name;
    cout << "추천 받을 유저의 이름을 입력해주세요" << "\n";
    cin >> client_name;
    if (user_game_data.find(client_name) == user_game_data.end()) {
        cout << "데이터베이스에 등록되어 있지 않은 유저입니다.";
        return 0;
    }
    
    // 3. 겹치는 게임 수 기반으로 유사한 사용자 찾기
    const auto& client_games = user_game_data[client_name];
    vector<pair<string, int>> similarity_list;

    for (const auto& it : user_game_data)
    {
        const string& user_name = it.first;
        const auto& user_games = it.second;

        if (user_name == client_name) continue; // 본인의 경우 패스
        
        //하나라도 겹치는 게임이 있으면 유사한 사용자로 등록
        int overlap = 0;
        for (const auto& game : user_games)
        {
            if (client_games.count(game)) overlap++;
        }

        if (overlap > 0)
        {
            similarity_list.emplace_back(user_name, overlap);
        }
    }

    // (추가) 3-1. 유사한 사용자중 top 3만 반영
    vector<pair<string, int>> sorted_similarity_list;
    if (similarity_list.empty())
    {
        cout << "비슷한 유형의 유저를 찾을 수 없습니다";
        return 0;
    }
    else
    {
        sort(similarity_list.begin(), similarity_list.end(), [](const pair<string, int>& a, const pair<string, int>& b) { return a.second > b.second;  });
        for (int i = 0; i < similarity_list.size() && 3; i++)
        {
            sorted_similarity_list.emplace_back(similarity_list[i]);
        }
    }
        
    // 4. 비슷한 집단의 게임 목록 모으기
    vector<int> recommend_games;
    for (const auto& it : sorted_similarity_list)
    {
        const string& user_name = it.first;
        for (const auto& game : user_game_data[user_name])
        {
            recommend_games.emplace_back(game);
        }
    }

    // 5. 추천 게임 목록에서 중복이 많이 되는 것을 우선 순위로 정렬해서 출력 (이 부분 검토 필요)
    unordered_map<int, int> game_count;
    for (int game : recommend_games) {
        game_count[game]++;
    }

    vector<pair<int, int>> sorted_games;
    for (const auto& it : game_count) {
        sorted_games.emplace_back(it.first, it.second);
    }
    sort(sorted_games.begin(), sorted_games.end(), [](const pair<int, int>& a, const pair<int, int>& b) {return a.second > b.second;});

    for (const auto& it : sorted_games) {
        cout << "게임 ID: " << it.first << " (추천 점수: " << it.second << ")\n";
    }


    return 0;
}




