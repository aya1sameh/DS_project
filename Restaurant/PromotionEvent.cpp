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
	inputFile>>Timestep>>ID>>ExMoney;

}
void PromotionEvent::Execute(Restaurant *pRest)
{

}
PromotionEvent::~PromotionEvent()
{

}
