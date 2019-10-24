#include<iostream>
#include<fstream>

int main()
{
    std::cout << "Program started!" << std::endl;

    std::ifstream file("generated30-1/generated30-1.cav");

    std::string line;

    if(file.is_open())
    {
       getline(file, line);
       std::cout << line << std::endl; //@cleanup remove this

        for(unsigned i = 0; i < line.size(); ++i)
        {
            
        }
    }
    else
    {
        std::cout << "Did not manage to open the file";//@cleanup remove this
    }

    return 0;
}