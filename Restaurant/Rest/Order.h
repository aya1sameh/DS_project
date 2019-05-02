#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"

class Order
{

protected:
	int ID;         //Each order has a unique ID (from 1 --> 999 )
	ORD_TYPE type;		//order type: Normal, Frozen, VIP
	REGION Region;  //Region of this order
	int Distance;	//The distance (in meters) between the order location and the resturant 
	                
	double totalMoney;	//Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish time
	int priority;
	int WaitingTime;
	

public:
	Order(int ID, ORD_TYPE r_Type, REGION r_region);
	Order(int ID, ORD_TYPE r_Type, REGION r_region,int r_Distance,double r_totalMoney,int r_ArrTime);
	virtual ~Order();

	int GetID();

	int GetType() const;
	REGION GetRegion() const;

	void SetDistance(int d);
	int GetDistance() const;

	void SetArrivalTime(int d);
	int GetArrivalTime() const;

	void SetFinishTime(int d);
	int GetFinishTime() const;

	void SetWaitingTime(int d);
	int GetWaitingTime() const;

	void SetServTime(int d);
	int GetServTime() const;

	int GetPriority() const;
};

#endif