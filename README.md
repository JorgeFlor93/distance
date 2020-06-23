This script maps an area in latitude and longitude format. 
[Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page) library has been used to manipulate vectors in c++
add Eigen folder to path /usr/local/include/ for easy compile.

to test:
	
simple to compile with g++ -Wall -g distance.cc -o distance

	./distance area 55.895017 -3.255537 55.893994 -3.248520

where first parameter indicates mode (area o line). 2 area points next parameters: top_left_latitude, top_left_longitude, bot_right_latitude and bot_right_longitude correspondingly.
	
line, mean point to point: start point and end_point lat and lng parameters.

	./distance line 55.895017 -3.255537 55.893994 -3.248520 

	
