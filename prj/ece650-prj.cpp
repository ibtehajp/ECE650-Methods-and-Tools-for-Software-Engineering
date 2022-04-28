// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>
#include "prj_header.h"
#include <sstream>
#include <fstream>
#include <memory>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// using namespace std;

std::vector<int> edge_list;
std::vector<std::vector<int>> new_list;
char check;
int vertices_number;
std::string line;
std::string statement;
std::string edge_string;
unsigned int i;
graph g1 {};
graph g2 {};
graph g3 {};
std::string algo1;
std::string algo2;
std::string algo3;
double duration_algo1;
double duration_algo2;
double duration_algo3;


void* thread_APPROX_VC_1(void* arg) {
	//cout << "1" << endl;

	clock_t start, finish;
	duration_algo2 = 0;

	start = clock();
	algo2 = APPROX_VC_1(g2);
	finish = clock();

	duration_algo2 = (double)(finish - start) / CLOCKS_PER_SEC;
	//std::cout << "the duration time of APPROX_VC_1 is: " << duration << std::endl;
}

void* thread_APPROX_VC_2(void* arg) {
	clock_t start, finish;
	duration_algo3 = 0;

	start = clock();
	algo3 = APPROX_VC_2(g3);
	finish = clock();

	duration_algo3 = (double)(finish - start)/CLOCKS_PER_SEC;
	//std::cout << "the duration time of APPROX_VC_2 is: " << duration << std::endl;
}

void* thread_CNF_SAT(void* arg) {

	//CNF_SAT_VC(std::ref(vertex_num), std::ref(vec_E));
	clock_t start, finish;
	duration_algo1 = 0;

	start = clock();
	algo1 = REDUCTION_TO_CNF_SAT(g1);
	finish = clock();

	duration_algo1 = (double)(finish - start) / CLOCKS_PER_SEC;
	//std::cout << "the duration time of CNF_SAT is: " << duration << std::endl;
}

void print_output(std::string a, std::string b,std::string c){
    std::cout << "CNF-SAT-VC: " << a << "\n";
    std::cout << "APPROX-VC-1: " << b << "\n";
    std::cout << "APPROX-VC-2: " << c << std::endl;
}

void print_time(double a,double b,double c){
    std::cout << "Duration of CNF-SAT-VC: " << a << "\n";
    std::cout << "Duration of APPROX-VC-1: " << b << "\n";
    std::cout << "Duration of APPROX-VC-2: " << c << std::endl;
}

void print_approx_ratio(int a,int b,int c){
    double b1 = b/a;
    double c1 = c/a;

    std::cout << "Approximation Ratio of CNF-SAT-VC: " << a/a << "\n";
    std::cout << "Approximation Ratio of APPROX-VC-1: " << b1 << "\n";
    std::cout << "Approximation Ratio of APPROX-VC-2: " << c1 << std::endl;
}

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

void* input_thread(void* arg) {
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
                g1.clear();
                g2.clear();
                g3.clear();
                algo1.clear();
                algo2.clear();
                algo3.clear();

                g1.adjacency_list_generator(new_list,vertices_number);
                g2.adjacency_list_generator(new_list,vertices_number);
                g3.adjacency_list_generator(new_list,vertices_number);
                
                pthread_t thread_algo1;
                pthread_t thread_algo2;
                pthread_t thread_algo3;

                int a1, a2, a3;

                a1 = pthread_create(&thread_algo1, NULL, thread_CNF_SAT, NULL);
                a2 = pthread_create(&thread_algo2, NULL, thread_APPROX_VC_1, NULL);
                a3 = pthread_create(&thread_algo3, NULL, thread_APPROX_VC_2, NULL);
                
                pthread_join(thread_algo1, NULL);
                //std::cout << " algo 1 join completed"<< std::endl;
                pthread_join(thread_algo2, NULL);
                //std::cout << " algo 2 join completed"<< std::endl;
                pthread_join(thread_algo3, NULL);
                //std::cout << " algo 3 join completed"<< std::endl;

                print_output(algo1, algo2, algo3);
                // print_time(duration_algo1,duration_algo2,duration_algo3);
                // print_approx_ratio();
                //csv_printer(vertices_number,duration_algo1,duration_algo2,duration_algo3);
            }    

        else{

            throw "Improper command";
        }

    }
    catch (const char* m) {
        std::cerr << "Error: " << m << std::endl;
    }

    }

}

int main(int argc, char** argv) {
    int p;
    pthread_t thread_in;

    p = pthread_create(&thread_in,NULL,input_thread,NULL);

    
    
    pthread_join(thread_in, NULL); 

    return 0;
}
