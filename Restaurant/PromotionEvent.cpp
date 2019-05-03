#include "PromotionEvent.h"
#include "Rest\Restaurant.h"
PromotionEvent::PromotionEvent(int Ts,int id,double exmon):Event(Ts,id)
{
	ExMoney=exmon;
}
PromotionEvent::PromotionEvent(ifstream &inputFile):Event(0,0)
{
	Load(inputFile);
}
void PromotionEvent::Load(ifstream &inputFile)
{
	inputFile>>EventTime>>OrderID>>ExMoney;

}
void PromotionEvent::Execute(Restaurant *pRest)
{
	int position;
	Order*pOrd = pRest->GetNormalOederWithId(OrderID, position);
	cout<<"\n\n\nOrder was promoted";
	if (pOrd)
	{
		switch (pOrd->GetRegion())
		{
		case A_REG:
			cout<<"\n\n\nOrder was promoted";
			pRest->RemoveFromNORMALOrdersList(position, 0);
			pOrd->SetType(TYPE_VIP);
			pRest->AddtoVipOrders(pOrd, 0);
				
			break;
		case B_REG:
			cout<<"\n\n\nOrder was promoted";
			pRest->RemoveFromNORMALOrdersList(position, 1);
			pOrd->SetType(TYPE_VIP);
			pRest->AddtoVipOrders(pOrd, 1);
				
			break;
		case C_REG:	
			cout<<"\n\n\nOrder was promoted";
			pRest->RemoveFromNORMALOrdersList(position, 2);
			pOrd->SetType(TYPE_VIP);
			pRest->AddtoVipOrders(pOrd, 2);
				cout<<"\n\n\nOrder was promoted";
			break;
		case D_REG:
			cout<<"\n\n\nOrder was promoted";
			pRest->RemoveFromNORMALOrdersList(position, 3);
			pOrd->SetType(TYPE_VIP);
			pRest->AddtoVipOrders(pOrd, 3);
			
			break;
		}
	}

}
PromotionEvent::~PromotionEvent()
{

}
