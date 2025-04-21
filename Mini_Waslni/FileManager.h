#pragma once
#include "Graph.h"

class FileManager
{
public:
	static void loadGraph(Graph& graph, const string& filename);
	static void saveGraph(Graph& graph, const string& filename);
};

