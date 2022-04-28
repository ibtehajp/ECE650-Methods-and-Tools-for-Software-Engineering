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
        void clear(void){
            total_edges =  0;
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

void REDUCTION_TO_CNF_SAT(graph g1);