#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <functional>
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
/*
def get_line(line_start_lat, line_start_lng, line_end_lat, line_end_lng):

# """
#
# List of coordinates inside a line between the given ones
#
# @param line_start_lat: defines the line coordinates (start lat of the line)
#
# @param line_start_lng: defines the line coordinates (start lng of the line)
#
# @param line_end_lat: defines the line coordinates (end lat of the line)
#
# @param line_end_lng: defines the line coordinates (end lng of the line)
#
# @return point_list: list of coordinates, each coordinate is a tuple in the form of (lat, lang)
#
# """

start_point = np.array([line_start_lat, line_start_lng])

    end_point = np.array([line_end_lat, line_end_lng])

    full_vector = np.subtract(end_point, start_point)

    module = np.linalg.norm(full_vector)

    res_vector = np.array([resolution['lat_res'], resolution['lng_res']])

    unit_vector = np.multiply(np.divide(full_vector, module), res_vector)

    current_point = np.copy(start_point)

    end = int(round(Decimal(module) / Decimal(res_vector[0])))

    point_list = []

    point_list.append((current_point[0], current_point[1]))

    for _ in range(end):
        current_point = np.add(current_point, unit_vector)
        point_list.append((current_point[0], current_point[1]))

    return point_list
*/