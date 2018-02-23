#include "traffic.h"


int station_data::read_subway_data(int i)
{
	subway_data_in >> number >> name >> change_num;//序号 站名 换乘线路数量
	if (number != i )
		cout << i << "error" << endl;
	for (int j = 0; j < change_num; j++)//换乘线路
		subway_data_in >> line_num[j];
	for (int j = 0; j < change_num; j++)//相邻站
		subway_data_in >> station_adjacent[2 * j] >> station_adjacent[2 * j + 1];
	//if (!DEBUG)
	for (int j = 0; j < 2 * change_num; j++)//相邻时间已给出
		subway_data_in >> station_adjacent_distance[j];
	/*
		for (int j = 0; j < 2 * change_num; j++)//假设相邻时间相等
			station_adjacent_distance[j] = 1;*/
	return 0;
}

void via_data::read_passenger_data(int i)
{
	char capture_date[TIME_LENGTH], capture_time[TIME_LENGTH], station_name[STATION_NAME_MAX], station_pos[POSITION];
	passenger_data_in >> id >> trip_id;//ID TRIP_ID
	passenger_data_in >> MAC;
	passenger_data_in >> capture_date >> capture_time >> station_name;
	if (strcmp(station_name, "列车"))
	{
		passenger_data_in >> station_pos;
		for (int j = 0; j < STATION_NUM; j++)//转换站名为站号
			if (!strcmp(station_name, sta[j].name))
				station = sta[j].number;
		pos = 1;
		passenger_data_in >> line;
		car_id = 0;
		train_id = 0;
	}
	else
	{
		station = 0;//若在列车上无站名置0
		pos = 0;
		passenger_data_in >> line >> car_id >> train_id;
	}

	//转换时间
	year = capture_date[0] * 1000 + capture_date[1] * 100 + capture_date[2] * 10 + capture_date[3] - 1111 * '0';
	month = capture_date[5] * 10 + capture_date[6] - 11 * '0';
	day = capture_date[8] * 10 + capture_date[9] - 11 * '0';
	time = capture_time[0] * 600 + capture_time[1] * 60 + capture_time[3] * 10 + capture_time[4] - 671 * '0';

}

void read_path()
{
	ifstream path_data_in;
	path_data_in.open(path_result, ios::in);
	for(int i=0;i<STATION_NUM;i++)
		for(int j=0;j<STATION_NUM;j++)
			path_data_in>>D[i][j];
	path_data_in.close();
}
