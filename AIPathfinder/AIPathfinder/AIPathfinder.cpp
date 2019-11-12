// AIPathfinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<math.h>
#include<sstream>

struct Cavern
{
	int cav_num = 0;
	int xcoord = 0;
	int ycoord = 0;

	double g = 0;
	double h = 0;
	double f = 0;

	int parent = 0;
	std::vector<int> connections;
};

void display_all_caverns();
std::vector<int> clean_input(std::string);
void build_matrix(std::vector<int>*);
void setup_caverns();
void a_star();
void reconstruct_path(Cavern);
void output(std::string s);
double calculate_distance(Cavern, Cavern);
bool check_list(std::vector<Cavern>*, int);
int get_lowest_f(std::vector<Cavern>*);

int number_of_caverns = 0;
std::vector<Cavern> caverns;
std::vector<int> coords;
std::vector<std::vector<int>> matrix;

int main()
{
	//std::ifstream input_file("../../generated30-1/generated30-1.cav"); //@cleanup set this to open the correct file location
	//std::ifstream input_file("../../cavernsfiles/input1.cav");
	std::ifstream input_file("../../generated1000-1/generated1000-1.cav");
	//std::ifstream input_file("../../generated100-1/generated100-1.cav");
	std::string line;

	if (input_file.is_open())
	{
		getline(input_file, line);

	
		std::vector<int>cleaned_input = clean_input(line);
		number_of_caverns = cleaned_input[0];

		cleaned_input.erase(cleaned_input.begin());
		
		//Loop thorugh all the coords in the caverns vector
		for (int i = 0; i < number_of_caverns * 2; ++i)
		{
			coords.push_back(cleaned_input[i]);
		}

		build_matrix(&cleaned_input);
		
		setup_caverns();

		//display_all_caverns();
		a_star();
	}
	else
	{
		std::cout << "Did not manage to open the file";//@cleanup remove this
	}
	return 0;
}

//Display all the cavern data
void display_all_caverns()
{
	for (int i = 0; i < caverns.size(); ++i) 
	{
		Cavern c = caverns[i];
		std::cout << "num: " << c.cav_num << " x " << c.xcoord << " y " << c.ycoord << " Parent " << c.parent << " Connections ";
		for (int j = 0; j < c.connections.size(); ++j)
		{
			std::cout << c.connections[j] << " ";
		}
		std::cout << std::endl;
	}
}

//Separate all the values of the file by comma and place them in a vector
std::vector<int> clean_input(std::string line)
{
	std::vector<int> cleaned_input;
	std::string s;
	int pushed_num = 0;
	std::string temp = "";
	//separate all the values from the file by comma			
	std::stringstream ss(line);

    for (int i; ss >> i;) 
	{
        cleaned_input.push_back(i);    
        if (ss.peek() == ',')
            ss.ignore();
    }
	return cleaned_input;
}

//Create a matrix of all the oonnections
void build_matrix(std::vector<int>* cleaned_input)
{
	//Put all the matrix values into the matrix vector
	//This creates a table similar to the one in coursework spec
	//Each vector within the table vector is a row in the table
	std::vector<int> v;
	std::vector<std::vector<int>> table;
	std::vector<int>& vector_ref = *cleaned_input;//dereference the pointer so that the indexes of the vector can be accessed

	for (int i = number_of_caverns * 2; i < cleaned_input->size(); ++i)
	{
		v.push_back(vector_ref[i]);
		
		if (v.size() == number_of_caverns)
		{
			table.push_back(v);
			v.clear();
		}
	}

	//set matrix vector to be conections for each to the caverns
	std::vector<int> temp;

	for (int i = 0; i < table.size(); ++i)
	{
		for (int j = 0; j < number_of_caverns ; ++j)
		{
			temp.push_back(table[j][i]);
		}
		matrix.push_back(temp);
		temp.clear();
	}
}

//Set up caverns with ids, coords and connections
void setup_caverns()
{
	//Set up the Cavern structs with numbers and coords
	int next_cav_num = 1;
	for (int i = 0; i < number_of_caverns * 2; i += 2)
	{
		struct Cavern* c = new Cavern;
		c->cav_num = next_cav_num;
		c->xcoord = coords[i];
		c->ycoord = coords[i + 1];
		//std::cout << "num " << c->cav_num << " x " << c->xcoord << " y " << c->ycoord << std::endl;
		++next_cav_num;
		caverns.push_back(*c);
	}

	//set up cavern connections
	for (int i = 0; i < matrix.size(); ++i)
	{
		for (int j = 0; j < matrix[i].size(); ++j)
		{
			//std::cout << "i " << i << " j " << j << std::endl;
			if (matrix[i][j] == 1)
			{
				caverns[i].connections.push_back(j + 1);
			}
		}
	}
}

void a_star()
{
	std::vector<Cavern> open_list;
	std::vector<Cavern> closed_list;

	open_list.push_back(caverns[0]);

	Cavern goal_node = caverns.back();

	bool found = false;
	while (open_list.size() != 0)
	{

		int current_index = get_lowest_f(&open_list);
		Cavern current = open_list[current_index];
		open_list.erase(open_list.begin() + current_index);
		closed_list.push_back(current);

		if (current.cav_num == goal_node.cav_num)
		{
			//std::cout<< "found the node" <<std::endl;
			found = true;
			break;
		}

		for (int i = 0; i < current.connections.size(); ++i)
		{
			if (!check_list(&closed_list, current.connections[i]) )
			{
				Cavern* child = &caverns[current.connections[i] - 1];

				double g = current.g + calculate_distance(current, *child);

				if (g < child->g || child->g == 0)
				{
					child->g = roundf(g * 100) / 100;
					
					child->parent = current.cav_num;
					open_list.push_back(*child);
				}

				child->h = roundf(calculate_distance(*child, goal_node) * 100) / 100;
				//child->f = child->g + child->h;
				child->f = roundf((child->g + child->h) * 100) / 100;

				if (check_list(&open_list, child->cav_num))
				{
					//std::cout << "in the open list already" << std::endl;
				}
				else
				{
					//std::cout << "inserting " << current.connections[i] << std::endl;
					open_list.push_back(*child);
				}
			}
		}
	}
	if (found)
	{
		std::cout << "Found the goal node" << std::endl;
		reconstruct_path(caverns[caverns.size() - 1]);
	}
	else
	{
		std::cout <<  "Did not find the goal node" << std::endl;
	}	
}

void reconstruct_path(Cavern c)
{
	std::vector<int> path;
	//double distance = c.g;
	double distance = caverns[c.parent - 1].g;
	//display_all_caverns();
	while (true)
	{
		path.push_back(c.cav_num);
		if (c.cav_num == 1) { break; }
		c = caverns[c.parent - 1];
	}
	std::string s = "";
	for (int i = path.size() - 1; i >= 0; --i)
	{
		std::cout << path[i] << " ";
		s += std::to_string(path[i]);
		s += " ";
	}
	std::cout << std::endl;
	std::cout << "Distance = " << distance << std::endl;
	output(s);
}

void output(std::string s)
{
	std::ofstream output_file("foo.bar");
	output_file << s;
	output_file.close();
}

double calculate_distance(Cavern a, Cavern b)
{
	//if (b.cav_num == number_of_caverns)
		//return 0;

	double x_dist = a.xcoord - b.xcoord;
	
	double y_dist = a.ycoord - b.ycoord;

	//std::cout << "Distance between " << a.cav_num << " and " << b.cav_num << " " << sqrt((x_dist*x_dist) + (y_dist*y_dist)) <<std::endl;

	return sqrt((x_dist*x_dist) + (y_dist*y_dist));
}


//Checks if a cavern exists within a vector
bool check_list(std::vector<Cavern>* list, int n)
{
	std::vector<Cavern>& vec_ref = *list;

	if (vec_ref.size() > 0)
	{
		for (int i = 0; i < vec_ref.size() - 1; ++i)
		{
			if (vec_ref[i].cav_num == n)
			{
				//found the value
				//std::cout << "found: " << n << std::endl;
				return true;
			}
		}
	}
	//std::cout << "not found " << n << std::endl;
	return false;
}

//Retruns the index of the cavern with the lowest f value in the open_list
int get_lowest_f(std::vector<Cavern>* open_list)
{
	std::vector<Cavern>& vec_ref = *open_list;

	int lowest_f = vec_ref[0].f;
	int lowest_index = 0;
	for (int i = 0; i < vec_ref.size(); ++i)
	{
		/*if (vec_ref[i].cav_num == 655 || vec_ref[i].cav_num == 687)
		{
			std::cout << " cavern " << vec_ref[i].cav_num << " " << vec_ref[i].f << std::endl;
		}*/
		if (vec_ref[i].f < vec_ref[lowest_index].f)
		{
			lowest_f = vec_ref[i].f;
			lowest_index = i;
		}
	}

	return lowest_index;
}
