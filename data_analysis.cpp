#include "traffic.h"


//Floyd算法 求网G（用邻接矩阵表示）中任意两点间最短路径 D[][]是最短路径长度矩阵，path[][]最短路径标志矩阵
void Floyd(MGraph * G,int path[][STATION_NUM],int D[][STATION_NUM],int n)
{
	int i, j, k;
	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
		{
			D[i][j]=a[i][j];
			path[i][j]=j;
		}

	for (k = 0; k < n; k++)
		for (i = 0; i < n; i++)
			for (j = 0; j < n; j++)
				if (D[i][j] > D[i][k] + D[k][j])
				{
					D[i][j] = D[i][k] + D[k][j];//取小者
					path[i][j] = path[i][k];//改Vi的后继
				}
}


int station_data::find_next_sta(int end_sta,int line)
{
	if(line)
	{
		int num_line;
		if(change_num==1)
			num_line=0;
		else
			for(int i=0;i<STATION_LINE_MAX;i++)
				if(line==line_num[i])
					num_line=i;
		if(!station_adjacent[2*num_line+1])
			return station_adjacent[2*num_line];
		else if(D[station_adjacent[2*num_line]][end_sta]<D[station_adjacent[2*num_line+1]][end_sta])
			return station_adjacent[2*num_line];
		return station_adjacent[2*num_line+1];
	}
	else
	{
		int min = D[station_adjacent[0]][end_sta]+station_adjacent_distance[0], minsta = station_adjacent[0];
		for (int i = 0; i < change_num * 2; i++)
			if (!station_adjacent[i])
				continue;
			else if (D[station_adjacent[i]][end_sta]+station_adjacent_distance[i] < min)
			{
				min=D[station_adjacent[i]][end_sta]+station_adjacent_distance[i];
				minsta=station_adjacent[i];
			}
		return minsta;
	}
}




//2-3

void scan(int cut)
{
	int num_via = 0;//从0计数 每次加上numintrip的值 via[num_via]即为数据
	int trip_max=cut;
	if(DEBUGSCAN)
		trip_max=DEBUGSCAN;
	for (int num_t = 0; num_t < trip_max;num_via+=numintrip[num_t],num_t++)//第0遍 头->尾 是否存在34号线信息
	{
		if(DEBUGSTOP&&num_t==DEBUGSTOP)
			getchar();
		int i;
		bool ok=1;

		for (i = num_via; i < num_via + numintrip[num_t]; i++)//第0遍 头->尾
		{
			if (via[i].pos)
			{
				for(int j=0;j<sta[via[i].station].change_num;j++)
					if(sta[via[i].station].line_num[j]==LINE1||sta[via[i].station].line_num[j]==LINE2)
						ok=0;
			}
			if(i==num_via + numintrip[num_t]-1)
				fun_del(num_t,STATUS_NO34);
		}
		if(ok)
			continue;
		if(DELNOOD)
		{
			for (i = num_via; i < num_via + numintrip[num_t]; i++)//第一遍 头->尾
			{
				if (via[i].pos)
				{
					if (DEBUGSCAN)
						cout << "tripnum:" << num_t + 1 << " scan1:" << i + 1 << " pass!" << endl;
					break;
				}
				if (via[i].line == LINE1 || via[i].line == LINE2)
				{
					fun_del(num_t);//34 no OD
					ok = 1;
					if (DEBUGSCAN)
						cout << "tripnum:" << num_t + 1 << " scan1:" << i + 1 << " 34noOD!" << endl;
					break;
				}
			}
			if (ok)
				continue;

			for (int i = num_via + numintrip[num_t] - 1; i >= num_via; i--)//第二遍 尾->头
			{
				if (via[i].pos)
				{
					if (DEBUGSCAN)
						cout << "tripnum:" << num_t + 1 << " scan2:" << i + 1 << " pass!" << endl;
					break;
				}
				if (via[i].line == LINE1 || via[i].line == LINE2)
				{
					fun_del(num_t);//34 no OD
					ok = 1;
					if (DEBUGSCAN)
						cout << "tripnum:" << num_t + 1 << " scan2:" << i + 1 << " 34noOD!" << endl;
					break;
				}
			}
			if (ok)
				continue;
		}
		int head=0,tail=0;
		int head_num,tail_num;
		int temphead=0;
		for(int i = num_via; i < num_via + numintrip[num_t]; i++)//第三遍 头->尾
		{
			int nofind=0;
			if(via[i].pos)
			{
				for (int ii = 0; ii < sta[via[i].station].change_num; ii++)
					if ((sta[via[i].station].line_num[ii] == LINE1 ||
					    sta[via[i].station].line_num[ii] == LINE2) && !temphead)
					{
						head = via[i].station;//第一站是34(简化模型 不考虑在34站经过没走34站的情况)
						head_num=i;
						if (DEBUGSCAN)
							cout << "head sta is" << sta[via[i].station].name << " on 34" << endl;
						break;
					}
					else if (sta[via[i].station].line_num[ii] == LINE1 ||
					         sta[via[i].station].line_num[ii] == LINE2)
					{
						if (sta[via[i].station].if_trans(temphead))
						{
							head = via[i].station;
							head_num=i;
							break;
						}
						else
						{
							nofind =1;
							break;
						}
					}
				if(nofind)
					break;

				if (!head)
				{
//					if(DEBUGSCAN)
//						cout<<"head sta not on 34!"<<endl;
//					break;
					temphead = via[i].station;
				}//第一站不是34
			}

//			if(via[i].line==LINE2||via[i].line==LINE1) //pos一定为1 即找到了3/4换乘站
//			{
//				head=via[i].station;
//				if(DEBUGSCAN)
//					cout<<"tripnum:" << num_t+1<<" scan3:"<<i+1<<" find 3/4!"<<endl;
//			}
//			else
//			{
//				//if(via[i].pos&&!head)
//				//	head=via[i].station;
//				int flag_find=0;
//				for(int j=0;j<sta[via[i].station].change_num;j++)
//				{
//					if(sta[via[i].station].line_num[j]==LINE1||sta[via[i].station].line_num[j]==LINE2)//找到34换乘站
//					{
//						flag_find=sta[via[i].station].line_num[j];//若找到34换乘站信息 检查是否在此换乘
//						for(int k=i;k<num_via + numintrip[num_t];k++)
//						{
//							if(!via[k].pos&&via[k].line==flag_find)//下一次捕获到在34列车 换乘
//							{
//								flag_find=1;
//								if(DEBUGSCAN)
//									cout<<"tripnum:" << num_t+1<<" scan3:"<<i+1<<"j: "
//								    << j<<"next catch in 34 subway ";
//								break;
//							}
//							if(via[k].pos)//捕获在站台
//							{
//								int flag_find_sub = 0;
//								for (int jj = 0; jj < sta[via[k].station].change_num; jj++)//检测是否为34的站台
//									if (if_34_station(k, i) == 1)//为34站台 且不在同一站
//									{
//										flag_find_sub = sta[via[k].station].line_num[jj];
//										if(DEBUGSCAN)
//											cout<<"tripnum:" << num_t+1<<" scan3:"<<i+1<<"j: "
//										    << j<<" jj: "<<jj<<" not same station"<<endl;
//									}
//									else if (!if_34_station(k, i))//同一站
//									{
//										if(DEBUGSCAN)
//											cout<<"tripnum:" << num_t+1<<" scan3:"<<i+1<<"j: "
//										    << j<<" jj: "<<jj<<" same station"<<endl;
//										break;
//									}
//									else//不在34的站台
//									{
//										flag_find_sub = -1;
//										if(DEBUGSCAN)
//											cout<<"tripnum:" << num_t+1<<" scan3:"<<i+1<<"j: "
//										    << j<<" jj: "<<jj<<"not 34 station"<<endl;
//										break;
//									}
//								if (flag_find_sub == -1)
//								{
//									flag_find = 0;
//									break;
//								}
//
//							}
//						}
//					}
//					if(flag_find)//找到换乘站
//					{
//						head=via[i].station;
//						if(DEBUGSCAN)
//							cout<<"tripnum:" << num_t+1<<" scan3:"<<i+1<<" flag_find=1"<<endl;
//						break;
//					}
//				}
			if(head)
			{
				if(DEBUGSCAN)
				{
					cout<<"tripnum:" << num_t+1<<" head:"<<sta[head].name<<endl;
				}
				break;
			}
		}

		int temptail=0;
		for(int i = num_via + numintrip[num_t]-1; i >= num_via; i--)//第四遍 尾->头
		{

			if(via[i].pos)
			{
				for (int ii = 0; ii < sta[via[i].station].change_num; ii++)
					if ((sta[via[i].station].line_num[ii] == LINE1 ||
					    sta[via[i].station].line_num[ii] == LINE2) && !temptail)
					{
						if (DEBUGSCAN)
							cout << "tail sta is" << sta[via[i].station].name << " on 34" << endl;
						tail = via[i].station;//第一站是34(简化模型 不考虑在34站经过没走34站的情况)
						tail_num=i;
						break;
					}
					else if (sta[via[i].station].line_num[ii] == LINE1 ||
					         sta[via[i].station].line_num[ii] == LINE2)
					{
						if(sta[via[i].station].if_trans(temptail))
						{
							tail=via[i].station;
							tail_num=i;
							break;
						}
						else
						{
							temptail=via[i].station;
							break;
						}
					}
				if (!tail)
				{
					temptail = via[i].station;
//					if(DEBUGSCAN)
//						cout<<"tail sta not on 34!"<<endl;
//					break;//第一站不是34
				}
			}

//			if(via[i].line==LINE2||via[i].line==LINE1) //pos一定为1 即找到了3/4换乘站
//			{
//				tail=via[i].station;
//				if(DEBUGSCAN)
//					cout<<"tripnum:" << num_t+1<<" scan4:"<<i+1<<" find 3/4!"<<endl;
//			}
//			else
//			{
//				//if(via[i].pos&&!head)
//				//	head=via[i].station;
//				int flag_find=0;
//				for(int j=0;j<sta[via[i].station].change_num;j++)
//				{
//					if(sta[via[i].station].line_num[j]==LINE1||sta[via[i].station].line_num[j]==LINE2)//找到34换乘站
//					{
//						flag_find=sta[via[i].station].line_num[j];//若找到34换乘站信息 检查是否在此换乘
//						for(int k=i;k>=num_via;k--)
//						{
//							if(!via[k].pos&&via[k].line==flag_find)//下一次捕获到在34列车 换乘
//							{
//								flag_find=1;
//								if(DEBUGSCAN)
//									cout<<"tripnum:" << num_t+1<<" scan4:"<<i+1<<"j: "
//								    << j<<"next catch in 34 subway ";
//								break;
//							}
//							if(via[k].pos)//捕获在站台
//							{
//								int flag_find_sub = 0;
//								for (int jj = 0; jj < sta[via[k].station].change_num; jj++)//检测是否为34的站台
//									if (if_34_station(k, i) == 1)//为34站台 且不在同一站
//									{
//										if(DEBUGSCAN)
//											cout<<"tripnum:" << num_t+1<<" scan4:"<<i+1<<"j: "
//										    << j<<" jj: "<<jj<<" not same station"<<endl;
//										flag_find_sub = sta[via[k].station].line_num[jj];
//									}
//									else if (!if_34_station(k, i))//同一站
//									{
//										if(DEBUGSCAN)
//											cout<<"tripnum:" << num_t+1<<" scan4:"<<i+1<<"j: "
//										    << j<<" jj: "<<jj<<" same station"<<endl;
//										break;
//									}
//									else//不在34的站台
//									{
//										flag_find_sub = -1;
//										if(DEBUGSCAN)
//											cout<<"tripnum:" << num_t+1<<" scan4:"<<i+1<<"j: "
//										    << j<<" jj: "<<jj<<"not 34 station"<<endl;
//										break;
//									}
//								if (flag_find_sub == -1)
//								{
//									flag_find = 0;
//									break;
//								}
//
//							}
//						}
//					}
//					if(flag_find)//找到换乘站
//					{
//						tail=via[i].station;
//						if(DEBUGSCAN)
//							cout<<"tripnum:" << num_t+1<<" scan4:"<<i+1<<" flag_find=1"<<endl;
//						break;
//					}
//				}
//			}
			if(tail)
			{
				if(DEBUGSCAN)
					cout<<"tripnum:" << num_t+1<<" tail:"<<sta[tail].name<<endl;
				break;
			}
		}

		if(head&&tail)
		{
			if(DEBUGSCAN)
				cout<<"tripnum:" << num_t+1<<" complete "<<endl;
			if(fun_complete(num_t,head,tail))
				trip_adjust(num_t,num_via,tail,head_num,tail_num);
			if(DELERROR&&status_trip[num_t]!=4&&del_error_trip(num_t,num_via))
				continue;
			if(right_trip(num_t,num_via)&&status_trip[num_t]!=4)
				status_trip[num_t]=STATUS_FULL;
		}
		else
		{
			int s34h=0,s34t=0,sn34h=0,sn34t=0;//34/not34 头站尾站所在via指针
			int t34h=0,t34t=0,tn34h=0,tn34t=0;
			//34/not34 最后一次捕捉到在站时间 最后一次捕捉到在线上时间的via指针
			if(!head)
			{
				int temp_sta1=0,temp_t1=0,i;
				for (i = num_via; i < num_via + numintrip[num_t]; i++)//第五遍 头->尾
				{
					if(via[i].pos&&!if_34_station(i))//非34站
						temp_sta1=i;
					else if(!via[i].pos&&!(via[i].line==LINE1||via[i].line==LINE2))//非34列车
						temp_t1=i;
					else if(!via[i].pos&&(via[i].line==LINE1||via[i].line==LINE2)&&!t34h)//34列车
						t34h=i;
					else if(via[i].pos&&if_34_station(i)&&!s34h)//34站
						s34h=i;
					if(temp_sta1&&s34h)//找到两个站 退出
						break;
				}
				if(i>=num_via + numintrip[num_t])
				{
					fun_del(num_t,3);
					continue;
				}///error

				tn34h=temp_t1;
				sn34h=temp_sta1;

				if(!t34h)
					t34h=s34h;
				if(!tn34h)
					tn34h=sn34h;
				if(s34h<sn34h)
				{
					fun_del(num_t,3);
					continue;
				}
				if(DEBUGSCAN)
					cout<<"find trans head"<<endl<<num_t<<" "<<s34h<<" "<<sn34h<<" "<<t34h<<" "<<tn34h<<endl;

				head=fun_find_trans(num_t,s34h,sn34h,t34h,tn34h);
				head_num=s34h;
				if(head==-1)
				{
					fun_del(num_t,3);
					continue;
				}
				if(DEBUGSCAN)
					cout<<"find head is "<<sta[head].number<<endl;
			}

			if(!tail)
			{
				int temp_sta1=0,temp_t1=0,i;
				for (int i = num_via + numintrip[num_t]-1; i >= num_via; i--)//第六遍 头->尾
				{
					if(via[i].pos&&!if_34_station(i))
						temp_sta1=i;
					else if(!via[i].pos&&!(via[i].line==LINE1||via[i].line==LINE2))
						temp_t1=i;
					else if(!via[i].pos&&(via[i].line==LINE1||via[i].line==LINE2)&&!t34t)
						t34t=i;
					else if(via[i].pos&&if_34_station(i)&&!s34t)
						s34t=i;
					if(temp_sta1&&s34t)
						break;
				}
				if(i<num_via)
				{
					fun_del(num_t,3);
					continue;
				}///error

				tn34t=temp_t1;
				sn34t=temp_sta1;

				if(!t34t)
					t34t=s34t;
				if(!tn34t)
					tn34t=sn34t;
				if(s34t>sn34t)
				{
					fun_del(num_t,3);
					continue;
				}
				if(DEBUGSCAN)
					cout<<"find trans tail"<<endl<<num_t<<" "<<s34t<<" "<<sn34t<<" "<<t34t<<" "<<tn34t<<endl;

				tail=fun_find_trans(num_t,s34t,sn34t,t34t,tn34t);
				tail_num=t34h;
				if(tail==-1)
				{
					fun_del(num_t,3);
					continue;
				}

				if(DEBUGSCAN)
					cout<<"find tail is "<<sta[tail].name<<endl;

			}
			if(fun_complete(num_t,head,tail,STATUS_FIND_TRANS))
				trip_adjust(num_t,num_via,tail,head_num,tail_num);
			if(DELERROR&&status_trip[num_t]!=4&&del_error_trip(num_t,num_via))
				continue;
			if(right_trip(num_t,num_via)&&status_trip[num_t]!=4)
				status_trip[num_t]=STATUS_FULL;
		}
	}//end of for
	if(DEBUGSCAN)
		test_print_scan(DEBUGSCAN);
	else
		test_print_scan(trip_max);
}

void scan_n(int cut)
{
	int num_via = 0;//从0计数 每次加上numintrip的值 via[num_via]即为数据
	int trip_max = cut;
	if (DEBUGSCAN)
		trip_max = DEBUGSCAN;
	for (int num_t = 0; num_t < trip_max; num_via += numintrip[num_t], num_t++)
	{
		if (DEBUGSTOP && num_t == DEBUGSTOP)
			getchar();
		int i;
		int num=0;
		int j;
		for(i = num_via; i < num_via + numintrip[num_t];)
		{
			if(via[i].pos)
			{
				for(j=i;j<num_via + numintrip[num_t];j++)
					if(via[j].pos&&via[j].station!=via[i].station)
					{
						num=short_line(num_t,via[i].station,via[j].station,num);
						i=j;
						break;
					}
				if(j==num_via + numintrip[num_t])
					break;
			}
			else
				i++;
		}
		status_trip[num_t]=STATUS_COMPLETE;
		cout<<num_t<<endl;
	}
	if (DEBUGSCAN)
		test_print_scan(DEBUGSCAN);
	else
		test_print_scan(trip_max);
}




int if_34_station(int k,int i)//第k条记录 (检测是否与i条记录在同一站)
{
//	for (int j = 0; j < sta[via[k].station].change_num; j++)
//		if ((sta[via[k].station].line_num[j] == LINE1 || sta[via[k].station].line_num[j] == LINE2) &&
//		    (!i || via[k].station != via[i].station))
//			return 1;//不同站返回1
//		else if (!i || via[k].station == via[i].station)//同一站返回0
//			return 0;
//	return -1;//不在34线 返回-1
	for(int j=0;j<sta[via[k].station].change_num;j++)
		if(sta[via[k].station].line_num[j]==LINE1||sta[via[k].station].line_num[j]==LINE2)
			return 1;
	return 0;
}

int fun_complete(int trip, int head, int tail, int status)//完整
{
	if(head==tail)
	{
		status_trip[trip]=status;
		trip_path[trip][0]=head;
		return status;
	}
	status_trip[trip]=status;
	int temp=head,i=1;
	trip_path[trip][0]=head;
	while(1)
	{
		temp=sta[temp].find_next_sta(tail);
		if(temp==tail)
			break;
		trip_path[trip][i]=temp;
		i++;
		if(i>VIA_TRIP_MAX)
			break;
	}
	if(temp!=tail)
	{
		fun_del(trip,STATUS_ERROR);
		return -1;
	}
	trip_path[trip][i]=tail;
	return status;
}



void fun_del(int trip, int status) //各种错误返回
{
	status_trip[trip]=status;
//	for(int i=0;i<VIA_TRIP_MAX&&trip_path[trip][i]!=-1;i++)
//		trip_path[trip][i]=-1;
//	if(DEBUGERROR&&status)
//		cout<<trip<<"  error:"<<status<<endl;
	if(DEBUGNOOD&&!status)
		cout<<trip<<"  no OD"<<endl;
}



int fun_find_trans(int trip, int s34, int sn34, int t34, int tn34)//找换乘
{
	int time1 = abs(via[sn34].time - via[tn34].time);
	int time2 = abs(via[s34].time - via[t34].time);
	int sta34 = via[s34].station;
	int stan34 = via[sn34].station;
	if (time2)
		while (1)
		{

			bool find = 0;
			int next = sta[sta34].find_next_sta(via[sn34].station);
//			if (next == via[sn34].station)
//			{
//				sta34 = via[sn34].station;
//				break;
//			}
			for (int i = 0; i < 2 * sta[sta34].change_num && !find; i++)
				if (next == sta[sta34].station_adjacent[i] && sta[sta34].station_adjacent_distance[i] < time2)
				{
					time2 -= sta[sta34].station_adjacent_distance[i];
					sta34 = next;
					break;
				}
				else if (next == sta[sta34].station_adjacent[i])
					find = 1;
			if (find)
				break;
		}
	if (time1)
		while (1)
		{

			bool find = 0;
			int next = sta[stan34].find_next_sta(via[s34].station);
//			if (next == via[s34].station)
//			{
//				stan34 = via[s34].station;
//				break;
//			}
			for (int i = 0; i < 2 * sta[stan34].change_num && !find; i++)
				if (next == sta[stan34].station_adjacent[i] && sta[stan34].station_adjacent_distance[i] < time1)
				{
					time1 -= sta[stan34].station_adjacent_distance[i];
					stan34 = next;
					break;
				}
				else if (next == sta[stan34].station_adjacent[i])
					find = 1;
			if (find)
				break;
		}
	/*if(s34_tem==via[sn34].station||sn34_tem==via[s34].station)
	{
		fun_del(trip, STATUS_ERROR);
		return -1;
	}*/

//	int linen34=0;
//	for(int i=0;i<sta[via[sn34_tem].station].change_num&&!linen34;i++)
//		if(sta[via[sn34_tem].station].line_num[i]!=LINE1&&sta[via[sn34_tem].station].line_num[i]!=LINE2)
//			linen34=sta[via[sn34_tem].station].line_num[i];
//	if(!linen34)
//	{
//		fun_del(trip, STATUS_ERROR);
//		return -1;
//	}

	int sta_find = sta34;
	while (1)
	{
		bool find34 = 0, findn34 = 0;
		for (int i = 0; i < sta[sta_find].change_num; i++)
		{
			if (sta[sta_find].line_num[i] == LINE1 || sta[sta_find].line_num[i] == LINE2)
				find34 = 1;
			if (sta[sta_find].line_num[i] != LINE1 && sta[sta_find].line_num[i] != LINE2)
				findn34 = 1;
		}
		if (find34 && findn34)
			break;
		if (sta_find == stan34)//如果直接到了终点站还没找到换乘站 错误返回
		{
			fun_del(trip, STATUS_ERROR);
			return -1;
		}
		sta_find = sta[sta_find].find_next_sta(stan34);
	}
	return sta_find;
}

bool station_data::if_trans(int station)
{
	for(int i =0 ;i<change_num;i++)
		for(int j=0;j<sta[station].change_num;j++)
			if(line_num[i]==sta[station].line_num[j])
				return 1;
	return 0;
}

int del_error_trip(int trip, int start_num)
{
	for(int i=start_num;i<start_num+numintrip[trip];i++)
	{
		int find=0;
		if(!via[i].pos||!if_34_station(i,0))
			continue;
		for(int j=0;j<VIA_TRIP_MAX&&trip_path[trip][j]!=-1;j++)
		{
			if(via[i].station==trip_path[trip][j])
			{
				find=1;
				break;
			}
		}
		if(!find)
		{
			status_trip[trip] = STATUS_ERROR;
			cout<<"error:"<<trip<<endl;
			return 1;
		}
	}
	return 0;
}

int right_trip(int trip, int start_num)
{
	for(int j=0;j<VIA_TRIP_MAX&&trip_path[trip][j]!=-1;j++)
	{
		int find=0;
		for(int i=start_num;i<start_num+numintrip[trip];i++)
		{
			if(!via[i].pos||!if_34_station(i,0))
				continue;
			if(via[i].station==trip_path[trip][j])
			{
				find=1;
				break;
			}
		}
		if(!find)
			return 0;
	}
	return 1;
}

int trip_adjust(int trip ,int start_num ,int tail ,int head_num ,int tail_num)
{
	int j=0;
	for(int i=head_num;i<tail_num;i++)
	{
		if(via[i].pos&&i&&via[i].station!=via[i-1].station)
		{
			if(via[i].station==tail)
				break;
			if(if_in_path(trip,via[i].station,j))
				j=if_in_path(trip,via[i].station,j)-1;
			else
			{
				cout<<trip<<"adjust"<<endl;
				clear_path(trip,j);
				j=short_line(trip,trip_path[trip][j],via[i].station,j);
				short_line(trip,via[i].station,tail,j);
			}
		}
	}
}

int if_in_path(int trip, int sta_via, int j)
{
	for(int i=1;i<VIA_TRIP_MAX-j+1;i++)
		if(sta_via==trip_path[trip][j+i-1])
			return i;
	return 0;
}

void clear_path(int trip ,int j)
{
	for(j++;j<VIA_TRIP_MAX;j++)
	{
		if(trip_path[trip][j]==-1)
			break;
		trip_path[trip][j]=-1;
	}
}

int short_line(int trip, int head, int tail, int j)
{
	int temp=head,i=j+1;
	trip_path[trip][j]=head;
	if(head==tail)
		return j;
	while(1)
	{
		temp=sta[temp].find_next_sta(tail);
		if(temp==tail)
			break;
		trip_path[trip][i]=temp;
		i++;
		if(i>=VIA_TRIP_MAX)
			break;
	}
	if(temp!=tail)
	{
		fun_del(trip,STATUS_ERROR);
		return -1;
	}
	trip_path[trip][i]=tail;
	return i;
}




