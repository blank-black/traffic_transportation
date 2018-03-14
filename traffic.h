#ifndef TRAFFIC_TRANSPORTAIN_TRAFFIC_H
#define TRAFFIC_TRANSPORTAIN_TRAFFIC_H

#endif //TRAFFIC_TRANSPORTAIN_TRAFFIC_H

#include <iostream>
#include <fstream>
#include <stdio.h>

using namespace std;

#define STATION_NAME_MAX 30
#define STATION_LINE_MAX 4
#define STATION_ADJACENT_NUM 8
#define STATION_NUM 305
#define TIME_LENGTH 17
#define MAC_LENGTH 30
#define VIA_MAX 28452
#define PATHS_MAX 1000
#define POSITION 6
#define MAX_INT 100000 //当做初始无穷大
#define VIA_TRIP_MAX 500
#define TRIP_MAX 1400
#define STATION_NUM_34 60
#define SPLITMAX 50


#define STATUS_NO_OD 0
#define STATUS_COMPLETE 1
#define STATUS_FIND_TRANS 2
#define STATUS_ERROR 3
#define STATUS_OD_SAME 4
#define STATUS_NO34 5
#define STATUS_FULL 6

#define LINE1 3
#define LINE2 4

//DEBUG
#define DELERROR 0 //是否删除预测路径和实际路径不匹配
#define DEBUGPATH 0 //是否输出任意两站最短路径
#define DEBUGVIA 0 //是否输出passenger
#define DEVIA 4 //输出第n条passenger信息
#define CALCULATEPATH 1 //计算任意两站最短距离
#define DEBUGERROR 0 //是否输出错误状态
#define DEBUGNOOD 0 //是否输出no OD情况
#define DEBUGSCAN 900 //测试前n组trip_id 0为正常运行
#define DEBUGCUT 0 //输出前n组numintrip
#define DEBUGFINDTRANS 0
#define DEBUGSTOP 0 //在第n次num_t停止
#define DELNOOD 0 //删除缺少OD



class station_data {
private:
	int change_num;
	int line_num[STATION_LINE_MAX];
	int station_adjacent[STATION_ADJACENT_NUM];
	int station_adjacent_distance[STATION_ADJACENT_NUM];
public:
	int number;
	char name[STATION_NAME_MAX];
	int read_subway_data(int i);
	void init_adjacency_matrix(int i);
	friend void calculate_path(int path[][STATION_NUM],int D[][STATION_NUM]);//全部可能路径输出测试
	friend void init_station34();
	int find_next_sta(int end_sta,int line = 0);
	friend void scan(int cut);
	friend int if_34_station(int k,int i);
	friend int fun_find_trans(int trip, int s34, int sn34, int t34, int tn34);//找换乘
	friend int test_find_trans(int num);
	friend int del_error_trip(int trip, int start_num);
	friend int right_trip(int trip, int start_num);
	bool if_trans(int station);
	friend int trip_adjust(int trip ,int start_num ,int tail ,int head_num ,int tail_num);
	friend void scan_n(int cut);


};

class via_data{
private:
	int id;
	int trip_id;
	char MAC[MAC_LENGTH];
	int time;
	int year;
	int month;
	int day;
	int station;//需要从char转换
	bool pos;//0列车,1站台/站厅储存
	int line;//线号
	int car_id;
	int train_id;
public:
	via_data();
	~via_data();
	void read_passenger_data(int i);
	void path_analysis(int path[][STATION_NUM],int D[][STATION_NUM]);//路径分析
	void test_print_path();
	void test_print_via();
	friend void scan(int cut);
	friend int cut();
	friend int if_34_station(int k,int i);
	friend int fun_find_trans(int trip, int s34, int sn34, int t34, int tn34);//找换乘
	friend int test_find_trans(int num);
	friend int del_error_trip(int trip, int start_num);
	friend int right_trip(int trip, int start_num);
	friend int trip_adjust(int trip ,int start_num ,int tail ,int head_num ,int tail_num);
	friend void scan_n(int cut);


};

//class path_stor{
//public:
//	int number;//经过站数
//	int station[VIA_MAX];//经过站号
//	int time_in[VIA_MAX];//到达每站的时间(按照时刻表匹配)
//	int time_out[VIA_MAX];//离开每站的时间(按照时刻表匹配)
//	int year;
//	int month;
//	int day;
//
//};

typedef int AdjType;

typedef struct{
	int pi[STATION_NUM];//存放v到vi的一条最短路径
	int end;
}PathType;

typedef short VType; //设顶点为short类型

typedef struct{
	VType V[STATION_NUM]; //顶点存储空间
	AdjType A[STATION_NUM][STATION_NUM]; //邻接矩阵
}MGraph;//邻接矩阵表示的图

static char subway_data[]="/data/jk_data/subway.txt";
static char passenger_data[]="/data/jk_data/passenger.txt";
static char path_result[]="/data/jk_data/path_result";
static char scan_result[]="/data/jk_data/scan_result";


extern AdjType a[STATION_NUM][STATION_NUM];
extern via_data via[VIA_MAX];
//extern path_stor pa[PATHS_MAX];
extern station_data sta[STATION_NUM];
extern int path[STATION_NUM][STATION_NUM];//v到各顶点的最短路径向量
extern int D[STATION_NUM][STATION_NUM];//v到各顶点最短路径长度向量
extern ifstream subway_data_in;
extern ifstream passenger_data_in;
extern MGraph G;
extern via_data v[TRIP_MAX][VIA_TRIP_MAX];
extern int numintrip[TRIP_MAX];
extern int status_trip[TRIP_MAX];
extern int trip_path[TRIP_MAX][VIA_TRIP_MAX];
extern int station34[STATION_NUM_34];
extern int trip_path_true[TRIP_MAX][VIA_TRIP_MAX];


void Floyd(MGraph * G,int path[][STATION_NUM],int D[][STATION_NUM],int n);
void test_print_all_path(int path[][STATION_NUM],int D[][STATION_NUM]);//全部可能路径输出测试
void init_MGraph();//初始化图
void read_path();
void calculate_path(int path[][STATION_NUM],int D[][STATION_NUM]);
int cut();
void scan(int cut);
void fun_del(int trip, int status=STATUS_NO_OD);
void init_station34();
int fun_complete(int trip, int head, int tail,int status=STATUS_COMPLETE);//完整
int fun_find_trans(int trip, int s34, int sn34, int t34, int tn34);//找换乘
int if_34_station(int k,int i=0);
void test_print_scan(int trip_max);
void init_array();
int test_find_trans(int num);
bool if_trans(int station);
int del_error_trip(int trip, int start_num);
int right_trip(int trip, int start_num);
int if_in_path(int trip, int sta_via, int j);
int trip_adjust(int trip ,int start_num ,int tail ,int head_num ,int tail_num);
void clear_path(int trip ,int j);
int short_line(int trip, int head, int tail, int j);
void scan_n(int cut);
void split_fun(int trip, int split[][VIA_TRIP_MAX], int split_num[], int h_num, int t_num, int s_num[][SPLITMAX]);
void interaction();
