/**
* Input Format 
* C = Commit Operation
* R = Read Operation
* W = Write Operation
* Operation   TransactionId  ResourseId
* R           1              X
* W           4              A
* C           3
*/

/*
* complie using following command:
* g++ -std=c++11 140101044.cpp
*/

#include <iostream>
#include <fstream>
#include <vector>
//#include <boost/algorithm/string.hpp>
#include <cstring>
#include <map>
#include <set>
#include <stack>
using namespace std;

typedef struct resourse_transaction
{
	string operation;
	int t_id;
	int op_pos;
}resourse_transaction;


map<string,vector<resourse_transaction> > r_map_t;

map<int, set<int> > adjacency_list;

set<int> t_ids;

map<int,int> commit_pos;

void print_set(set<int> v){
	set<int>::iterator i = v.begin();
	while(i != v.end()){
		cout << *i << ", ";
		i++;
	}
	cout << "\b\b " << endl;
}


void print_vector(vector<int> v){
	int size = v.size();
	for (int i = 0; i < size; ++i)
	{
		cout << v[i] << ", ";
	}
	cout << "\b\b " << endl;
}
void print_vector2(vector<resourse_transaction> v){
	int size = v.size();
	for (int i = 0; i < size; ++i)
	{
		cout << v[i].operation << " " << v[i].t_id  << " " << v[i].op_pos << endl;
	}
	//cout << "\b\b " << endl;
}

void print_map(map<int,set<int> > r_map_t){
	map<int,set<int> >::iterator i = r_map_t.begin();
	while(i != r_map_t.end()){
		cout << i->first << '\n';
		print_set(i->second);
		i++;
	}
}
void print_map3(map<int,int > commit_pos){
	map<int,int >::iterator i = commit_pos.begin();
	while(i != commit_pos.end()){
		cout << i->first << " " << i->second <<'\n';
		i++;
	}
}
void print_map2(map<string,vector<resourse_transaction> > r_map_t){
	map<string,vector<resourse_transaction> >::iterator i = r_map_t.begin();
	while(i != r_map_t.end()){
		cout << i->first << '\n';
		print_vector2(i->second);
		i++;
	}
}

class Graph
{
	// used by isCyclic()
	bool isCyclicUtil(int v, map<int, bool> &visited, map<int, bool> &rs);
	// A function used by topologicalSort
	void topologicalSortUtil(int v, map<int, bool> &visited, stack<int> &Stack);
public:
	
	// returns true if there is a cycle in this graph
	bool isCyclic();
	// prints a Topological Sort of the complete graph
    void topologicalSort();
	
};

// This function is a variation of DFSUytil() in http://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclicUtil(int v, map<int, bool> &visited, map<int, bool> &recStack)
{
    if(visited[v] == false)
    {
        // Mark the current node as visited and part of recursion stack
        visited[v] = true;
        recStack[v] = true;
 
        // Recur for all the vertices adjacent to this vertex
        set<int>::iterator i;
        for(i = adjacency_list[v].begin(); i != adjacency_list[v].end(); ++i)
        {
            if ( !visited[*i] && isCyclicUtil(*i, visited, recStack) )
                return true;
            else if (recStack[*i])
                return true;
        }
 
    }
    recStack[v] = false;  // remove the vertex from recursion stack
    return false;
}
 
// Returns true if the graph contains a cycle, else false.
// This function is a variation of DFS() in http://www.geeksforgeeks.org/archives/18212
bool Graph::isCyclic()
{
	
    // Mark all the vertices as not visited and not part of recursion
    // stack
    map<int, bool> visited;
    map<int, bool> recStack;
    /*bool *visited = new bool[V];
    bool *recStack = new bool[V];*/
    set<int>::iterator i;
    for(i = t_ids.begin(); i != t_ids.end(); i++)
    {
        visited[*i] = false;
        recStack[*i] = false;
    }
 
    // Call the recursive helper function to detect cycle in different
    // DFS trees
    for(i = t_ids.begin(); i != t_ids.end(); i++)
        if (isCyclicUtil(*i, visited, recStack))
            return true;
 
    return false;
}

// A recursive function used by topologicalSort
void Graph::topologicalSortUtil(int v, map<int, bool> &visited, stack<int> &Stack)
{
    // Mark the current node as visited.
    visited[v] = true;
 
    // Recur for all the vertices adjacent to this vertex
    set<int>::iterator i;
    for (i = adjacency_list[v].begin(); i != adjacency_list[v].end(); ++i)
        if (!visited[*i])
            topologicalSortUtil(*i, visited, Stack);
 
    // Push current vertex to stack which stores result
    Stack.push(v);
}
 
// The function to do Topological Sort. It uses recursive 
// topologicalSortUtil()
void Graph::topologicalSort()
{
    stack<int> Stack;
 
    // Mark all the vertices as not visited
    //bool *visited = new bool[V];
    map<int, bool> visited;

    /*for (int i = 0; i < V; i++)
        visited[i] = false;*/
    set<int>::iterator i;
    for(i = t_ids.begin(); i != t_ids.end(); i++)
    {
        visited[*i] = false;
    }
 
    // Call the recursive helper function to store Topological
    // Sort starting from all vertices one by one
    for (i = t_ids.begin(); i != t_ids.end(); i++)
      if (visited[*i] == false)
        topologicalSortUtil(*i, visited, Stack);
 
    cout << "A serial order of transactions:" << '\n';
    // Print contents of stack
    while (Stack.empty() == false)
    {
        cout << Stack.top() << " ";
        Stack.pop();
    }
    cout << '\n';
}
void create_adjacency_list(map<string,vector<resourse_transaction> > r_map_t){

	set<int>::iterator vec = t_ids.begin();
	while(vec != t_ids.end()){
		adjacency_list[*vec] = set<int>();
		vec++;
	}
	map<string,vector<resourse_transaction> >::iterator i = r_map_t.begin();
	while(i != r_map_t.end()){

		if (i->second.size() <= 1)
		{
			i++;
			continue;
		}
		
		vector<resourse_transaction>::iterator first = i->second.begin();
		vector<resourse_transaction>::iterator second = i->second.begin();

		while(first + 1 != i->second.end()){
			if (first->operation.compare("R") == 0 )
			{	
				second = first + 1;
				while(second != i->second.end() && (second->operation.compare("R") == 0  || first->t_id == second->t_id)){
					second++;
				}
				if ( second != i->second.end())
				{
					adjacency_list[first->t_id].insert(second->t_id);
				}
			}
			else{
				second = first + 1;
				while(second != i->second.end() && first->t_id == second->t_id ){
					second++;
				}
				if (first->t_id != second->t_id)
				{
					adjacency_list[first->t_id].insert(second->t_id);
				}
			}
			first++;
		}
		i++;
	}
}

bool cascade_rollback(){

	map<string,vector<resourse_transaction> >::iterator i = r_map_t.begin();
	while(i != r_map_t.end()){

		if (i->second.size() <= 1)
		{
			i++;
			continue;
		}
		
		vector<resourse_transaction>::iterator first = i->second.begin();
		vector<resourse_transaction>::iterator second = i->second.begin();
		map<int,int>::iterator f;
		map<int,int>::iterator s;

		while(first + 1 != i->second.end()){
			if (first->operation.compare("W") == 0 )
			{	
				second = first + 1;
				while(second != i->second.end()){
					if (first->t_id == second->t_id)
					{
						second++;
					}
					else{
						f = commit_pos.find(first->t_id);
						s = commit_pos.find(second->t_id);
						if (f->second > second->op_pos)
						{
							return false;
						}
					}
					second++;
					
				}
			}
			first++;
		}
		i++;
	}
	return true;
}

bool recoverable(){

	map<string,vector<resourse_transaction> >::iterator i = r_map_t.begin();
	while(i != r_map_t.end()){

		if (i->second.size() <= 1)
		{
			i++;
			continue;
		}
		
		vector<resourse_transaction>::iterator first = i->second.begin();
		vector<resourse_transaction>::iterator second = i->second.begin();
		map<int,int>::iterator f;
		map<int,int>::iterator s;

		while(first + 1 != i->second.end()){
			if (first->operation.compare("W") == 0 )
			{	
				second = first + 1;
				while(second != i->second.end()){
					if (first->t_id == second->t_id)
					{
						second++;
					}
					else{
						f = commit_pos.find(first->t_id);
						s = commit_pos.find(second->t_id);
						if (f->second > second->op_pos)
						{
							if (f->second > s->second)
							{
								return false;
							}
						}
					}
					second++;
					
				}
			}
			first++;
		}
		i++;
	}
	return true;
}
void update_r_map_t(){
	map<string,vector<resourse_transaction> >::iterator i = r_map_t.begin();
	while(i != r_map_t.end()){
		vector<resourse_transaction>::iterator vec = i->second.begin();
		//set<int>::iterator check;
		while(vec != i->second.end()){
			if (t_ids.find(vec->t_id) == t_ids.end())
			{
				i->second.erase(vec);
			}else{
				vec++;
			}
		}
		i++;
	}
}

vector<string> split(string str)
{
    vector<string> result;
    char buf[100];
    size_t length = str.copy(buf,str.length(),0);
    buf[length] = '\0';
    char *p = strtok(buf, " ");
	while (p) {
		string temp = p;
		result.push_back(temp);
	   /* printf ("Token: %s\n", p);*/
	    p = strtok(NULL, " ");
	}

    return result;
}


int main(int argc, char const *argv[])
{
	string line;
	int line_no = 0;
	ifstream reader;
	reader.open(argv[1]);
	if (!reader.is_open())
	{
		//cout << "Error opening the file." << endl;
		return 0;	
	}
	else{
		char c;
		while(getline(reader, line)){

			vector<string> fields = split(line);
		    /*boost::split(fields, line, boost::is_any_of(" "));*/

		    /*for (const auto& field : fields)
		        std::cout << "\"" << field << "\"\n";*/
			if (fields.size() != 2 && fields.size() != 3)
			{
				continue;
			}
		    if (fields[0] == "C")
		    {
		    	//cout << fields[2] << '\n';
		    	t_ids.insert(stoi(fields[1]));
		    	commit_pos[stoi(fields[1])] = line_no;
		    }
		    else{
		    	resourse_transaction temp;
		    	temp.operation = fields[0];
		    	temp.t_id = stoi(fields[1]);
		    	temp.op_pos = line_no;
		    	r_map_t[fields[2]].push_back(temp);
		    }
			//cout << line << '\n';
			line_no++;
		}
	}
	/*print_map2(r_map_t);
	update_r_map_t();
	cout << "Seconf time r_map_t" << endl;
	print_map2(r_map_t);
	cout << "Commited Operations" << endl;
	print_map3(commit_pos);*/
	create_adjacency_list(r_map_t);
	//print_map(adjacency_list);
	reader.close();
	/*cout << "Vertices in the graph" << endl;
	std::vector<int> output(t_ids.size());
	copy(t_ids.begin(), t_ids.end(), output.begin());
	print_vector(output);*/
	Graph graph;
	/*if (t_ids.size() <= 1)
	{
		cout << "Only " << t_ids.size() << " transaction(s) are committed transactions." << endl;
		return 0;		
	}*/
	if(graph.isCyclic()){
        cout << "Conflict serializable : No" << endl;
	}
    else{
        cout << "Conflict serializable : Yes" << endl;
    	//graph.topologicalSort();	
    }

    if(recoverable()){
        cout << "Recoverable: Yes" << endl;
	}
    else{
        cout << "Recoverable: NO" << endl;
    }
    if(cascade_rollback()){
        cout << "Cascaded roll back free: Yes" << endl;
	}
    else{
        cout << "Cascaded roll back free: NO" << endl;
    }
    
    return 0;
}