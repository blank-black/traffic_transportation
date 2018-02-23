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
	for(int i=0;i<trip_max;i++)
	{
		out<<"trip id: "<<i+1<<endl;
		out<<"trip status: ";
		if(status_trip[i]==0)
			out<<"缺少OD"<<endl;
		if(status_trip[i]==1)
			out<<"正常补全"<<endl;
		if(status_trip[i]==2)
			out<<"用缩短OD方法找换乘并补全"<<endl;
		if(status_trip[i]==3)
			out<<"错误"<<endl;
		if(status_trip[i]==4)
			out<<"OD相同"<<endl;
		if(status_trip[i]==5)
			out<<"未出现与34有关信息"<<endl;
		if(status_trip[i]==1||status_trip[i]==2)
		{
			out<<"trip path: ";
			for(int j=0;j<VIA_TRIP_MAX;j++)
				if(trip_path[i][j]!=-1)
					out<<sta[trip_path[i][j]].name<<" ";
			out<<endl;
		}
		out<<endl;
	}
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