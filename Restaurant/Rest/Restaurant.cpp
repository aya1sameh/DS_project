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
	
	while(!EventsQueue.isEmpty() || !AllOrdersAREserved() || !AllMotorsCameBack())
	{    AssignOrdersToMotor="";
		ExecuteEvents(CurrentTimeStep);

		AssighnOrdersToMotors(CurrentTimeStep,AssignOrdersToMotor);
		Finishing(CurrentTimeStep);
		ResetWindowALL();
		pGUI->PrintMessage("Click on the mouse to deliver the next order  "+to_string(CurrentTimeStep));
		printOutput(AssignOrdersToMotor);
		CurrentTimeStep++;
		pGUI->waitForClick();
	}
	AssighnOrdersToMotors(CurrentTimeStep,AssignOrdersToMotor);
	Finishing(CurrentTimeStep);
	ResetWindowALL();
	pGUI->PrintMessage("generation done, click to END program");

	/////////////////////////////////////////////////////////////
	//Just to test
	for(int i=11;i<300;i++)
	{
		Finishing(i);
	}
	/*while(!ServicedOrders.isEmpty())
	{
		Order*ptr;
		ServicedOrders.dequeue(ptr);
		cout<<"\n Order with ID: "<<ptr->GetID()<<" Finish Time: "<<ptr->GetFinishTime()<<" Waiting Time: "<<ptr->GetWaitingTime()<<" Arrival Time "<<ptr->GetArrivalTime()<<" Service Time: "<<ptr->GetServTime();	
	}*/
	//cout<<"\n----------------------------------------------------\n Please Work \n";
	//for (int i=0;i<4;i++)
	//{
	//	while(!AssighnedVIPOrder[i].isEmpty())
	//	{
	//		Order*ptr;
	//		AssighnedVIPOrder[i].dequeue(ptr);
	//		cout<<"\n Order with ID: "<<ptr->GetID()<<" Finish Time: "<<ptr->GetFinishTime()<<" Waiting Time: "<<ptr->GetWaitingTime()<<" Arrival Time "<<ptr->GetArrivalTime()<<" Service Time: "<<ptr->GetServTime();
	//	}
	//	cout<<"\n----------------------------------------------------\n";
	//}
	//for (int i=0;i<4;i++)
	//{
	//	while(!AssighnedNORMALOrder[i].isEmpty())
	//	{
	//		Order*ptr;
	//		AssighnedNORMALOrder[i].dequeue(ptr);
	//		cout<<"\n Order with ID: "<<ptr->GetID()<<" Finish Time: "<<ptr->GetFinishTime()<<" Waiting Time: "<<ptr->GetWaitingTime()<<" Arrival Time "<<ptr->GetArrivalTime()<<" Service Time: "<<ptr->GetServTime();
	//	}
	//	cout<<"\n----------------------------------------------------\n";
	//}
	//for (int i=0;i<4;i++)
	//{
	//	while(!AssighnedFROZENOrder[i].isEmpty())
	//	{
	//		Order*ptr;
	//		AssighnedFROZENOrder[i].dequeue(ptr);
	//		cout<<"\n Order with ID: "<<ptr->GetID()<<" Finish Time: "<<ptr->GetFinishTime()<<" Waiting Time: "<<ptr->GetWaitingTime()<<" Arrival Time "<<ptr->GetArrivalTime()<<" Service Time: "<<ptr->GetServTime();
	//	}
	//	cout<<"\n----------------------------------------------------\n";
	//}
	//
	////////////////////////////////////////////////////////////////

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

	int normal[4],frozen[4],vip[4];
	for(int i=0;i<4;i++)
	{
		inputFile>>normal[i]>>frozen[i]>>vip[i];
	}
	///////////////////////////////////////////////
	for(int i=0;i<normal[0];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s1);
		NormalMotors[0].enqueue(pM,s1);
	}
	for(int i=0;i<frozen[0];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s2);
		FrozMotors[0].enqueue(pM,s2);
	}
	for(int i=0;i<vip[0];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s3);
		FastMotors[0].enqueue(pM,s3);
	}
	/////////////////////////////////////////////
	for(int i=0;i<normal[1];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s1);
		NormalMotors[1].enqueue(pM,s1);
	}
	for(int i=0;i<frozen[1];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s2);
		FrozMotors[1].enqueue(pM,s2);
	}
	for(int i=0;i<vip[1];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s3);
		FastMotors[1].enqueue(pM,s3);
	}
	///////////////////////////////////////////
	for(int i=0;i<normal[2];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s1);
		NormalMotors[2].enqueue(pM,s1);
	}
	for(int i=0;i<frozen[2];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s2);
		FrozMotors[2].enqueue(pM,s2);
	}
	for(int i=0;i<vip[2];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s3);
		FastMotors[2].enqueue(pM,s3);
	}
	///////////////////////////////////////////
	for(int i=0;i<normal[3];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s1);
		NormalMotors[3].enqueue(pM,s1);
	}
	for(int i=0;i<frozen[3];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s2);
		FrozMotors[3].enqueue(pM,s2);
	}
	for(int i=0;i<vip[3];i++)
	{
		Motorcycle* pM=new Motorcycle(i+1,s3);
		FastMotors[3].enqueue(pM,s3);
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
	int k=WaitingOrders.getSize();
	for(int i=0;i<k;i++)
	{
		Order*pOrd;
		Motorcycle* pMotor;
		if(!AvailableMotors.isEmpty())
		{
			WaitingOrders.dequeue(pOrd);
			AvailableMotors.dequeue(pMotor);
			pOrd->SetServTime(pOrd->GetDistance()/(pMotor->MotorGetSpeed()));
			pOrd->SetWaitingTime(TimeStep-pOrd->GetArrivalTime());
			pOrd->SetFinishTime(TimeStep+pOrd->GetServTime());
			pMotor->MotorSetFinishTime(pOrd->GetServTime()+pOrd->GetFinishTime());
			InServiceOrders.enqueue(pOrd,-pOrd->GetFinishTime());
			InServiceeMotors.enqueue(pMotor,-pMotor->MotorGetFinishTime());
			
			if (pMotor->MotorGetType()==TYPE_VIP)
				s+="V "+to_string(pMotor->MotorGetId())+" ";
			else if(pMotor->MotorGetType()==TYPE_FROZ)
				s+="F "+to_string(pMotor->MotorGetId())+" ";
			else
				s+="N "+to_string(pMotor->MotorGetId())+" ";
			
			if (pOrd->GetType()==TYPE_VIP)
				s+="(V "+to_string(pOrd->GetID())+")    ";
			else if(pOrd->GetType()==TYPE_FROZ)
				s+="(F "+to_string(pOrd->GetID())+")    ";
			else
				s+="(N "+to_string(pOrd->GetID())+")    ";
		}
		else
			return;
	}
}
void Restaurant::AssighnOrder_Queue(int TimeStep,Queue <Order*>& WaitingOrders,PQueueLinkedList<Order*,int>& InServiceOrders,PQueueLinkedList<Motorcycle*,int>& AvailableMotors,PQueueLinkedList<Motorcycle*,int>& InServiceeMotors,string&s)
{
	int k=WaitingOrders.getSize();
	for(int i=0;i<k;i++)
	{
		Order*pOrd;
		Motorcycle* pMotor;
		if(!AvailableMotors.isEmpty())
		{
			WaitingOrders.dequeue(pOrd);
			AvailableMotors.dequeue(pMotor);
			pOrd->SetServTime(pOrd->GetDistance()/(pMotor->MotorGetSpeed()));
			pOrd->SetWaitingTime(TimeStep-pOrd->GetArrivalTime());
			pOrd->SetFinishTime((TimeStep+pOrd->GetServTime()));
			pMotor->MotorSetFinishTime(pOrd->GetServTime()+pOrd->GetFinishTime());
			InServiceOrders.enqueue(pOrd,-pOrd->GetFinishTime());
			InServiceeMotors.enqueue(pMotor,-pMotor->MotorGetFinishTime());

			if (pMotor->MotorGetType()==TYPE_VIP)
				s+="V "+to_string(pMotor->MotorGetId())+" ";
			else if(pMotor->MotorGetType()==TYPE_FROZ)
				s+="F "+to_string(pMotor->MotorGetId())+" ";
			else
				s+="N "+to_string(pMotor->MotorGetId())+" ";
			
			if (pOrd->GetType()==TYPE_VIP)
				s+="(V "+to_string(pOrd->GetID())+")    ";
			else if(pOrd->GetType()==TYPE_FROZ)
				s+="(F "+to_string(pOrd->GetID())+")    ";
			else
				s+="(N "+to_string(pOrd->GetID())+")    ";
		}
		else
			return;
	}
}
void Restaurant::AssighnOrder_List(int TimeStep,LinkedList<Order*>& WaitingOrders,PQueueLinkedList<Order*,int>& InServiceOrders,PQueueLinkedList<Motorcycle*,int>& AvailableMotors,PQueueLinkedList<Motorcycle*,int>& InServiceeMotors,string&s)
{
	int k=WaitingOrders.getSize();
	for(int i=0;i<k;i++)
	{
		Order*pOrd;
		Motorcycle* pMotor;
		if(!AvailableMotors.isEmpty())
		{
			WaitingOrders.DeleteLast(pOrd);
			AvailableMotors.dequeue(pMotor);
			pOrd->SetServTime(pOrd->GetDistance()/(pMotor->MotorGetSpeed()));
			pOrd->SetWaitingTime(TimeStep-pOrd->GetArrivalTime());
			pOrd->SetFinishTime((TimeStep+pOrd->GetServTime()));
			pMotor->MotorSetFinishTime(pOrd->GetServTime()+pOrd->GetFinishTime());
			InServiceOrders.enqueue(pOrd,-(pOrd->GetFinishTime()));
			InServiceeMotors.enqueue(pMotor,-(pMotor->MotorGetFinishTime()));

		if (pMotor->MotorGetType()==TYPE_VIP)
				s+="V "+to_string(pMotor->MotorGetId())+" ";
			else if(pMotor->MotorGetType()==TYPE_FROZ)
				s+="F "+to_string(pMotor->MotorGetId())+" ";
			else
				s+="N "+to_string(pMotor->MotorGetId())+" ";
			
			if (pOrd->GetType()==TYPE_VIP)
				s+="(V "+to_string(pOrd->GetID())+")    ";
			else if(pOrd->GetType()==TYPE_FROZ)
				s+="(F "+to_string(pOrd->GetID())+")    ";
			else
				s+="(N "+to_string(pOrd->GetID())+")    ";
			
		}
		else
			/*if(TimeStep-pOrd->GetArrivalTime()>10)
			{
				Promote this order
			}*/
			return;
	}
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
			ServicedOrders.enqueue(pOrd,pOrd->GetFinishTime());
	}

	Motorcycle* pMotor;
	for(int i=0;i<4;i++)
	{
		while( InserviseFrozMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			InserviseFrozMotors[i].dequeue(pMotor);
			FrozMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
		}
		while( InserviseNormalMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			InserviseNormalMotors[i].dequeue(pMotor);
			NormalMotors[i].enqueue(pMotor,pMotor->MotorGetSpeed());
		}

		while( InserviseFastMotors[i].peekFront(pMotor) )	
		{
			if(pMotor->MotorGetFinishTime()>CurrentTimeStep )
				break;
			InserviseFastMotors[i].dequeue(pMotor);
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