#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <queue>
#include <stack>

using Graph =
        std::unordered_map<std::string, std::set<std::string>>;

void bfs(const Graph& g, const std::string& start_vertex) {
    std::queue<std::string> q_will_visit;
    std::unordered_set<std::string> visited;

    q_will_visit.push(start_vertex);
    visited.insert(start_vertex);

    while (!q_will_visit.empty()) {
        std::cout << q_will_visit.front() << std::endl;

        auto current_iter = g.find(q_will_visit.front());
        // If element haven't children
        if (current_iter == g.end()) {
            q_will_visit.pop();
            continue;
        }
        for (const auto& neighbor : current_iter->second) {
            if (visited.find(neighbor) == visited.end()) {
                q_will_visit.push(neighbor);
                visited.insert(neighbor);
            }
        }
        q_will_visit.pop();
    }
}

void dfs(const Graph& g, const std::string& start_vertex) {
    std::stack<std::string> s_will_visit;
    std::unordered_set<std::string> visited;

    s_will_visit.push(start_vertex);

    while (!s_will_visit.empty()) {
        if (visited.find(s_will_visit.top()) == visited.end()) {
            std::cout << s_will_visit.top() << std::endl;
            visited.insert(s_will_visit.top());
        }

        auto current_iter = g.find(s_will_visit.top());
        // If element haven't children
        if (current_iter == g.end()) {
            s_will_visit.pop();
            continue;
        }

        s_will_visit.pop();

        for (auto neighbor = current_iter->second.rbegin();
             neighbor != current_iter->second.rend();
             ++neighbor)
        {
            if (visited.find(*neighbor) == visited.end()) {
                s_will_visit.push(*neighbor);
            }
        }
    }
}

int main() {
    Graph graph;

    char graph_type;
    std::string start_vertex;
    char search_type;

    std::cin >> graph_type >> start_vertex >> search_type;

    std::string vertex_1, vertex_2;
    while(std::cin >> vertex_1 >> vertex_2) {
        switch (graph_type) {
            case 'd':
                graph[vertex_1].insert(vertex_2);
                break;
            case 'u':
                graph[vertex_1].insert(vertex_2);
                graph[vertex_2].insert(vertex_1);
                break;
            default:
                break;
        }
    }

    if (search_type == 'b' ) {
        bfs(graph, start_vertex);
    } else {
        dfs(graph, start_vertex);
    }
}
