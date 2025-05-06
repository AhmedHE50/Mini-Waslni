#pragma once
#include "Graph.h"

class FileManager
{
public:
    // Graph File Operations
    static void loadGraph(Graph& graph, const string& filename);
	static void saveGraph(const Graph& graph, const string& filename);

    // Shortest Path File Operations
	static void savePathJSON(const vector<string>& path, const string& filename);
	static void savePathJSON(const string& start, const string& end, const map<string, pair<double, string>>& shortestMap, const string& filename);
};

