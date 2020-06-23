#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <Eigen/Core>
#include <Eigen/Dense>

/* Resolution */ 
// 0.00208333333 -> 225m
// 0.0002777778 -> 30m
// 0.000458333 -> 50m
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

int get_dimension_lng(double line_start_lng, double line_end_lng){
    double inc_lng = 0.0;

    inc_lng = abs(line_start_lng - line_end_lng);

    return round(inc_lng/lng_res);
}

int get_dimension_lat(double line_start_lat, double line_end_lat){
    double inc_lat = 0.0;

    inc_lat = abs(line_start_lat - line_end_lat);

    return round(inc_lat/lat_res);
}

std::vector<Eigen::Matrix<double, 1, 2>> get_area(double top_lat , double top_lng, double bot_lat, double bot_lng){
    
    //get lat and lng dimension
    int amount_lat = 0;
    int amount_lng = 0;
    amount_lat = get_dimension_lat(top_lat, bot_lat);
    amount_lng = get_dimension_lng(top_lng, bot_lng);

    std::vector<Eigen::Matrix<double, 1, 2>> data;
    Eigen::Matrix<double, 1, 2> start_point;
    Eigen::Matrix<double, 1, 2> current_point;

    start_point(0,1) = top_lng + lng_res/2;
    current_point(0,0) = top_lat - lat_res/2;
    current_point(0,1) = start_point(0,1);

    for(int i = 0; i < amount_lat; i++){
        for(int j = 0; j < amount_lng; j++){
            data.push_back(current_point);
            current_point(0,1) += lng_res;
        }
        current_point(0,1) = start_point(0,1);
        current_point(0,0) -= lat_res;
    }

    return data;
}

int main(int argc, const char *argv[]){ // ./distance area 55.895017 -3.255537 55.893994 -3.248520
                                       
    
    double top_lat = atof(argv[2]); double top_lng = atof(argv[3]);
    double bot_lat = atof(argv[4]); double bot_lng = atof(argv[5]);
    std::vector<std::string> mode;

    // std::cout << "top_lat-lng: [" << top_lat << ", " << top_lng << "], bot_lat-lng: [" << bot_lat << ", " << bot_lng << "]" <<"\n";
    
    std::vector<Eigen::Matrix<double, 1, 2>> point_list;
    
    mode.push_back(argv[1]);

    if(mode[0] == "line")
        point_list = get_line(top_lat, top_lng, bot_lat, bot_lng);

    else if(mode[0] == "area")
        point_list = get_area(top_lat, top_lng, bot_lat, bot_lng); 

    else
        std::cout << "Wrong inputs" << std::endl;
    

    std::cout.precision(12);
    for(int j = 0; j < point_list.size(); j++)
        std::cout <<"[" << std::fixed << point_list[j] << "] ";
    
    /* Accediendo al elemento 7->long: */
        // std::cout << std::endl;
        // std::cout << point_list[7](0,1);
        // std::cout << std::endl;

    return 0;
}
