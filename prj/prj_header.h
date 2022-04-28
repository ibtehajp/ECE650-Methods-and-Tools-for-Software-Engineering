#pragma once

class graph{
    public:
        unsigned int total_edges;
        std::vector<std::vector<unsigned int>> adjacency_list;

        void adjacency_list_generator(std::vector<std::vector<int>> a, unsigned int b){
            adjacency_list.resize(b,{});
            for (unsigned int i = 0; i < a.size(); ++i){
                adjacency_list[a[i][0]-1].push_back(a[i][1]-1);
                adjacency_list[a[i][1]-1].push_back(a[i][0]-1);
                total_edges = total_edges + 1;
            }
        }
        void remove_vertex(int a){
            for (unsigned int i = 0; i < adjacency_list[a].size(); ++i ){
                
                adjacency_list[adjacency_list[a][i]].erase((std::find(adjacency_list[adjacency_list[a][i]].begin(),adjacency_list[adjacency_list[a][i]].end(),a)));
                total_edges = total_edges - 1;
            }
            adjacency_list[a].clear();
        }
        void clear(void){
            total_edges = 0;
            adjacency_list.clear();
        }
        void adj_printer(void){
            for (unsigned int i = 0; i< adjacency_list.size(); ++i){
                for (unsigned int j = 0 ; j <adjacency_list[i].size(); ++j){
                    std::cout<< adjacency_list[i][j] << ", ";
                }
                std::cout << "\n";
            }

        }
};

std::string REDUCTION_TO_CNF_SAT(graph g1);
std::string APPROX_VC_1(graph g1);
std::string APPROX_VC_2(graph g1);

void print_approx_ratio(void);

void csv_printer(int v, double a, double b, double c);