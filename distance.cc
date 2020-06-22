#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Dense>

double lat_res = 0.0004583333;
double lng_res = 0.0004583333;

std::vector<Eigen::Matrix<double, 1, 2>> get_line(double line_start_lat , double line_start_lng, double line_end_lat, double line_end_lng){

    int end = 0;
    double module = 0.0;

    Eigen::Matrix<double, 1, 2> start_point; // Matrix(type, rows, col)
    start_point(0,0) = line_start_lat;
    start_point(0,1) = line_start_lng;

    Eigen::Matrix<double, 1, 2> end_point;
    end_point(0,0) = line_end_lat;
    end_point(0,1) = line_end_lng;

    Eigen::Matrix<double, 1, 2> full_vector;
    full_vector (0, 0) =  line_end_lat - line_start_lat; 
    full_vector (0, 1) =  line_end_lng -line_start_lng;

    module = full_vector.norm();

    Eigen::Matrix<double, 1, 2> res_vector;
    res_vector(0,0) = lat_res;
    res_vector(0,1) = lng_res;


    Eigen::Matrix<double, 1, 2> unit_vector;
    unit_vector(0,0) = (full_vector(0,0) / module) * lat_res;
    unit_vector(0,1) = (full_vector(0,1) / module) * lng_res;

    Eigen::Matrix<double, 1, 2> current_point = start_point;

    end = round((module)/(lat_res));

    std::vector<Eigen::Matrix<double, 1, 2>> point_list;
    point_list.push_back(current_point);

    for(int i=0;i<end;i++){
        current_point = current_point + unit_vector;
        point_list.push_back(current_point);
    }
    
   return point_list;
}

int main(int argc, char *argv[]){ // ./distance 55.895017 -3.255537 55.893994 -3.248520

    //std::vector<double> coord;

    double top_lat = atof(argv[1]); double top_lng = atof(argv[2]);
    double bot_lat = atof(argv[3]); double bot_lng = atof(argv[4]);
    // std::cout << "top_lat-lng: [" << top_lat << ", " << top_lng << "], bot_lat-lng: [" << bot_lat << ", " << bot_lng << "]" <<"\n";
    std::vector<Eigen::Matrix<double, 1, 2>> point_list;
    point_list = get_line(top_lat, top_lng, bot_lat, bot_lng);

    std::cout.precision(15);
    for(int j = 0; j < point_list.size(); j++){
        std::cout << std::fixed << point_list[j] << "; ";
    }
    
    //Accediendo al elemento 7->long:
    std::cout << std::endl;
    std::cout << point_list[7](0,1);
    std::cout << std::endl;

    return 0;
}
