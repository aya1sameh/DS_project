#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type, REGION r_region)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	Region = r_region;	
}
Order::Order(int id, ORD_TYPE r_Type, REGION r_region,int r_Distance,double r_totalMoney,int r_ArrTime)
{
	ID = (id>0&&id<1000)?id:0;	//1<ID<999
	type = r_Type;
	Region = r_region;	
	Distance=r_Distance;
	totalMoney=r_totalMoney;
	ArrTime=r_ArrTime;
	WaitingTime=0;
	switch (r_Type)	
	{
	case TYPE_NRM:
		priority=0;
		break;
	case TYPE_FROZ:
		priority=0;
		break;
	case TYPE_VIP:
		priority=(totalMoney*0.5)+(ArrTime*0.2)+(Distance*0.3);
		break;
	};
}
Order::~Order()
{
}

int Order::GetID()
{
	return ID;
}


int Order::GetType() const
{
	return type;
}
void Order::SetType(ORD_TYPE r_Type ) 
{
	type = r_Type;
	switch (r_Type)	
	{
	case TYPE_NRM:

		priority=0;
		break;
	case TYPE_FROZ:
		priority=0;
		break;
	case TYPE_VIP:
		priority=(totalMoney*0.5)+(ArrTime*0.2)+(Distance*0.3);
		break;
	};
}
void Order::SetArrivalTime(int d)
{
	ArrTime=d;
}
int Order::GetArrivalTime() const
{
	return ArrTime;
}
REGION Order::GetRegion() const
{
	return Region;
}
void Order::SetFinishTime(int d)
{
	FinishTime=d;
}
int Order::GetFinishTime() const
{
	return FinishTime;
}
void Order::SetWaitingTime(int d)
{
	WaitingTime=d;

}
int Order::GetWaitingTime() const
{
	return WaitingTime;
}
void Order::SetServTime(int d)
{
	ServTime=d;

}
int Order::GetServTime() const
{
	return ServTime;
}
void Order::SetDistance(int d)
{
	Distance = d>0?d:0;
}
int Order::GetDistance() const
{
	return Distance;
}
int Order::GetPriority() const
{
	return priority;
}
void Order::SetPriority(int d)
{
	priority=d;
}