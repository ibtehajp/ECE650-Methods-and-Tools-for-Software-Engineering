// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include "a4_header.h"

// using namespace std;

std::vector<int> edge_list;
std::vector<std::vector<int>> new_list;
char check;
int vertices_number;
std::string line;
std::string statement;
std::string edge_string;
unsigned int i;

void parser_edge(std::string edge_list, int vertices_number){
    //std::cout <<"Parser Edge function executed" << std::endl;

    std::smatch matches;
    
    edge_list.clear(); // need to do this to clear the vector in each loop
    new_list.clear(); // need to do this to clear the vector in each loop
    
    std::cout << std::boolalpha;
    std::regex reg("[+-]?[0-9]+");
    i = 0;
    unsigned int t = 0;
    while(std::regex_search(statement, matches, reg)){
        // std::cout << matches.str(0) << "\n";
        edge_list.push_back(stoi(matches.str(0)));
        statement = matches.suffix().str();
        if (edge_list[t] > vertices_number){
            throw "Edge is beyond the number of vertices";
        }
        ++t;
        //std::cout << matches.str(0) << '\n';
        // i++;
    }
    // std::cout << "Edge size is: " << edge_list.size();
        //std::cout<< "while loop completed"<< std::endl;

        //std::cout << edge_list.size()<< std::endl;
        //std::cout << edge_list[11]<< "compared to" << edge_list.at(11) << std::endl;
    for (i = 0; i != edge_list.size(); ++i){
        //std::cout << "Loop started"<< std::endl;
        if (edge_list[i] <= 0 || edge_list[i+1] <= 0){
            new_list.clear();
            throw "Edge list is incorrect";
        }
        new_list.push_back({edge_list[i],edge_list[i+1]});
        i = i + 1 ;
    }
    //std::cout << new_list[5][0] << "," << new_list[5][1]<<std::endl; 
    // std::cout << new_list[1][0] << "," << new_list[1][1];
}



int main(int argc, char** argv) {
    // Test code. Replaced with your code

    while (getline(std::cin, line)) {
        check = (char) 0; // need to do this to clear "check" in each loop
        std::istringstream input(line);
        //std::cout << "While loop running" << "\n";
        input >> check;
        try{
            if (check == 'V') {
                vertices_number = 0;
                input >> vertices_number;
                edge_list.clear(); // added here to conform to the assignment requirement
                new_list.clear(); // added here to conform to the assignment requirement
                if (vertices_number <= 0){
                    throw "Number of vertices are less than or equal to zero";
                }
            }

            else if (check == 'E') {
                //std::cout << "if of E executed" << '\n'; 
                input >> statement;
                    parser_edge(statement, vertices_number);
                    //std::cout << new_list[0][0] << new_list[3][0] << std::endl;
                    graph g1 {};
                    g1.adjacency_list_generator(new_list,vertices_number);
                    REDUCTION_TO_CNF_SAT(g1);
                }    

            else{

                throw "Improper command";
            }

        }
        catch (const char* m) {
            std::cerr << "Error: " << m << std::endl;
        }
        
    }
    return 0;
}
