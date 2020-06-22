import numpy as np

# USED FOR VECTOR MATH

from decimal import *

# Critical floating point operations must be done with this data type in python. (Other languages like js or c++ uses this format by default)

# Obtained from GDAL SRTM
# 0.00208333333 -> 225m
# 0.0002777778 -> 30m
# 0.000458333 -> 50m

resolution = {'lat_res': 0.0004583333, 'lng_res': 0.0004583333}


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
    print(str(end) + "\n")
    point_list = []

    point_list.append((current_point[0], current_point[1]))

    for _ in range(end):
        current_point = np.add(current_point, unit_vector)
        point_list.append((current_point[0], current_point[1]))

    return point_list


def get_dimensions(top_lat, top_lng, bot_lat, bot_lng):

# """
#
# Gets the amount of "cells" between the given coordinates on the GlobalGrid
#
# @param top_lat: top left lat coordinate of the area
#
# @param top_lng: top left lng coordinate of the area
#
# @param bot_lat: bot right lat coordinate of the area
#
# @param bot_lng: bot right lng coordinate of the area
#
# @return (0) Amount of cells in lat axis
#
# @return (1) Amount of cells in lng axis
#
# @return (2) Used resolution (other files might not have access to the global resolution)
#
# """

    inc_lat = abs(top_lat - bot_lat)

    inc_lng = abs(top_lng - bot_lng)

    return int(round(Decimal(inc_lat)/Decimal(resolution['lat_res']))), int(round(Decimal(inc_lng)/Decimal(resolution['lng_res']))), resolution


def get_area(top_lat, top_lng, bot_lat, bot_lng):

# """
#
# List of coordinates inside an area between the given ones
#
# @param top_lat: top left lat coordinate of the area
#
# @param top_lng: top left lng coordinate of the area
#
# @param bot_lat: bot right lat coordinate of the area
#
# @param bot_lng: bot right lng coordinate of the area
#
# @return data: list of coordinates, each coordinate is a tuple in the form of (lat, lang)
#
# """

    amount_lat, amount_lng, resolution = get_dimensions(top_lat, top_lng, bot_lat, bot_lng)

    data = []

    start_lng = top_lng + resolution['lng_res']/2

    current_lat = top_lat - resolution['lat_res']/2

    current_lng = start_lng

    for _ in range(amount_lat):
        for _ in range(amount_lng):
            data.append((current_lat, current_lng))
            current_lng += resolution['lng_res']
        current_lng = start_lng
        current_lat -= resolution['lat_res']

    return data


def Convert_decimal_to_meters(diff):

    # Conversion to dms
    d = round(Decimal(diff))
    m = round((diff - d) * 60)
    s = (Decimal(diff) - Decimal(d) - Decimal(m) / Decimal(60)) * Decimal(3600)

    # Conversion dms to meters -> equivalence: 1º = 111.32km, 1' = 1.85km, 1" = 30.9m
    meters = d * Decimal(111320) + m * Decimal(1850) + s * Decimal(30.9)

    return meters

def get_height(top_lat, bot_lat):

    height = Decimal(top_lat) - Decimal(bot_lat)
    height = Convert_decimal_to_meters(height)

    return height

# DEMO

if __name__ == '__main__':

    import sys

    arguments = sys.argv
    result = []
    if len(arguments) == 6:
        _, arg_type, top_lat, top_lng, bot_lat, bot_lng = arguments
    else:
        arg_type = 'area'
        top_lat, top_lng, bot_lat, bot_lng = [0.01, 0, 0, 0.01]
#       print(f'Calculating {arg_type} between ({top_lat}, {top_lng}) and ({bot_lat},{bot_lng})')
        result = 'Invalid arguments'

    height_meters = get_height(top_lat, bot_lat)
    #print("\n" + str(height_meters) + "\n")

    if arg_type == 'area':
        result = get_area(float(top_lat), float(top_lng), float(bot_lat), float(bot_lng))
    elif arg_type == 'line':
        result = get_line(float(top_lat), float(top_lng), float(bot_lat), float(bot_lng))

    print(result)
