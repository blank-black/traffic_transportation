#include "traffic.h"

int station34[STATION_NUM_34];

via_data::via_data()
{
	id=0;
	trip_id=0;
	strcpy(MAC,"");
	time=0;
	year=0;
	month=0;
	day=0;
	station=0;
	pos=0;
	line=0;
	car_id=0;
	train_id=0;
}

via_data::~via_data()
{
	id=0;
	trip_id=0;
	strcpy(MAC,"");
	time=0;
	year=0;
	month=0;
	day=0;
	station=0;
	pos=0;
	line=0;
	car_id=0;
	train_id=0;
}

void station_data::init_adjacency_matrix(int i)//初始化邻接矩阵
{
	for (int j = 1; j <= STATION_NUM; j++)
	{
		a[i][j] = MAX_INT;
		a[i][i] = 0;
		if (j != i)
			for (int k = 0; k < 2 * change_num; k++)
				if (j == station_adjacent[k])
				{
					a[i][j] = station_adjacent_distance[k];
					break;
				}
	}
}

void init_MGraph()//初始化图
{
	for(int i=0;i<STATION_NUM;i++)
		for(int j=0;j<STATION_NUM;j++)
			G.A[i][j] = a[i][j];
}

void calculate_path(int path[][STATION_NUM],int D[][STATION_NUM])
{
	ofstream subway_data_out;
	subway_data_out.open(path_result, ios::out);
	for (int i = 0, k; i < STATION_NUM; i++)//输出每对顶点间最短路径长度及最短路径
	{
		for (int j = 0; j < STATION_NUM; j++)
		{
			if (DEBUGPATH)
				printf("%s 到 %s 的最短长度:", sta[i].name, sta[j].name);
			if (DEBUGPATH)
				printf("%d\t", D[i][j]);//输出Vi到Vj的最短路径长度
			subway_data_out << D[i][j] << " ";
			k = path[i][j];//取路径上Vi的后续Vk
			if (k == -1&&DEBUGPATH)
				printf("There is no path between V%d and V%d\n", i, j);//路径不存在
			else
			{
				if (DEBUGPATH)
					printf("(%s", sta[i].name);//起点
				while (k != j)
				{//k不等于路径终点j时
					if (DEBUGPATH)
						printf("->%s", sta[k].name);//途径站
					k = path[k][j];//求路径上下一顶点序号
				}
				if (DEBUGPATH)
					printf("->%s)\n", sta[j].name);//终点
			}
			if (DEBUGPATH)
				printf("\n");

		}
		subway_data_out << endl;
	}
}

void init_array()
{
	for(int i=0;i<TRIP_MAX;i++)
		for(int j=0;j<VIA_TRIP_MAX;j++)
			trip_path[i][j]=-1;

}

void init_station34()
{
	int st=0;
	for(int i=0;i<STATION_NUM;i++)
	{
		for(int j=0;j<2*sta[i].change_num;j++)
			if(sta[i].line_num[j]==LINE1||sta[i].line_num[j]==LINE2)
			{
				station34[st]=i;
				st++;
			}
	}
}

//2-3

int cut()
{
	int trip=1,i=-1,num=0;

	for(int j=0;j<VIA_MAX;j++,i++)
	{
		if(via[j].trip_id!=trip)
		{
			trip=via[j].trip_id;
			numintrip[num]=i+1;//第num个trip有i条数据
			i=-1;
			num++;
		}
	}
	for(int j=0;j<DEBUGCUT;j++)
		cout<<numintrip[j]<<endl;
	return num+1;
}
