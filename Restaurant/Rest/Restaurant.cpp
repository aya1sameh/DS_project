#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace std;

#include "Restaurant.h"
#include "..\Events\ArrivalEvent.h"
#include"..\CancelEvent.h"
#include"..\PromotionEvent.h"
#include"..\Rest\Order.h"
Restaurant::Restaurant() 
{
	pGUI = NULL;
	for (int i = 0; i < 4; i++)
	{
		ServicedVIPOrders[i]=0;
	}

	for (int i = 0; i < 4; i++)
	{
		ServicedFROZENOrders[i]=0;
	}
	for (int i = 0; i < 4; i++)
	{
		ServicedNORMALOrders[i]=0;
	}
}

//////////////////////////////////////////////

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE	mode = pGUI->getGUIMode();
	pGUI->ResetDrawingList();	
	switch (mode)	
	{
	case MODE_INTR:
		Interactive_Mode();
		break;
	case MODE_STEP:
		break;
	case MODE_SLNT:
		break;
	case MODE_DEMO:
		break;

	};

}

//////////////////////////////////////////////

Order* Restaurant::GetNormalOederWithId(int Id,int& Position)
{
	for(int i=0;i<ActiveNORMALOrder[0].getSize();i++)
	{
		Order*ptr=ActiveNORMALOrder[0].getEntry(i+1);
		if(ptr->GetID()==Id)
		{
			Position=i+1;
			return ptr;
		}
	}
	for(int i=0;i<ActiveNORMALOrder[1].getSize();i++)
	{
		Order*ptr=ActiveNORMALOrder[1].getEntry(i+1);
		if(ptr->GetID()==Id)
		{
			Position=i+1;
			return ptr;
		}
	}
	for(int i=0;i<ActiveNORMALOrder[2].getSize();i++)
	{
		Order*ptr=ActiveNORMALOrder[2].getEntry(i+1);
		if(ptr->GetID()==Id)
		{
			Position=i+1;
			return ptr;
		}
	}
	for(int i=0;i<ActiveNORMALOrder[3].getSize();i++)
	{
		Order*ptr=ActiveNORMALOrder[3].getEntry(i+1);
		if(ptr->GetID()==Id)
		{
			Position=i+1;
			return ptr;
		}
	}
	return NULL;
}

//////////////////////////////////////////////

//////////////////////////////////  Event handling functions   /////////////////////////////
void Restaurant::AddEvent(Event* pE)	//adds a new event to the queue of events
{
	EventsQueue.enqueue(pE);
	cout<<"Event was added  "<<pE->getEventTime()<<"  "<<pE->getOrderID();
}

//////////////////////////////////////////////

//Executes ALL events that should take place at current timestep
void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event *pE;
	while( EventsQueue.peekFront(pE) )	//as long as there are more events
	{
		if(pE->getEventTime() > CurrentTimeStep )	//no more events at current time
			return;

		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue
		delete pE;		//deallocate event object from memory
	}

}

//////////////////////////////////////////////

Restaurant::~Restaurant()
{
	delete pGUI;
}

//////////////////////////////////////////////

void Restaurant::AddtoVipOrders(Order* pOrd,int i)
{
	ActiveVIPOrder[i].enqueue(pOrd,pOrd->GetPriority());
}	
Order* Restaurant::getVipOrders(int i)
{
	Order* pOrd;
	ActiveVIPOrder[i].dequeue(pOrd);
	return pOrd;
}

//////////////////////////////////////////////

void Restaurant::AddtoNormalOrders(Order* pOrd,int i)
{
	ActiveNORMALOrder[i].InsertBeg(pOrd);
}	
Order* Restaurant::getNormalOrders(int i)
{
	Order* pOrd;
	ActiveNORMALOrder[i].DeleteLast(pOrd);
	return pOrd;
}

//////////////////////////////////////////////

void Restaurant::AddtoFROZENOrders(Order* pOrd,int i)
{
	ActiveFROZENOrder[i].enqueue(pOrd);
}	

Order* Restaurant::getFROZENOrders(int i)
{
	Order* pOrd;
	ActiveFROZENOrder[i].dequeue(pOrd);
	return pOrd;
}
void Restaurant::RemoveFromNORMALOrdersList(int Position,int i)
{
	ActiveNORMALOrder[i].remove(Position);
}

//////////////////////////////////////////////

void Restaurant::Interactive_Mode()
{
	/////////////////////////////////////////////////////////
	string AssignOrdersToMotor;
	pGUI->PrintMessage("Enter file name ^_^");
	Load();
	int CurrentTimeStep = 1;
	pGUI->PrintMessage("Click on the mouse to deliver the order");
	pGUI->waitForClick();

	while(!EventsQueue.isEmpty()|| !AllOrdersAREserved() || !AllMotorsCameBack())
	{    AssignOrdersToMotor="";
	ExecuteEvents(CurrentTimeStep);
	Finishing(CurrentTimeStep);
	//Repair(CurrentTimeStep);
	AssighnOrdersToMotors(CurrentTimeStep,AssignOrdersToMotor);
	ResetWindowALL();
	pGUI->PrintMessage("Click on the mouse to deliver the next order  "+to_string(CurrentTimeStep));
	printOutput(AssignOrdersToMotor);
	CurrentTimeStep++;
	pGUI->waitForClick();
	}
	pGUI->PrintMessage("generation done, click to END program");
	Output();
	pGUI->waitForClick();

}
void Restaurant::ResetQueueWindow (Queue<Order*>& Rhs )
{  
	Order**arr;
	arr=NULL;
	Rhs.toArray(arr);

	for(int i=0;i<Rhs.getSize();i++)
	{
		pGUI->AddOrderForDrawing(arr[i]);
	}
}
void Restaurant::ResetPQueueWindow(PQueueLinkedList<Order*,int> &Rhs)
{  
	Order**arr;
	arr=NULL;
	Rhs.toArray(arr);
	for(int i=0;i<Rhs.getSize();i++)
	{
		pGUI->AddOrderForDrawing(arr[i]);
	}
}
void Restaurant::ResetListWindow(LinkedList<Order*>& Rhs )
{  
	Order**arr;
	arr=NULL;
	Rhs.toArray(arr);

	for(int i=(Rhs.getSize())-1;i>-1;i--)
	{
		pGUI->AddOrderForDrawing(arr[i]);
	}
}
void Restaurant::ResetWindowALL( )
{
	pGUI->ResetDrawingList();

	if(!ActiveVIPOrder[0].isEmpty())
		ResetPQueueWindow(ActiveVIPOrder[0]);
	if(!ActiveFROZENOrder[0].isEmpty())
		ResetQueueWindow(ActiveFROZENOrder[0]);
	if(ActiveNORMALOrder[0].getSize()!=0)
		ResetListWindow(ActiveNORMALOrder[0]);

	if(!ActiveVIPOrder[1].isEmpty())
		ResetPQueueWindow(ActiveVIPOrder[1]);
	if(!ActiveFROZENOrder[1].isEmpty())
		ResetQueueWindow(ActiveFROZENOrder[1]);
	if(ActiveNORMALOrder[1].getSize()!=0)
		ResetListWindow(ActiveNORMALOrder[1]);

	if(!ActiveVIPOrder[2].isEmpty())
		ResetPQueueWindow(ActiveVIPOrder[2]);
	if(!ActiveFROZENOrder[2].isEmpty())
		ResetQueueWindow(ActiveFROZENOrder[2]);
	if(ActiveNORMALOrder[2].getSize()!=0)
		ResetListWindow(ActiveNORMALOrder[2]);


	if(!ActiveVIPOrder[3].isEmpty())
		ResetPQueueWindow(ActiveVIPOrder[3]);
	if(!ActiveFROZENOrder[3].isEmpty())
		ResetQueueWindow(ActiveFROZENOrder[3]);
	if(ActiveNORMALOrder[3].getSize()!=0)
		ResetListWindow(ActiveNORMALOrder[3]);

	pGUI->UpdateInterface();
}
void Restaurant:: Load( )
{
	string filename;
	ifstream inputFile;
	filename=pGUI->GetString()+".txt";
	inputFile.open(filename);
	if(inputFile.is_open())
	{
	int s1,s2,s3;
	inputFile>>s1>>s2>>s3;

	
	for(int i=0;i<4;i++)
	{
		inputFile>>normal[i]>>frozen[i]>>vip[i];
	}
	///////////////////////////////////////////////
	for(int j=0;j<4;j++)
	{
		for(int i=0;i<normal[j];i++)
		{
			Motorcycle* pM=new Motorcycle(i+1,s1+2*i);
			NormalMotors[j].enqueue(pM,pM->MotorGetSpeed());
		}
	}

	for(int j=0;j<4;j++)
	{
		for(int i=0;i<frozen[j];i++)
		{
			Motorcycle* pM=new Motorcycle(i+1,s2+i);
			FrozMotors[j].enqueue(pM,pM->MotorGetSpeed());
		}
	}
	for(int j=0;j<4;j++)
	{
		for(int i=0;i<vip[j];i++)
		{
			Motorcycle* pM=new Motorcycle(i+1,s3+3*i);
			FastMotors[j].enqueue(pM,pM->MotorGetSpeed());
		}
	}
	int promlimit;
	inputFile>>promlimit;
	int numofevents;
	inputFile>>numofevents;
	cout<<"num of events done"<<endl;
		Event* ptr;
		for(int i=0;i<numofevents;i++)
		{

			char m;
			inputFile>>m;
			if(m=='R')
			{
				ptr= new ArrivalEvent(inputFile);
				AddEvent(ptr);
			}
			else if(m=='X')
			{
				ptr= new CancelEvent(inputFile);
				AddEvent(ptr);
			}
			else if (m=='P')
			{
				ptr= new PromotionEvent(inputFile);
				AddEvent(ptr);
			}
		}
	}

}
void Restaurant::DeleteActiveOrders()
{
	Order*op;
	for(int i=0;i<4;i++)
	{
		if(!ActiveNORMALOrder[i].isEmpty())
		{
			ActiveNORMALOrder[i].remove(ActiveNORMALOrder[i].getSize());
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!ActiveVIPOrder[i].isEmpty())
		{
			ActiveVIPOrder[i].dequeue(op);
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!ActiveFROZENOrder[i].isEmpty())
		{
			ActiveFROZENOrder[i].dequeue(op);
		}
	}
}
void Restaurant::printOutput(string &s)
{  
	pGUI->PrintMessage("RegionA :  ACorders:ViP: "+to_string(ActiveVIPOrder[0].getSize())+"  FROZEN: "+to_string(ActiveFROZENOrder[0].getSize())+"   NORMAL: "+to_string(ActiveNORMALOrder[0].getSize())+"       SERVED:VIP: "+to_string(ServicedVIPOrders[0])+" FROZ: "+to_string(ServicedFROZENOrders[0])+" NORM: "+to_string(ServicedNORMALOrders[0])+"        Motorcycles:ViP: "+to_string(FastMotors[0].getSize())+" FROZEN: "+to_string(FrozMotors[0].getSize())+" NORMAL: "+to_string(NormalMotors[0].getSize()),1);
	pGUI->PrintMessage("RegionB :  ACorders:ViP: "+to_string(ActiveVIPOrder[1].getSize())+"  FROZEN: "+to_string(ActiveFROZENOrder[1].getSize())+"   NORMAL: "+to_string(ActiveNORMALOrder[1].getSize())+"       SERVED:VIP: "+to_string(ServicedVIPOrders[1])+" FROZ: "+to_string(ServicedFROZENOrders[1])+" NORM: "+to_string(ServicedNORMALOrders[1])+"        Motorcycles:ViP: "+to_string(FastMotors[1].getSize())+" FROZEN: "+to_string(FrozMotors[1].getSize())+" NORMAL: "+to_string(NormalMotors[1].getSize()),2);
	pGUI->PrintMessage("RegionC :  ACorders:ViP: "+to_string(ActiveVIPOrder[2].getSize())+"  FROZEN: "+to_string(ActiveFROZENOrder[2].getSize())+"   NORMAL: "+to_string(ActiveNORMALOrder[2].getSize())+"       SERVED:VIP: "+to_string(ServicedVIPOrders[2])+" FROZ: "+to_string(ServicedFROZENOrders[2])+" NORM: "+to_string(ServicedNORMALOrders[2])+"        Motorcycles:ViP: "+to_string(FastMotors[2].getSize())+" FROZEN: "+to_string(FrozMotors[2].getSize())+" NORMAL: "+to_string(NormalMotors[2].getSize()),3);
	pGUI->PrintMessage("RegionD :  ACorders:ViP: "+to_string(ActiveVIPOrder[3].getSize())+"  FROZEN: "+to_string(ActiveFROZENOrder[3].getSize())+"   NORMAL: "+to_string(ActiveNORMALOrder[3].getSize())+"       SERVED:VIP: "+to_string(ServicedVIPOrders[3])+" FROZ: "+to_string(ServicedFROZENOrders[3])+" NORM: "+to_string(ServicedNORMALOrders[3])+"        Motorcycles:ViP: "+to_string(FastMotors[3].getSize())+" FROZEN: "+to_string(FrozMotors[3].getSize())+" NORMAL: "+to_string(NormalMotors[3].getSize()),4);
	pGUI->PrintMessage(s,5);
}

void Restaurant::AssighnOrdersToMotors(int TimeStep,string&s)
{
	for(int i=0;i<4;i++)
	{
		if(!ActiveVIPOrder[i].isEmpty())
		{
			AssighnOrder_PQueue(TimeStep,ActiveVIPOrder[i],AssighnedVIPOrder[i],FastMotors[i],InserviseFastMotors[i],s);
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!ActiveVIPOrder[i].isEmpty())
		{
			AssighnOrder_PQueue(TimeStep,ActiveVIPOrder[i],AssighnedVIPOrder[i],NormalMotors[i],InserviseNormalMotors[i],s);
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!ActiveVIPOrder[i].isEmpty())
		{
			AssighnOrder_PQueue(TimeStep,ActiveVIPOrder[i],AssighnedVIPOrder[i],FrozMotors[i],InserviseFrozMotors[i],s);
		}
	}

	for(int i=0;i<4;i++)
	{
		if(!ActiveFROZENOrder[i].isEmpty())
		{
			AssighnOrder_Queue(TimeStep,ActiveFROZENOrder[i],AssighnedFROZENOrder[i],FrozMotors[i],InserviseFrozMotors[i],s);
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!ActiveNORMALOrder[i].isEmpty())
		{
			AssighnOrder_List(TimeStep,ActiveNORMALOrder[i],AssighnedNORMALOrder[i],NormalMotors[i],InserviseNormalMotors[i],s);
		}
	}
	for(int i=0;i<4;i++)
	{
		if(!ActiveNORMALOrder[i].isEmpty())
		{
			AssighnOrder_List(TimeStep,ActiveNORMALOrder[i],AssighnedNORMALOrder[i],FastMotors[i],InserviseFastMotors[i],s);
		}
	}
}
void Restaurant::AssighnOrder_PQueue(int TimeStep,PQueueLinkedList<Order*,int>& WaitingOrders,PQueueLinkedList<Order*,int>& InServiceOrders,PQueueLinkedList<Motorcycle*,int>& AvailableMotors,PQueueLinkedList<Motorcycle*,int>& InServiceeMotors,string&s)
{

		Order*pOrd;
		Motorcycle* pMotor;
		while(!AvailableMotors.isEmpty()&&!WaitingOrders.isEmpty())
		{	WaitingOrders.dequeue(pOrd);
		AvailableMotors.dequeue(pMotor);
		pOrd->SetServTime(pOrd->GetDistance()/(pMotor->MotorGetSpeed()));
		pOrd->SetWaitingTime(TimeStep-pOrd->GetArrivalTime());
		pOrd->SetFinishTime(TimeStep+pOrd->GetServTime());
		pMotor->MotorSetFinishTime(pOrd->GetServTime()+pOrd->GetFinishTime());
		pMotor->MotorSetDistance(pOrd->GetServTime()*2);
		InServiceOrders.enqueue(pOrd,-pOrd->GetFinishTime());
		InServiceeMotors.enqueue(pMotor,-pMotor->MotorGetFinishTime());
		if (pMotor->MotorGetType()==TYPE_VIP)
			s+="V "+to_string(pMotor->MotorGetId())+" ";
		else if(pMotor->MotorGetType()==TYPE_FROZ)
			s+="F "+to_string(pMotor->MotorGetId())+" ";
		else
			s+="N "+to_string(pMotor->MotorGetId())+" ";

		//if (pOrd->GetType()==TYPE_VIP)
		s+="(V "+to_string(pOrd->GetID())+")    ";
		/*	else if(pOrd->GetType()==TYPE_FROZ)
		s+="(F "+to_string(pOrd->GetID())+")    ";
		else
		s+="(N "+to_string(pOrd->GetID())+")    ";*/
	}
}
void Restaurant::AssighnOrder_Queue(int TimeStep,Queue <Order*>& WaitingOrders,PQueueLinkedList<Order*,int>& InServiceOrders,PQueueLinkedList<Motorcycle*,int>& AvailableMotors,PQueueLinkedList<Motorcycle*,int>& InServiceeMotors,string&s)
{

		Order*pOrd;
		Motorcycle* pMotor;
		while(!AvailableMotors.isEmpty()&&!WaitingOrders.isEmpty())
		{
			WaitingOrders.dequeue(pOrd);
			AvailableMotors.dequeue(pMotor);
			pOrd->SetServTime(pOrd->GetDistance()/(pMotor->MotorGetSpeed()));
			pOrd->SetWaitingTime(TimeStep-pOrd->GetArrivalTime());
			pOrd->SetFinishTime((TimeStep+pOrd->GetServTime()));
			pMotor->MotorSetFinishTime(pOrd->GetServTime()+pOrd->GetFinishTime());
			pMotor->MotorSetDistance(pOrd->GetServTime()*2);
			InServiceOrders.enqueue(pOrd,-pOrd->GetFinishTime());
			InServiceeMotors.enqueue(pMotor,-pMotor->MotorGetFinishTime());

			if (pMotor->MotorGetType()==TYPE_VIP)
				s+="V "+to_string(pMotor->MotorGetId())+" ";
			else if(pMotor->MotorGetType()==TYPE_FROZ)
				s+="F "+to_string(pMotor->MotorGetId())+" ";
			else
				s+="N "+to_string(pMotor->MotorGetId())+" ";

			//if (pOrd->GetType()==TYPE_VIP)
			//s+="(V "+to_string(pOrd->GetID())+")    ";
			//else if(pOrd->GetType()==TYPE_FROZ)
			s+="(F "+to_string(pOrd->GetID())+")    ";
			//else
			//s+="(N "+to_string(pOrd->GetID())+")    ";
		}
}
void Restaurant::AssighnOrder_List(int TimeStep,LinkedList<Order*>& WaitingOrders,PQueueLinkedList<Order*,int>& InServiceOrders,PQueueLinkedList<Motorcycle*,int>& AvailableMotors,PQueueLinkedList<Motorcycle*,int>& InServiceeMotors,string&s)
{
	
		Order*pOrd;
		Motorcycle* pMotor;
		while(!AvailableMotors.isEmpty()&&!WaitingOrders.isEmpty())
		{

			WaitingOrders.DeleteLast(pOrd);
			AvailableMotors.dequeue(pMotor);
			pOrd->SetServTime(pOrd->GetDistance()/(pMotor->MotorGetSpeed()));
			pOrd->SetWaitingTime(TimeStep-pOrd->GetArrivalTime());
			pOrd->SetFinishTime((TimeStep+pOrd->GetServTime()));
			pMotor->MotorSetFinishTime(pOrd->GetServTime()+pOrd->GetFinishTime());
			pMotor->MotorSetDistance(pOrd->GetDistance()*2);
			InServiceOrders.enqueue(pOrd,-(pOrd->GetFinishTime()));
			InServiceeMotors.enqueue(pMotor,-(pMotor->MotorGetFinishTime()));

			if (pMotor->MotorGetType()==TYPE_VIP)
				s+="V "+to_string(pMotor->MotorGetId())+" ";
			else if(pMotor->MotorGetType()==TYPE_FROZ)
				s+="F "+to_string(pMotor->MotorGetId())+" ";
			else
				s+="N "+to_string(pMotor->MotorGetId())+" ";

			/*if (pOrd->GetType()==TYPE_VIP)
			s+="(V "+to_string(pOrd->GetID())+")    ";
			else if(pOrd->GetType()==TYPE_FROZ)
			s+="(F "+to_string(pOrd->GetID())+")    ";
			else*/
			s+="(N "+to_string(pOrd->GetID())+")    ";

		}
		//int k=WaitingOrders.getSize();
		while(!WaitingOrders.isEmpty()&&TimeStep-WaitingOrders.getEntry(WaitingOrders.getSize())->GetArrivalTime()>=AutoProm)
		{
			WaitingOrders.DeleteLast(pOrd);
			pOrd->SetType(TYPE_VIP);
			if(pOrd)
			{
				switch(pOrd->GetRegion())
				{
				case A_REG:
					AddtoVipOrders(pOrd,0);
					break;
				case B_REG:
					AddtoVipOrders(pOrd,1);
					break;
				case C_REG:
					AddtoVipOrders(pOrd,2);
					break;
				case D_REG:
					AddtoVipOrders(pOrd,3);
					break;
				}
			}
		}
		return;
}

void Restaurant::Finishing(int CurrentTimeStep)
{
	Order *pOrd;
	PQueueLinkedList<Order*,int> TempPQueue;
	for(int i=0;i<4;i++)
	{
		while( AssighnedFROZENOrder[i].peekFront(pOrd) )	
		{
			if(pOrd->GetFinishTime()>CurrentTimeStep )
				break;
			AssighnedFROZENOrder[i].dequeue(pOrd);
			TempPQueue.enqueue(pOrd,-pOrd->GetServTime());
			ServicedFROZENOrders[i]++;
		}

		while( AssighnedVIPOrder[i].peekFront(pOrd) )	
		{
			if(pOrd->GetFinishTime()>CurrentTimeStep )
				break;
			AssighnedVIPOrder[i].dequeue(pOrd);
			TempPQueue.enqueue(pOrd,-pOrd->GetServTime());
			ServicedVIPOrders[i]++;
		}

		while( AssighnedNORMALOrder[i].peekFront(pOrd) )	
		{
			if(pOrd->GetFinishTime()>CurrentTimeStep )
				break;
			AssighnedNORMALOrder[i].dequeue(pOrd);
			TempPQueue.enqueue(pOrd,-pOrd->GetServTime());
			ServicedNORMALOrders[i]++;
		}
	}
	while(!TempPQueue.isEmpty())
	{
		TempPQueue.dequeue(pOrd);
		ServicedOrders.enqueue(pOrd);
	}

	Motorcycle* pMotor;
	for(int i=0;i<4;i++)
	{
		while( InserviseFrozMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			InserviseFrozMotors[i].dequeue(pMotor);
			pMotor->MotorSetFinishTime(0);
			pMotor->MotorSetHealth(pMotor->MotorGetHealth()-pMotor->MotorGetHealth()*(1-pMotor->MotorGetDistance()/1000));
			pMotor->MotorSetDistance(0);
		/*	if(pMotor->MotorGetHealth()<5)
			{
				pMotor->MotorSetFinishTime(CurrentTimeStep+10-pMotor->MotorGetHealth());
				DamagedFrozMotors[i].enqueue(pMotor,-pMotor->MotorGetFinishTime());
				cout<<"\n Motor with Health "<<pMotor->MotorGetHealth()<<" and FinishTime= "<<pMotor->MotorGetFinishTime();
			}
			else
			{*/
				FrozMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
			//}
		}
		while( InserviseNormalMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			InserviseNormalMotors[i].dequeue(pMotor);
			pMotor->MotorSetFinishTime(0);
			pMotor->MotorSetHealth(pMotor->MotorGetHealth()-pMotor->MotorGetHealth()*(1-pMotor->MotorGetDistance()/1000));
			pMotor->MotorSetDistance(0);
			/*if(pMotor->MotorGetHealth()<5)
			{
				pMotor->MotorSetFinishTime(CurrentTimeStep+10-pMotor->MotorGetHealth());
				DamagedNormalMotors[i].enqueue(pMotor,-pMotor->MotorGetFinishTime());
				cout<<"\n Motor with Health "<<pMotor->MotorGetHealth()<<" and FinishTime= "<<pMotor->MotorGetFinishTime();
			}
			else
			{*/
				NormalMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
			//}
		}

		while( InserviseFastMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			InserviseFastMotors[i].dequeue(pMotor);
			pMotor->MotorSetFinishTime(0);
			pMotor->MotorSetHealth(pMotor->MotorGetHealth()-pMotor->MotorGetHealth()*(1-pMotor->MotorGetDistance()/1000));
			pMotor->MotorSetDistance(0);
			/*if(pMotor->MotorGetHealth()<5)
			{
				pMotor->MotorSetFinishTime(CurrentTimeStep+5-pMotor->MotorGetHealth());
				DamagedFastMotors[i].enqueue(pMotor,-pMotor->MotorGetFinishTime());
				cout<<"\n Motor with Health "<<pMotor->MotorGetHealth()<<" and FinishTime= "<<pMotor->MotorGetFinishTime();
			}
			else
			{*/
				FastMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
		//	}
		}
	}
}
void Restaurant::Repair(int CurrentTimeStep)
{
	Motorcycle* pMotor;
	for(int i=0;i<4;i++)
	{
		while( DamagedFrozMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			DamagedFrozMotors[i].dequeue(pMotor);
			pMotor->MotorSetFinishTime(0);
			pMotor->MotorSetHealth(10);
			pMotor->MotorSetDistance(0);

			FrozMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
		}
		while( DamagedNormalMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			DamagedNormalMotors[i].dequeue(pMotor);
			pMotor->MotorSetFinishTime(0);
			pMotor->MotorSetHealth(10);
			pMotor->MotorSetDistance(0);
			NormalMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
		}

		while( DamagedFastMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			DamagedFastMotors[i].dequeue(pMotor);
			pMotor->MotorSetFinishTime(0);
			pMotor->MotorSetHealth(10);
			pMotor->MotorSetDistance(0);
			FastMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
		}
	}
}
bool Restaurant::AllOrdersAREserved()
{ 
	int checkV=0;
	int checkF=0;
	int checkN=0;
	for (int i = 0; i < 4; i++)
	{  if(ActiveVIPOrder[i].isEmpty()) 
	checkV++;
	if(ActiveNORMALOrder[i].isEmpty()) 
		checkN++;
	if(ActiveFROZENOrder[i].isEmpty()) 
		checkF++;
	}


	if (checkV==4 && checkF==4 && checkN==4 )
		return true;
	else
		return false;

}
bool Restaurant::AllMotorsCameBack()
{

	int checkV=0;
	int checkF=0;
	int checkN=0;
	for (int i = 0; i < 4; i++)
	{  if(InserviseFastMotors[i].isEmpty()) 
	checkV++;
	if(InserviseNormalMotors[i].isEmpty()) 
		checkN++;
	if(InserviseFrozMotors[i].isEmpty()) 
		checkF++;
	}


	if (checkV==4 && checkF==4 && checkN==4 )
		return true;
	else
		return false;


}
void Restaurant::Output()
{
	string filename;
	ofstream outfile;
	pGUI->PrintMessage("Enter outputfile name ^_^");
	filename=pGUI->GetString()+".txt";
	outfile.open(filename);
	if(outfile.is_open(),ios::trunc)
	{
		outfile<<"FT" <<"\t"<<"ID"<<"\t"<<"AT"<<"\t"<<"WT"<<"\t"<<"ST"<<endl;
	/*	int NA=0;
		int VA=0;
		int FA=0;
		int NB=0;
		int VB=0;
		int FB=0;
		int NC=0;
		int VC=0;
		int FC=0;
		int ND=0;
		int VD=0;
		int FD=0;
		int n[4];
        int v[4];
		int f[4];
		int O[4];*/
	while(!ServicedOrders.isEmpty())
	{
		Order*ptr; 
		ServicedOrders.dequeue(ptr);
			outfile<<ptr->GetFinishTime()<<"\t"<<ptr->GetID()<<"\t"<<ptr->GetArrivalTime()<<"\t"<<ptr->GetWaitingTime()<<"\t"<<ptr->GetServTime()<<endl;
	}
//			if(ptr->GetRegion()==0&&ptr->GetType()==0)
//		{
//	     NA++;
//	     n[0]=NA;
//		}
//		else if(ptr->GetRegion()==0&&ptr->GetType()==1)
//		{
//			FA++;
//			f[0]=FA;
//		}
//		else if(ptr->GetRegion()==0&&ptr->GetType()==2)
//		{
//		VA++;
//			v[0]=VA;
//		}
//		else if(ptr->GetRegion()==1&ptr->GetType()==0)
//		{
//			NB++;
//			n[1]=NB;
//		}
//		else if(ptr->GetRegion()==1&&ptr->GetType()==1)
//		{
//		FB++;
//		f[1]=FB;
//		}
//		else if(ptr->GetRegion()==1&&ptr->GetType()==2)
//		{
//			VB++;
//			v[1]=VB;
//		
//		}
//		else if(ptr->GetRegion()==2&&ptr->GetType()==0)
//		{
//			NC++;
//			n[2]=NC;
//		}
//		else if(ptr->GetRegion()==2&&ptr->GetType()==1)
//		{
//		FC++;
//			f[2]=FC;
//		}
//		else if(ptr->GetRegion()==2&&ptr->GetType()==2)
//		{
//			VC++;
//			v[2]=VC;
//			
//		}
//
//		else if(ptr->GetRegion()==3&&ptr->GetType()==0)
//		{
//			ND++;
//			n[3]=ND;
//		}
//else if(ptr->GetRegion()==3&&ptr->GetType()==1)
//		{
//			FD++;
//		f[3]=FD;
//		}
//		else if(ptr->GetRegion()==3&&ptr->GetType()==2)
//		{
//			VD++;
//			v[3]=VD;
//}
//O[0]=n[0]+v[0]+f[0];
//O[1]=n[1]+v[1]+f[1];
//O[2]=n[2]+v[2]+f[2];
//O[3]=n[3]+v[3]+f[3];
//	}
	for(int i=0;i<4;i++)
	{
			outfile<<"Region"<<" "<<Region(i)<<":"<<endl;
		int m=normal[i]+frozen[i]+vip[i];
		
	outfile<<"  "<<"Orders:"<<ServicedNORMALOrders[i]+ServicedFROZENOrders[i]+ServicedVIPOrders[i]<<" "<<"Norm:"<<ServicedNORMALOrders[i]<<" "<<"Froz:"<<ServicedFROZENOrders[i]<<" "<<"VIP:"<<ServicedVIPOrders[i]<<endl;
	outfile<<"  "<<"Motocycles:"<<NormalMotors[i].getSize()+FrozMotors[i].getSize()+FastMotors[i].getSize()<<"  "<<"Norm:"<<NormalMotors[i].getSize()<<" "<<"Froz:"<<FrozMotors[i].getSize()<<" "<<"Fast:"<<FastMotors[i].getSize()<<endl;

	}
	outfile.close();
}
}
char Restaurant::Region(int m)
{
	char p;
	if(m==0)
	{
		p='A';
	}
	else	if(m==1)
	{
		p='B';
	}
	else if(m==2)
	{
		p='C';
	}
	else if(m==3)
	{
		p='D';
	}

	return p;
}