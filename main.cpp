#include "traffic.h"

AdjType a[STATION_NUM][STATION_NUM];

int trip_id;

via_data via[VIA_MAX];
//path_stor pa[PATHS_MAX];
station_data sta[STATION_NUM+1];

int path[STATION_NUM][STATION_NUM];//v到各顶点的最短路径向量
int D[STATION_NUM][STATION_NUM];//v到各顶点最短路径长度向量

ifstream subway_data_in;
ifstream passenger_data_in;

MGraph G;

via_data v[TRIP_MAX][VIA_TRIP_MAX];

int numintrip[TRIP_MAX]={0};

int status_trip[TRIP_MAX]={-1};//-1:未处理  0:34号线缺OD舍弃
int trip_path[TRIP_MAX][VIA_TRIP_MAX];


/*void via_data::path_analysis(int path[][STATION_NUM],int D[][STATION_NUM])//路径分析
{
	for (int i = 0, j, k, trip_id = via[1].trip_id; i < VIA_DATA_MAX; i += j+1)
	{
		pa[trip_id].station[0] = station;

		for (j = 0,k=1; i <= VIA_DATA_MAX && via[i+j+1].trip_id == trip_id; j++)
		{
			int temp = path[via[i + j].station][via[i + j + 1].station];
			for (; temp != via[i + j + 1].station; k++)
			{
				pa[trip_id].station[j + k] = temp;
				temp = path[temp][via[i + j + 1].station];
			}
			pa[trip_id].station[j + k] = temp;
		}//只有到了trip终点时才会退出此循环
		pa[trip_id].number=j+k;
		trip_id++;

		//时间未算

	}
}*/

/*void via_data::test_print_path()
{
	for(int i=0;i<trip_id;i++)
	{
		for(int j=0;j<pa[i].number;j++)
		{
			printf("%d :",trip_id);
			if(j<pa[i].number-1)
				printf("%s->",sta[pa[i].station[j]].name);
			else
				printf("%s",sta[pa[i].station[j]].name);
		}
	}
}*/



int main() {
	subway_data_in.open(subway_data, ios::in);
	if(!subway_data_in.is_open())
		return -1;
	for(int i = 1; i <= STATION_NUM; i++)
		sta[i].read_subway_data(i);
	subway_data_in.close();
	for(int i = 0; i < STATION_NUM; i++)
		sta[i].init_adjacency_matrix(i);
	passenger_data_in.open(passenger_data, ios::in);
	if(!passenger_data_in.is_open())
		return -1;
	for (int i = 0; i <= VIA_MAX; i++)
		via[i].read_passenger_data(i);
	passenger_data_in.close();
	init_MGraph();
	Floyd(&G,path,D,STATION_NUM);
	if(CALCULATEPATH)
		calculate_path(path,D);
	else
		read_path();
	if(DEBUGVIA)
		via[DEVIA].test_print_via();

	init_array();
	cut();
	init_station34();
	if(DEBUGFINDTRANS)
		test_find_trans(DEBUGFINDTRANS);
	else
		scan(TRIP_MAX);

    return 0;
}


