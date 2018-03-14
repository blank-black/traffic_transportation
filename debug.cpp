#include "traffic.h"

void via_data::test_print_via()
{
	cout<<DEVIA<<":"<<endl;
	cout<<"id "<<id<<endl;
	cout<<"trip_id "<<trip_id<<endl;
	cout<<"MAC "<<MAC<<endl;
	cout<<"time "<<time<<endl;
	cout<<"year "<<year<<endl;
	cout<<"month "<<month<<endl;
	cout<<"day "<<day<<endl;
	cout<<"station num "<<station<<endl;
	cout<<"station name "<<sta[station-1].name<<endl;
	cout<<"pos "<<pos<<endl;
	cout<<"line "<<line<<endl;
	cout<<"car_id "<<car_id<<endl;
	cout<<"train_id "<<train_id<<endl;
}

void test_print_scan(int trip_max)
{
	ofstream out;
	out.open(scan_result, ios::out);
	int status_nood=0,status_right=0,status_shortod=0,status_error=0,status_sameod=0,status_no34=0,status_full=0;
	for(int i=0;i<trip_max;i++)
	{
		out<<"trip id: "<<i+1<<endl;
		out<<"trip status: ";
		if(status_trip[i]==0)
		{
			out<<"缺少OD"<<endl;
			status_nood++;
		}
		if(status_trip[i]==1)
		{
			out<<"正常补全"<<endl;
			status_right++;
		}
		if(status_trip[i]==2)
		{
			out<<"用缩短OD方法找换乘并补全"<<endl;
			status_shortod++;
		}
		if(status_trip[i]==3)
		{
			out<<"错误"<<endl;
			status_error++;
		}
		if(status_trip[i]==4)
		{
			out<<"OD相同"<<endl;
			status_sameod++;
		}
		if(status_trip[i]==5)
		{
			out<<"未出现与34有关信息"<<endl;
			status_no34++;
		}
		if(status_trip[i]==6)
		{
			out<<"数据完整"<<endl;
			status_full++;
		}
		if(status_trip[i]==1||status_trip[i]==2||status_trip[i]==6)
		{
			out<<"trip path: ";
			for(int j=0;j<VIA_TRIP_MAX;j++)
				if(trip_path[i][j]!=-1)
					out<<sta[trip_path[i][j]].name<<" ";
			out<<endl;
		}
		out<<endl;
	}
	if(DELNOOD)
		cout<<"缺少OD:"<<status_nood<<endl;
	cout<<"正常补全:"<<status_right<<endl;
	cout<<"用缩短OD方法找换乘并补全:"<<status_shortod<<endl;
	cout<<"错误:"<<status_error<<endl;
	cout<<"OD相同:"<<status_sameod<<endl;
	cout<<"未出现与34有关信息:"<<status_no34<<endl;
	cout<<"数据完整:"<<status_full<<endl;
	cout<<"总计:"<<status_nood+status_right+status_shortod+status_error+status_sameod+status_no34+status_full<<endl;
//	out.close();
}




int test_find_trans(int num)
{
	int head=0,tail=0,num_t=num,num_via=0;
	for(int i=0;i<num;i++)
		num_via+=numintrip[i];
	int s34h=0,s34t=0,sn34h=0,sn34t=0;//34/not34 头站尾站所在via指针
	int t34h=0,t34t=0,tn34h=0,tn34t=0;
	//34/not34 最后一次捕捉到在站时间 最后一次捕捉到在线上时间的via指针
	if(!head)
	{
		int temp_sta1, temp_t1, i;
		for (i = num_via; i < num_via + numintrip[num_t]; i++)//第五遍 头->尾
		{
			if (via[i].pos && !if_34_station(i))
				temp_sta1 = i;
			else if (!via[i].pos && !(via[i].line == LINE1 || via[i].line == LINE2))
				temp_t1 = i;
			else if (!via[i].pos && (via[i].line == LINE1 || via[i].line == LINE2) && !t34h)
				t34h = i;
			else if (via[i].pos && if_34_station(i) && !s34h)
				s34h = i;
			if (temp_sta1 && temp_t1 && t34h && s34h)
				break;
		}
//		if(i>=num_via + numintrip[num_t])
//		{
//			fun_del(num_t);
//			continue;
//		}///error

		tn34h = temp_t1;
		sn34h = temp_sta1;

		head = fun_find_trans(num_t, s34h, sn34h, t34h, tn34h);

	}

	if(0)
	{
		int temp_sta1,temp_t1,i;
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
			if(temp_sta1&&temp_t1&&t34t&&s34t)
				break;
		}
//		if(i<num_via)
//		{
//			fun_del(num_t);
//			continue;
//		}///error

		tn34t=temp_t1;
		sn34t=temp_sta1;

		tail=fun_find_trans(num_t,s34t,sn34t,t34t,tn34t);
	}

	cout<<head<<" "<<tail<<endl;
}


void interaction()
{
	cout<<"Paths have completed!"<<endl;
	while(1)
	{
		cout << "Press 1 to calculate section traffic" << endl;
		cout << "Press 2 to calculate transfer amount " << endl;
		cout << "Press 3 to calculate path split ratio" << endl;
		cout << "Press 4 to quit" << endl;
		int a;
		cin>>a;
		if (a == 1)
		{
			char a_char[STATION_NAME_MAX],b_char[STATION_NAME_MAX];
			int a_int,b_int;
			cout<<"Enter two adjacent stations name in number: (like 66 15)"<<endl;
			cin>>a_int>>b_int;
//			cout<<a_char<<b_char;
			int j,section_traffic=0;
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(a_char, sta[j].name))
//					a_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(b_char, sta[j].name))
//					b_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
			for(int i=0;i<TRIP_MAX;i++)
				for(int k=0;k<VIA_TRIP_MAX;k++)
				{
					if(trip_path[i][k]==-1)
						break;
					if(trip_path[i][k]==a_int&&trip_path[i][k+1]==b_int)
						section_traffic++;
				}
			cout<<"section traffic = "<<section_traffic<<endl;
		}
		else if (a == 2)
		{
			char a_char[STATION_NAME_MAX],b_char[STATION_NAME_MAX],c_char[STATION_NAME_MAX];
			int a_int,b_int,c_int;
			cout<<"Enter three stations name in number: (like 62 35 36)"<<endl;
			cin>>a_int>>b_int>>c_int;
			int j,transfer_amount=0;
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(a_char, sta[j].name))
//					a_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(b_char, sta[j].name))
//					b_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(c_char, sta[j].name))
//					c_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
			for(int i=0;i<TRIP_MAX;i++)
				for(int k=0;k<VIA_TRIP_MAX;k++)
				{
					if(trip_path[i][k]==-1)
						break;
					if(trip_path[i][k]==a_int&&trip_path[i][k+1]==b_int&&trip_path[i][k+2]==c_int)
						transfer_amount++;
				}
			cout<<"transfer amount = "<<transfer_amount<<endl;

		}
		else if (a == 3)
		{
			char a_char[STATION_NAME_MAX],b_char[STATION_NAME_MAX];
			int a_int,b_int;
			cout<<"Enter two stations name in number: (like 4 15)"<<endl;
			cin>>a_int>>b_int;
			int j=0;
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(a_char, sta[j].name))
//					a_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
//			for (j = 1; j < STATION_NUM; j++)//转换站名为站号
//				if (!strcmp(b_char, sta[j].name))
//					b_int = sta[j].number;
//			if(j==STATION_NUM)
//			{
//				cout<<"Wrong name!"<<endl;
//				continue;
//			}
			int split[SPLITMAX][VIA_TRIP_MAX];
			int split_num[SPLITMAX]={0};
			int s_num[SPLITMAX][SPLITMAX];
			for(int i=0;i<SPLITMAX;i++)
				for(int k=0;k<VIA_TRIP_MAX;k++)
					split[i][k]=0;

			int ok=0;
			for(int i=0;i<TRIP_MAX;i++)
				for(int k=0;k<VIA_TRIP_MAX;k++)
				{
					if(trip_path[i][k]==-1)
						break;
					if(trip_path[i][k]==a_int)
					{
						for(int kk=k;kk<VIA_TRIP_MAX;kk++)
						{
							if(trip_path[i][kk]==-1)
								break;
							if(trip_path[i][kk]==b_int)
							{
								split_fun(i,split,split_num,k,kk,s_num);
								ok=1;
								break;
							}
						}
						if(ok)
							break;
					}
				}
			int total=0;
			int i;
			for(i=0;i<SPLITMAX;i++)
			{
				if(!split_num[i])
					break;
				total+=split_num[i];
			}
			cout<<"Total amount:"<<total<<endl;
			for(int k=0;k<i;k++)
			{
				cout<<"The "<<k+1<<" path is :"<<endl;
				for(int kk=0;kk<VIA_TRIP_MAX;kk++)
				{
					if(!split[k][kk])
						break;
					cout<<sta[split[k][kk]].name<<" ";
				}
				cout<<endl<<"amount:"<<split_num[k]<<"  ratio:"<<(split_num[k]*100.0/total)<<"%"<<endl<<endl;
//				for(int kk=0;kk<split_num[k];kk++)
//					cout<<s_num[k][kk]<<" "<<endl;
			}

		}
		else if (a == 4)
			break;
		else
			cout << "Only press 1,2,3,4!!!"<<endl;
	}
}

void split_fun(int trip, int split[][VIA_TRIP_MAX], int split_num[], int h_num, int t_num, int s_num[][SPLITMAX])
{
	int i;
	for(i=0;i<SPLITMAX;i++)
	{
		if(!split_num[i])
			break;
		int ok=0;
		for(int j=0;j<VIA_TRIP_MAX;j++)
		{
			if(!split[i][j])
			{
				ok=1;
				break;
			}
			if(trip_path[trip][h_num+j]!=split[i][j])
				break;
		}
		if(ok)
		{
			split_num[i]++;
			s_num[i][split_num[i]-1]=trip;
			return void();
		}
	}
	for(int j=0;j<VIA_TRIP_MAX;j++)
	{
		if(trip_path[trip][h_num+j]==trip_path[trip][t_num])
		{
			split[i][j]=trip_path[trip][h_num+j];
			split_num[i]++;
			s_num[i][0]=trip;
			break;
		}
		split[i][j]=trip_path[trip][h_num+j];
	}
}
