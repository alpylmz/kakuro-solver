#include <z3++.h>
#include <z3.h>
#include <vector>

using namespace z3;

void kakuro_example(std::vector<std::vector<std::pair<int,int>>> matrix,int len1,int len2){
    std::cout << "kakuro example\n";

    context c;

    expr_vector x(c);
    for (char i = 0; i < len1; i++){
        for (char j = 0; j < len2; j++) {
            std::stringstream x_name;

            x_name << "x_" << i << '_' << j;
            x.push_back(c.int_const(x_name.str().c_str()));
        }
    }
    solver s(c);

    //choose which squares take numbers between 1-9
    for (char i = 0; i < len1; i++){
        for (char j = 0; j < len2; j++) {
            if(matrix[i][j].first == 0 && matrix[i][j].second == 0)
                s.add(x[i * len1 + j] >= 1 && x[i * len2 + j] <= 9);
        }
    }

    //if two numbers are in the same sum, they cannot be the same number
    for(int i=0;i<len1;i++){
        for(int j=0;j<len2;j++){

            if(matrix[i][j].first != -1 && matrix[i][j].first != 0){ //downside
                expr_vector t(c);
                for(int k=i+1;k<len1;k++){
                    if(matrix[k][j].first != 0)
                        break;
                    t.push_back(x[k * len1 + j]);
                }
                s.add(sum(t)==matrix[i][j].first);
                s.add(distinct(t));
            }

            if(matrix[i][j].second != -1 && matrix[i][j].second != 0){ //to right
                expr_vector t(c);
                for(int k=j+1;k<len2;k++){
                    if(matrix[i][k].first != 0)
                        break;
                    t.push_back(x[i * len1 + k]);
                }
                s.add(sum(t)==matrix[i][j].second);
                s.add(distinct(t));
            }
              

        }
    }
    

    s.check();

    model m = s.get_model();
    
    for (unsigned i = 0; i < len1; ++i) {
        for (unsigned j = 0; j < len2; ++j)
            std::cout << m.eval(x[i * len1 + j]) << " ";
        std::cout << '\n';
    }
    
}

int main(){
    std::vector<std::vector<std::pair<int,int>>> matrix = {{std::make_pair(-1,-1),std::make_pair(23,-1),std::make_pair(12,-1),std::make_pair(10,-1)},
                                                        {std::make_pair(-1,24), std::make_pair(0,0), std::make_pair(0,0),std::make_pair(0,0)},
                                                        {std::make_pair(-1,11), std::make_pair(0,0), std::make_pair(0,0),std::make_pair(0,0)},
                                                        {std::make_pair(-1,10),std::make_pair(0,0),std::make_pair(0,0),std::make_pair(0,0)}};
    kakuro_example(matrix,matrix.size(),matrix[0].size());
    return 0;
}