#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <chrono>
#include <omp.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <string>


void bfs(std::vector<std::vector<int>> graph) {
    std::vector<int> toVisit;
    bool visited[graph.size()] = { false };

    visited[0] = true;
    toVisit.push_back(0);

    while (!toVisit.empty()) {
        int item = toVisit[0];
        toVisit.erase(toVisit.begin());

        for (int i = 0; i < graph[item].size(); i++) {
            if (visited[graph[item][i]]) {
                continue;
            }
            visited[graph[item][i]] = true;
            toVisit.push_back(graph[item][i]);
        }
    }
}

void bfsParallel(std::vector<std::vector<int>> graph) {
    std::vector<int> toVisit;
    bool visited[graph.size()] = { false };

    visited[0] = true;
    toVisit.push_back(0);

    while (!toVisit.empty()) {
        int len = toVisit.size();

        #pragma omp parallel for
        for (int i = 0; i < len; ++i) {
            int item;
            #pragma omp critical
            {
                item = toVisit[0];
                toVisit.erase(toVisit.begin());
            }


            for (int i = 0; i < graph[item].size(); i++) {
                if (visited[graph[item][i]]) {
                    continue;
                }
                
                #pragma omp critical
                {
                    visited[graph[item][i]] = true;
                    toVisit.push_back(graph[item][i]);
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int isParallel = std::stoi(argv[1]);
    std::vector<std::vector<int>> graph;
    std::ifstream file("graph.txt");

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::vector<int> lineV;
            std::istringstream iss(line);
            int number;
            while (iss >> number) {
                lineV.push_back(number);
            }

            graph.push_back(lineV);
        }
        file.close();
    } else {
        std::cerr << "Unable to open file: " << std::endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    if (isParallel == 1) {
        bfsParallel(graph);
    } else {
        bfs(graph);
    }
    std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - start;
    std::cout << duration.count() << std::endl;

    return 0;
}