#include <vector>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <list>
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include "a4_header.h"

// class graph{
//     public:
//         unsigned int total_edges;
//         std::vector<std::vector<unsigned int>> adjacency_list;

//         void adjacency_list_generator(std::vector<std::vector<int>> a, unsigned int b){
//             adjacency_list.resize(b,{});
//             for (unsigned int i = 0; i < a.size(); ++i){
//                 adjacency_list[a[i][0]-1].push_back(a[i][1]-1);
//                 adjacency_list[a[i][1]-1].push_back(a[i][0]-1);
//                 total_edges = total_edges + 1;
//             }
//         }
//         void clear(void){
//             total_edges =  0;
//             adjacency_list.clear();
//         }
//         void adj_printer(void){
//             for (unsigned int i = 0; i< adjacency_list.size(); ++i){
//                 for (unsigned int j = 0 ; j <adjacency_list[i].size(); ++j){
//                     std::cout<< adjacency_list[i][j] << ", ";
//                 }
//                 std::cout << "\n";
//             }

//         }
// };

void REDUCTION_TO_CNF_SAT(graph g1){
    // start the minisat program
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    // matrix of size n x k for the atomic propositions
    std::vector<std::vector<Minisat::Lit>> nxk_matrix(g1.adjacency_list.size());

    std::vector<int> final_result;

    for(unsigned int k_num = 1; k_num <  g1.adjacency_list.size() + 1; ++k_num){

        //generate the complete n x k matrix made up of Minisat literal elements
        for (unsigned int i = 0 ; i < g1.adjacency_list.size(); ++i){
            for (unsigned int j = 0 ; j < k_num; ++j){
                Minisat::Lit literal_1 = Minisat::mkLit(solver->newVar());
                nxk_matrix[i].push_back(literal_1);
            }
        }
        //std::cout << "k = " << k_num << " | n x k matrix generated" << std::endl; 

        // condition 1: At least one vertex is the ith vertex in the vertex cover
        for (unsigned int l = 0 ; l < k_num; ++l){
            
            // generate a vector for the first condition for each value of k
            Minisat::vec<Minisat::Lit> first_condition_literal;
            
            for (unsigned int m = 0 ; m < g1.adjacency_list.size(); ++m){
                first_condition_literal.push(nxk_matrix[m][l]);
            }
            solver->addClause(first_condition_literal);
        }
        //std::cout << "k = " << k_num << " | condition 1 completed" << std::endl;


        // condition 2: No one vertex can appear twice in a vertex cover
        for(unsigned int n = 0; n < g1.adjacency_list.size(); ++n){
            //std::cout << "n = " << n << std::endl;
            for(unsigned int o = 0; o < k_num; o++){
                //std::cout << "o = " << o << std::endl;
                for(unsigned int p = o + 1; p < k_num; p++){
                    //std::cout << "p = " << p << std::endl;
                    solver->addClause(~nxk_matrix[n][o], ~nxk_matrix[n][p]);
                }
            }
        }
        //std::cout << "k = " << k_num << " | condition 2 completed" << std::endl;

        // condition 3: No more than one vertex appears in the mth position of the vertex cover
        for(unsigned int q = 0; q < k_num; ++q){
            for(unsigned int r = 0; r < g1.adjacency_list.size(); ++r){
                for(unsigned int s = r + 1; s < g1.adjacency_list.size(); ++s){
                    solver->addClause(~nxk_matrix[r][q], ~nxk_matrix[s][q]);
                }
            }
        }
        //std::cout << "k = " << k_num << " | condition 3 completed" << std::endl;

        // condition 4: Every edge is incident to at least one vertex in the vertex cover
        for(unsigned vertex_1 = 0 ; vertex_1 < g1.adjacency_list.size(); ++vertex_1) {
            for (auto vertex_2 : g1.adjacency_list[vertex_1]) {
                if(vertex_2 < vertex_1) continue;
                Minisat::vec<Minisat::Lit> edge;
                for (unsigned int t = 0; t < k_num; t++) {
                    edge.push(nxk_matrix[vertex_1][t]);
                    edge.push(nxk_matrix[vertex_2][t]);
                }
                solver->addClause(edge);
            }
        }
        //std::cout << "k = " << k_num << " | condition 4 completed" << std::endl;


        // calculate the cnf using the minisat solver
        bool res = solver->solve();

        // return the vertex cover if the cnf is satisfiable
        if (res == true){
            for ( unsigned int i = 0; i < g1.adjacency_list.size(); ++i){
                for ( unsigned int j =0; j < k_num; ++j){
                    if ( Minisat::toInt(solver->modelValue(nxk_matrix[i][j])) == 0){
                        final_result.push_back(i);
                    }
                }
            }
            
            std::sort( final_result.begin(), final_result.end(), std::less<int>());

            for ( unsigned int i = 0; i < final_result.size(); ++i){
                std::cout << final_result[i] + 1 << " ";
            }
            std::cout << "" << std::endl;
            break; // stop the k loop
        }
        else {
            solver.reset(new Minisat::Solver());
            //std::cout << "at k = " << k_num << " , cnf is not satisfiable" << std::endl;
        }          
    }
}


// int main(){
//     graph g1{};
//     g1.adjacency_list_generator({{1,5},{5,2},{1,4},{4,5},{4,3},{2,4}},5);
//     //g1.adj_printer();
    
//     //std::cout <<'\n' << "//Now the reduction algorithm begins//" << std::endl;
//     //std::cout << "\n";  
//     REDUCTION_TO_CNF_SAT(g1);
//     //std::cout << "number of vertices: " << g1.adjacency_list.size() << std::endl;
//     return 0;
// }