/*
4.1. Максимальный путь 1
Имеется взвешенный ориентированный ациклический граф. Найти максимальный
путь, используя алгоритм Беллмана-Форда.
Ввод из файла INPUT.TXT. Первая строка входного файла INPUT.TXT содержит 3
числа: N - количество вершин графа (3 ≤ N ≤ 1000), M – дуг (3 ≤ M ≤ 500000), A – номер
начальной вершины. В следующих M строках по 3 числа, задающих дуги: начало дуги,
конец дуги, длина (вес).
Вывод в файл OUTPUT.TXT. В i-й строке выводится длина максимального пути из
вершины S до i-й вершины, затем количество вершин максимального пути, а далее
номера вершин максимального пути. Все числа разделены пробелами. Если пути в
некоторую вершину не существует, то в соответствующей строке выводится слово No.
Если в графе имеется достижимый из начальной вершины цикл положительной длины,
то вывод состоит из двух строк. В первой строке выводится слово No, а во второй –
количество и номера вершин обнаруженного цикла через пробел, начиная с его любой
вершины и заканчивая ей же. При наличии нескольких циклов вывести информацию о
любом из них.

Пример
Ввод        Ввод 2
4 6 1       5 6 1
1 2 1       1 2 1
1 4 2       1 4 2
2 3 3       2 3 3
2 4 3       2 4 3
3 4 1       3 4 1
4 3 1       5 2 5
Вывод 1     Вывод 2
No          0 1 1
3 4 3 4     1 2 1 2
            4 3 1 2 3
            5 4 1 2 3 4
            No

Разработчики: студенты группы ПСм-11 Стародубцева А. и Свинчуков И.

Среда выполнения: Visual Studio 2017

Инструкция о запуске:
1. Запустить программу
2. Ввести название входного файла
3. Ввести название выходного файла

По умолчанию входные файлы должны лежать в одной директории с исполняемым файлом
*/

#include "pch.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Edge {
public:
	int from;
	int to;
	int weight;
};

static const int NEG_INF = INT32_MIN;
static const int POS_INF = INT32_MAX;

void ReadFromFile(vector<Edge> & graph, int & nodeCount, int & edgeCount, int & startNode);
int FindMaxWays(const vector<Edge> & graph, vector<int> & maxWeights, vector<int> & prevNodes);
void WriteToFile(const vector<int> & maxWeights, const vector<int> & prevNodes, int changedNode);
vector<int> GetPath(const vector<int> & prevNodes, int startNode, bool isCycle = false);

int main() {
	int nodeCount, edgeCount, startNode;
	vector<Edge> graph;

	ReadFromFile(graph, nodeCount, edgeCount, startNode);

	vector<int> maxWeights(nodeCount, NEG_INF);
	vector<int> prevNodes(nodeCount, -1);
	maxWeights[startNode - 1] = 0;

	int changedNode = FindMaxWays(graph, maxWeights, prevNodes);

	WriteToFile(maxWeights, prevNodes, changedNode);

	return 0;
}

void ReadFromFile(vector<Edge> & graph, int & nodeCount, int & edgeCount, int & startNode) {
	string inputFile = "";

	cout << "Input file: ";
	cin >> inputFile;
	
	ifstream stream(inputFile);

	stream >> nodeCount >> edgeCount >> startNode;	

	for (int i = 0; i < edgeCount; ++i) {
		int from, to, weight;
		Edge newEdge;

		stream >> from >> to >> weight;

		newEdge.from = from - 1;
		newEdge.to = to - 1;
		newEdge.weight = weight;

		graph.push_back(newEdge);
	}
}

int FindMaxWays(const vector<Edge> & graph, vector<int> & maxWeights, vector<int> & prevNodes) {
	int changedNode;
	int nodeCount = maxWeights.size();
	int edgeCount = graph.size();


	for (int i = 0; i < nodeCount; ++i) {
		changedNode = -1;
		
		for (int j = 0; j < edgeCount; ++j) {
			if (maxWeights[graph[j].from] > NEG_INF) { //не трогать вершину, которую еще не достигли
				int from = graph[j].from;
				int to = graph[j].to;
				int cost = graph[j].weight;
				int newWeight = min(maxWeights[from] + cost, POS_INF); //предупреждение переполнения int

				if (maxWeights[to] < newWeight) {
					maxWeights[to] = newWeight;
					prevNodes[to] = from;
					changedNode = to;
				}
			}
		}
		
		if (changedNode == -1) { //завершение, если не произошло улучшения результата
			break;
		}
	}

	return changedNode;
}

void WriteToFile(const vector<int> & maxWeights, const vector<int> & prevNodes, int changedNode) {
	string outputFile = "";

	cout << "Output file: ";
	cin >> outputFile;

	ofstream stream(outputFile);

	if (changedNode > -1) {
		vector<int> path = GetPath(prevNodes, changedNode, true);
	
		stream << "No" << endl << path.size();
		for (auto node : path) {
			stream << " " << node + 1;
		}

		return;
	}
	
	for (int i = 0; i < maxWeights.size(); ++i) {
		vector<int> path = GetPath(prevNodes, i);

		if (maxWeights[i] == NEG_INF) {
			stream << "No" << endl;
			continue;
		}

		stream << maxWeights[i] << " " << path.size();
		for (auto node : path) {
			stream << " " << node + 1;
		}
		stream << endl;
	}
}

vector<int> GetPath(const vector<int> & prevNodes, int startNode, bool isCycle) {
	int node = prevNodes[startNode];
	vector<int> path;

	while (isCycle && find(path.begin(), path.end(), node) == path.end()
		|| !isCycle && node != -1) {
		path.push_back(node);
		node = prevNodes[node];
	}

	if (isCycle) {
		path.push_back(node);
	}
	
	reverse(path.begin(), path.end());

	if (!isCycle) {
		path.push_back(startNode);
	}

	return path;
}