#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <typeinfo>

std::vector<std::vector<int>> all_points;
std::vector<std::vector<std::vector<int>>> all_segments;
int colinear_count = 2;

int create_segment_list (std::vector<std::vector<int>> a){ //working version
    for (unsigned i = 0 ; i < (a.size()-1); ++i){

        all_segments.push_back({a[i],a[i+1]});
        
    }
    return 0;
}

class street{
    public:
        std::string name;
        unsigned int street_number;
        std::vector<std::vector<int>> edge_list;

        void info(std::string a, unsigned int b, std::vector<std::vector<int>> c){
            name = a;
            street_number = b;
            edge_list = c;
            all_points.insert(all_points.end(),c.begin(),c.end());
            create_segment_list(edge_list);

            return;
        }
};

std::vector<std::string> street_name(unsigned int num){
    std::vector<std::string> street_name_list;
    std::string name = "street";

    for (unsigned int i=0; i < num; i++)
    {
        name = name +"s";
        street_name_list.push_back(name);
    }
    return street_name_list;
}

unsigned int street_number(unsigned int s){
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    
    unsigned int random = 42;
    urandom.read((char*)&random, sizeof(unsigned int));
    random = (random % (s-1)) + 2; // to make sure that "random" is always between 2 and s
    urandom.close();
    return (unsigned int) random;
}

unsigned int street_segments(unsigned int n){
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    unsigned int random = 42;
    urandom.read((char*)&random, sizeof(unsigned int));
    random = (random % n) + 2;
    urandom.close();
    return (unsigned int) random;
}

unsigned int time_to_wait(unsigned int l){
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    unsigned int time = 42;
    urandom.read((char*)&time, sizeof(unsigned int)); 
    time = (time % (l-4)) + 5; // to make sure that "l" is always between 5 and l
    urandom.close();
    return (unsigned int) time;
}



int coordinate_x (unsigned int k)
{
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom for x\n";
        return 1;
    }
    unsigned int rand = 41;
    urandom.read((char*)&rand, sizeof(int));
    rand = rand % (k+k);
    int x_cord = rand - (k) ;
    urandom.close();
    return x_cord;
}

int coordinate_y (unsigned int k)
{
    // open /dev/urandom to read
    std::ifstream urandom("/dev/urandom");

    // check that it did not fail
    if (urandom.fail()) {
        std::cerr << "Error: cannot open /dev/urandom for y\n";
        return 1;
    }
    unsigned int rand = 41;
    urandom.read((char*)&rand, sizeof(int));
    rand = rand % (k+k); // to generate a maximum value of twice the size of k
    int y_cord = rand - (k) ; // to shift the values in case 
    urandom.close();
    return y_cord;
}

bool onSegment(std::vector<int> p, std::vector<int> q, std::vector<int> r)
{
    if (q[0] <= std::max(p[0], r[0]) && q[0] >= std::min(p[0], r[0]) &&
        q[1] <= std::max(p[1], r[1]) && q[1] >= std::min(p[1], r[1]))
       return true;
 
    return false;
}
 
int orientation(std::vector<int> p, std::vector<int> q, std::vector<int> r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q[1] - p[1]) * (r[0] - q[0]) -
              (q[0] - p[0]) * (r[1] - q[1]);
 
    if (val == 0) return 0;  // collinear
 
    return (val > 0)? 1: 2; // clock or counterclock wise
}
 
// The main function that returns true if line segment 'p1q1' and 'p2q2' intersect.
bool doIntersect(std::vector<int> p1, std::vector<int> q1, std::vector<int> p2, std::vector<int> q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);
 
    // General case
    if (o1 != o2 && o3 != o4)
        return true;
 
    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;
 
    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;
 
    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;
 
     // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;
 
    return false; // Doesn't fall in any of the above cases
}


int check_duplicate(std::vector<std::vector<int>> a, std::vector<int> b){ // compares it against all the street
    for (unsigned int i = 0 ; i < a.size(); ++i){
        if (a[i] == b){
            return 1;
        }
    }
    return 0;
}

int collinear(std::vector<int> vec1, std::vector<int> vec2, std::vector<int> vec3){
    int a = vec1[0] * (vec2[1] - vec3[1]) +
            vec2[0] * (vec3[1] - vec1[1]) +
            vec3[0] * (vec1[1] - vec2[1]);
 
    if (a == 0)
        return 1;
    else
        return 0;
}

int check_segment_collinear (std::vector<int> a){
    if (colinear_count % 2 == 0){
        
        if (all_segments.size() == 0){
            return 0;
        }
        
        for (unsigned int i = 0 ; i < all_segments.size(); ++i){
            if(collinear(all_segments[i][0],all_segments[i][1], a) == 1){
                return 1;
            }
        }
        colinear_count = colinear_count +1; 
        return 0;
    }
    else{
        colinear_count = colinear_count +1;
        return 0;
    }
}

int check_segment_intersect(std::vector<std::vector<int>> a, std::vector<int> b){
    if (a.size() <= 2){
        return 0;
    }
    
    else{
        for (unsigned int j = 0 ; j < (a.size()-2); ++j){
            if(doIntersect( a[j],a[j+1],a[(a.size()-1)],b)==1){
                return 1;
            }

            // else if (collinear(a[(a.size()-2)],a[(a.size()-1)],b)==1){
            //     return 1;
            // }
        }
    }
    return 0; 
}


std::vector<std::vector<int>> coordinate_list(unsigned int n,  int c){
    
    std::vector<std::vector<int>> cord_list;
        int error = 0;
    // std::vector<int> a = {3,8};
    // cord_list = {{2,-1}, {2,2}};
    // cord_list.push_back(a);

    for (unsigned int i = 0 ; i != n ; ++i){
        if (error == 100){
           std::cerr << "Error: failed to generate valid input after 50 simultaneous attempts" << std::endl;
           std::cout << "quitquit" << std::endl;
            exit(1);
    }

        int x = coordinate_x(c);
        int y = coordinate_y(c);
        std::vector<int> n_cord = {x,y};
        // std::cout << cord_list[cord_list.size()-1][0] << "," << cord_list[cord_list.size()-1][1]<< std::endl; 
               
        
        if (check_duplicate(cord_list,n_cord) == 1 || check_segment_intersect(cord_list,n_cord)== 1 || (check_duplicate(all_points, n_cord)==1) || check_segment_collinear(n_cord) == 1){
            //std::cout<< error<< std::endl;
            error = error + 1;
            i = i -1;
        }


        else{
            cord_list.push_back(n_cord);
        }   
    }
    
    return cord_list;
}


void add_command(street obj){ // only for printing the add command
    
    std:: string coord_list;

    for (unsigned int i = 0; i != obj.edge_list.size(); ++i){
        coord_list = coord_list + " (" + std::to_string(obj.edge_list[i][0]) + "," + std::to_string(obj.edge_list[i][1]) + ")";
    }

    std::cout << "add \"" <<obj.name << "\"" << coord_list << "\n";

    return;
}


void rm_command(street obj){ // only for printing the rm command

    std::cout << "rm \"" <<obj.name << "\"" << std::endl;
    return;
}

// int duplicate_check(vector<street> a){
//         for (unsigned int i = 0; i < nstreets; ++i){
//             for (unsigned int j = 0; j < home[i].edge_list.size(); ++j){
//                 for (unsigned int k = 0; k < home[i+1].edge_list.size(); ++k){
//                     if (home[i].edge_list[j] == home[i+1].edge_list[k]){
//                         delete[] home;
//                         continue;
//                     }
//         }

//             }
//         }
// }

int main (int argc, char **argv) {
    int s_int = 10;
    int n_int = 5;
    int l_int = 5;
    int c_int = 20;
    int c;

    std::string s_command;
    std::string n_command;
    std::string l_command;
    std::string c_command;

    while ((c = getopt(argc, argv, "s:n:l:c:")) != -1) { // this loop takes the arguments for the 4 variables
        switch (c) {
            case 's': // number of streets
                s_command = optarg;
                s_int = std::atoi(s_command.c_str());
                if (s_int < 2)
                {
                    std::cerr << "Error: the value of s is smaller than 2"<<std::endl;
                    exit(1);
                }
                break;

            case 'n': // number of line segments in a street
                n_command = optarg;
                n_int = std::atoi(n_command.c_str());
                if (n_int < 1)
                {
                    std::cerr << "Error: the value of n is smaller than 1"<<std::endl;
                    exit(1);
                }
                break;

            case 'l': // waiting time in seconds
                l_command = optarg;
                l_int = std::atoi(l_command.c_str());
                if (l_int < 5)
                {
                    std::cerr << "Error: the value of l is smaller than 5"<<std::endl;
                    exit(1);
                }
                break;

            case 'c': // range for generating the coordinates
                c_command = optarg;
                c_int = std::atoi(c_command.c_str());
                if (c_int < 1)
                {
                    std::cerr << "Error: the value of n is smaller than 1"<<std::endl;
                    exit(1);
                }
                break;

            case '?': // catches the errors for the argument
                if (optopt == 's') {
                    std::cerr << "Error: option -" << optopt << " requires an argument." << std::endl;
                    exit(1);
                    break;
                }
                else if (optopt == 'n') {
                    std::cerr << "Error: option -" << optopt << " requires an argument." << std::endl;
                    exit(1);
                    break;
                }
                else if (optopt == 'l') {
                    std::cerr << "Error: option -" << optopt << " requires an argument." << std::endl;
                    exit(1);
                    break;
                }
                else if (optopt == 'c') {
                    std::cerr << "Error: option -" << optopt << " requires an argument." << std::endl;
                    exit(1);
                    break;
                }
                else{ 
                    std::cerr << "Error: unknown option: " << optopt << std::endl;
                    exit(1);
                    break;
                }
        }
    }
    
    while(true){
        unsigned int nstreets = street_number(s_int);

        street* home = new street[nstreets];

        std::vector<std::string> name_list = street_name(nstreets);

        for (unsigned int i = 0; i < nstreets; ++i){
            
            home[i].info(name_list[i], i , coordinate_list(street_segments(n_int),c_int));
            
        }

        for (unsigned int i = 0; i < nstreets; ++i){
            
            add_command(home[i]);
            //add add command with the name

        }

        // std::cout << "add \"King Street S\" (4,2) (4,8)" << std::endl;
        // std::cout << "add \"Davenport Road\" (1,4) (5,8)" <<std::endl;
        std::cout << "gg" << std::endl;
        
        sleep(time_to_wait(l_int)); // going to sleep

        for (unsigned int i = 0; i < nstreets; ++i){ // remove the streets from python

            rm_command(home[i]);
            //add rm command with the name

        }
        all_points.clear();
        all_segments.clear();
        colinear_count = 2;
        delete[] home; // delete the heap array
    }

    return 0;
}