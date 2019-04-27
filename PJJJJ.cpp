#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

const int MAX_Cashier = 5;   		//we only have 5 normal cashier
const int MAX_Express_Cashier = 1;  //only one express line
const int Time_flow = 13;			//usually the average time of one customer coming to queue is 13s

int queue_plate[3][15]= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			//show the normal queue
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
						  
int express_plate[7]= {0,0,0,0,0,0,0};								//show the express queue

struct grocery_basket{				//set limit to the customers' basket
	string product[100];
	float Price[100];
	float Ptime[100];
	
};

struct customers					//record the No. of the customer, the total processing time, their income, the money they sepent, what they like to buy, who are they and their basket
{
	int id;
	float time;
	float money;
	float spent;
	string favor;
	string Class_;
	grocery_basket brought;
	customers *next;
};

struct cashier{						//record the No. of customer when they come to the cashier
	int id ;
	float time;						//the processing time
	string cust_class;				//customer's class
	float money_used;				//the money they spent = the money we earn
	bool ON_DUTY;					//is the cashier open


};


						  
void Read_From_Grogecy(string NType[],string NID[],string Nname[], string Nprice[], string Ntime[]){	//read our grogecy file
	
	ifstream fin1("grocery.txt");
	int skip = 0, i = 0 ;

	
	if (fin1.fail()) {
		cout << "Error in open file!";
		exit(1);
	}
	
	while (fin1 >> NType[i]) {
		if(skip < 5){
			skip++;
			continue;
		}
		
		fin1 >>  NID[i] >> Nname[i] >> Nprice[i] >> Ntime[i];
		i++;
	}
	fin1.close();

}
void Read_From_Customer(string NClass[], string Nincome[], string NFavor[]){							//read our customer file based on the Questionnaire
	
	ifstream fin2("customer.txt");
	if (fin2.fail()) {
		cout << "Error in open file!";
		exit(1);
	}
	int skip = 0, j = 0;

	while (fin2 >> NClass[j]) {
		if(skip < 3){
			skip++;
			continue;
		}
		fin2 >>  Nincome[j] >> NFavor[j];
		j++;
	}
	fin2.close();
	
}
void Write_To_Profit(float revenue, int Finish_payment, int Times_of_register){							// write down sale information after the simulate

	ofstream fout;
    fout.open("Profit.txt");

    if ( fout.fail() ) {
       cout << "Error in file opening!" 				<< endl;
       exit(1);
    }

    fout << "The number of customer who finished payment is " <<Finish_payment << endl; 
	fout << "We have earned  " << revenue << endl;
	fout << "The register cost is  " << (Times_of_register + 2) * 200 << endl;
	fout << "The Profit is  " << revenue - (Times_of_register + 2) * 200 << endl;
    fout.close();

 
}
					  
void appendID_to_queue(customers *&head, customers *&E_head, int i)    // append customer to queue
{	
	string *NType = new string [36];
	string *NID = new string [36];
	string *Nname = new string [36];
	string *Nprice = new string [36];
	string *Ntime = new string [36];
	
	string *NClass = new string [8];
	string *Nincome = new string [8];
	string *NFavor = new string [8];
	

	Read_From_Grogecy(NType,NID,Nname,Nprice, Ntime);
	Read_From_Customer(NClass,Nincome,NFavor);
	
	customers *newcustomers = new customers;
	newcustomers->time = 0;
	
	int random_p = rand() % 8;					// randomly generate a customer
	
	newcustomers->Class_ = NClass[random_p];		//assign the random number to take the Class, income and Favor
	newcustomers->money = stof(Nincome[random_p]);
	newcustomers->favor = NFavor[random_p];
	
	float intial_money = newcustomers->money;		//mark the budget
	int amount_OF_product = 0;						//how many did they buy
	newcustomers->id = i;
	
	if (newcustomers->favor == "All"){			//if they want everything
		int k = 0;
		int t = 0;
		while(newcustomers->money > 0){		// assume they will use most of the budget
			int rannum = rand() % 36;
			if(stof(Nprice[rannum]) > newcustomers->money){		//prvent overbudget
				t++;
				if(t > 12){		//random generate until > 12
					break;
				}
				continue;
			}
			
			newcustomers->brought.product[k] = Nname[rannum];	//store
			newcustomers->brought.Price[k] = stof(Nprice[rannum]);
			newcustomers->brought.Ptime[k] = stof(Ntime[rannum]);

			newcustomers->money -= newcustomers->brought.Price[k];
			newcustomers->time += newcustomers->brought.Ptime[k];
			k++;
			amount_OF_product++;			//how many did they buy

		}
	}
	else if (newcustomers->favor != "All"){	//if they don't wanna buy anything
		int k = 0;
		int t = 0;
		
		while(newcustomers->money > 0){			// assume they will use most of the budget
			int rannum = rand() % 36;
			if(newcustomers->favor.find(NType[rannum]) != -1){
				if(stof(Nprice[rannum]) > newcustomers->money){	//prvent overbudget
					t++;
					if(t > 50){			//random generate until > 12
						break;
					}
				continue;
				}
			
			newcustomers->brought.product[k] = Nname[rannum];
			newcustomers->brought.Price[k] = stof(Nprice[rannum]);
			newcustomers->brought.Ptime[k] = stof(Ntime[rannum]);
			newcustomers->money -= newcustomers->brought.Price[k];
			newcustomers->time += newcustomers->brought.Ptime[k];
			k++;
			amount_OF_product++;

			}
		}
	}
	newcustomers->spent = intial_money - newcustomers->money; 
	newcustomers->next = NULL;
	
	if ( amount_OF_product + 1 > 5){			//if grocery > 5
		if (head == NULL) {						//use normal head
			head = newcustomers;
		}
		else {
			customers *current = head;
			while (current->next != NULL)
			{
				current = current->next;
			}
			current->next = newcustomers;
		}
	}
	else if (amount_OF_product + 1 <= 5){		//if grocery <= 5
		if (E_head == NULL) {					//use express head
			E_head = newcustomers;
		}
		else {
			customers * E_current = E_head;
			while (E_current->next != NULL)
			{
				E_current = E_current->next;
			}
			E_current->next = newcustomers;
		}
	}

	delete newcustomers;
	newcustomers = 0;
	delete []NType;
	delete []NID;
	delete []Nname;
	delete []Nprice;
	delete []Ntime;
	
	
	NType = 0;
	NID = 0;
	Nname = 0;
	Nprice = 0;
	Ntime = 0;
	
	delete []NClass;
	delete []Nincome;
	delete []NFavor;	
		
	NClass = 0;
	Nincome = 0;
	NFavor = 0;	
}

void Qtime(customers *&head, float &total_time) 		// only count the time of normal queue since not many people go to express line
{ 
    customers *p = new customers;
    p = head;
    if (p == NULL) 
        return; 
  
    Qtime(p->next, total_time); 
    total_time = total_time + p->time; 
    delete p;
    p = 0;
} 
void Qpeople(customers *&head, int &total_people) 		//only count the people of normal queue since not many people go to express line
{   
    customers *p = new customers;
    p = head;
    int counter = 0;
    while (p->next != NULL)
		{
			p = p->next;
			counter++;
		}
	
	total_people = counter + 1;
	if (p->id == head->id ){
		total_people = 1;
	}
	delete p;
    p = 0;

} 



					  
void customer_generator(int &total_customers, customers *&head, customers *&E_head){ // generate customer
		total_customers++;
		appendID_to_queue(head, E_head, total_customers);
}

void initialize_array(){										//reset normal queue
	for (int i = 0; i < 3; i++){
		for(int j = 0; j < 15; j++){
			queue_plate[i][j] = 0;
		}
	}

}

void initialize_E_array(){										//reset express queue
	for(int k  = 0;  k < 7; k++){
		express_plate[k] = 0;
	}
}

void printPlate(){												//print all the queue
	int k = 0;
	for (int i = 0; i < 3; i++ ){
		if (i % 2 == 0){
			cout << "     |-------------------------------------------------------| ";
			if (express_plate [k] == 0)
				cout << setw(3) << " " ;
			else
				cout << setw(3) << express_plate[k];	
			
			cout << "  |" << endl;
			k++ ;
		}
			
		
		else{
			cout << "---------------------------------------------------------|   | ";
			if (express_plate [k] == 0)
				cout << setw(3) << " " ;
			else
				cout << setw(3) << express_plate[k]; 
			cout << "  |" << endl;
			k++;
		}
			
		for (int j = 0; j < 15; j++){
			if (queue_plate[i][j] == 0)
				cout << setw(4) <<" ";
			else
				cout << setw(4) << queue_plate[i][j];	
			
			if (j == 14 ){
				cout << " | ";
				if (express_plate [k] == 0)
					cout << setw(3) <<" " ;
				else
					cout << setw(3) << express_plate[k]; 
				cout << "  |" ;
				k++;
			}
		}
		cout << endl;
		
	}
	k++;
	cout << "------------------------------------------------------|      | ";
	if (express_plate [k] == 0)
				cout << setw(3) << " " ;
			else
				cout << setw(3) << express_plate[k]; 
			cout << "  |\n\n" ;
}

void initialize_cashier(cashier machine[], cashier express[]){				//initialize all the cashier
	for(int i = 0; i < MAX_Cashier; i++){
			machine[i].id = 0;
			machine[i].time = 0;
			if(i == 0){
				machine[i].ON_DUTY = true;
			}
			else{
				machine[i].ON_DUTY = false;
			}
			
		}
	for(int i = 0; i < MAX_Express_Cashier; i++){
			express[i].id = 0;
			express[i].time = 0;
			express[i].ON_DUTY = true;

			
		}
	
	
}

void Print_cashier(cashier machine[], cashier express[])							//print all the cashier include express 
{	
	cout << setw(10) << "cashier_1"  << setw(12)  << "cashier_2"<< setw(12) << "cashier_3"
		 << setw(12) << "cashier_4"<< setw(12) << "cashier_5"<< setw(12) << "Express_C"<<endl;
	cout << setw(10) << "========="  << setw(12)  << "========="<< setw(12) << "========="
		 << setw(12) << "========="<< setw(12) << "========="<< setw(12) << "========="<<endl;


	cout << setw(6);
	for(int j = 0; j < MAX_Cashier ; j++){
		if(machine[j].id != 0)
			cout <<  machine[j].id << setw(12);
		else{
			cout  << " " << setw(12);
		}
	}
	if(express[0].id != 0)
		cout << setw(12) << express[0].id ;
	else
		cout  <<" " << setw(12);
		
	cout << endl << endl;
	cout << setw(8) << "Remain: ";											//also show the remaining time of the cashier
	for(int j = 0; j < MAX_Cashier ; j++)
		cout << machine[j].time << setw(1) << "s" << setw(11);
	cout << express[0].time << setw(1) << "s";
	cout << endl <<endl;

}

void append_to_cashier(cashier machine[], customers *&Next){				//append customer to normal cashier

	for (int i = 0; i < MAX_Cashier; i++){
		if(machine[i].ON_DUTY == true && machine[i].time <= 0 ){
			machine[i].id = Next->id;
			machine[i].time = Next->time;
			machine[i].cust_class = Next->Class_;
			machine[i].money_used = Next->spent;
			
			break;
		}
	}
	
}

void append_to_express(cashier express[], customers *E_Next){				//append customer to express cashier
	if(express[0].ON_DUTY == true && express[0].time <= 0 ){
		express[0].id = E_Next->id;
		express[0].time = E_Next->time;
		express[0].cust_class = E_Next->Class_;
		express[0].money_used = E_Next->spent;
	}
}

void save_in_array(customers *&head){							//save the updated customer info to the normal queue
	customers *current = head;

	for (int i = 0; i < 3; i++ ){
		if (i % 2 == 0){
			for (int j = 0; j < 15; j++){
				
				if (current == NULL){
					break;
				}
				queue_plate[i][j] = current->id;
				current = current->next;
			}	
		}		
		else{
			for (int j = 14; j >= 0; j--){				//reverse  printing 
				
				if (current == NULL){
					break;
				}
				queue_plate[i][j] = current->id ;
				current = current->next;
			}
				
		}
	}
} 

void save_in_E_array(customers *&E_head){			//save the updated customer info to the express queue

	customers *E_current =  E_head;

	for(int k  = 0;  k < 7; k++){
		if (E_current == NULL){

			break;
		}
		express_plate[k] = E_current->id;
		E_current = E_current->next;
	}
} 

bool IS_OVER_15mins(float total_time, cashier machine[]){					//check if the queue is overtime
	float counter = 0;
	for(int i = 0; i < MAX_Cashier; i++){
		if (machine[i].ON_DUTY == true)
				counter ++;
		}
	
	if(total_time / counter > 900)					//the average waiting time depends on how many cashiers are using
	{	
		return true;
	}
	else if (total_time / counter < 900)
	{
		return false;
	}
}

void pop_tail(customers *&head){						//the last customer will leave if he knows the waiting time exceed  5 mins 
	customers *p = new customers ;
	p = head;
	customers *prev = head;
    while (p->next != NULL)
		{
			prev = p;
			p = p->next;
		}
	prev->next = NULL;
	cout << "Customer No."<<p->id <<" was waiting to long and left.\n";
	delete p;
	p = 0;
}

customers *pop_queue(customers *&head){					//assign customer in the normal queue to cashier
	customers* current = head;
	head = current->next;
	current->next = NULL;
	return current;
}
 
customers *pop_E_queue(customers *&E_head){				//assign customer in the express queue to cashier
	customers* E_current = E_head;
	E_head = E_current->next;
	E_current->next = NULL;
	return E_current;
}


void MARKET_PLAXX()										//the simulation
{	
	int queue_plate[3][15]= { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,			//show the normal queue
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
						  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
						  
	int express_plate[7]= {0,0,0,0,0,0,0};								//show the express queue
	customers *head = NULL;
	customers *E_head = NULL;	
	cashier machine[MAX_Cashier];
	cashier express[MAX_Express_Cashier];
	initialize_array();
	initialize_E_array();
	initialize_cashier(machine,express);
	
	
	string input;
	
	float total_time = 0;
	int total_people = 0;
	float revenue = 0;
	int Times_of_register = 0;
	int Finish_payment = 0;
	int total_customers = 0;

	customer_generator(total_customers,head, E_head);


	if(head != NULL){						//if normal queue have customer, save the updated info to the normal line
		save_in_array(head);
	}
	else if(E_head != NULL){				//if normal queue have customer, save the updated info to the express line
		save_in_E_array(E_head);
	}
	Print_cashier(machine, express);
	printPlate();
	if (head != NULL)						// if normal queue have customer, calculate the time and people
	{
		Qtime(head, total_time);
		Qpeople(head, total_people);

	}

	cout << total_people << " people in the queue." << endl;		//show the updated info
	cout << "The total time of the normal queue is " << total_time << "s ." << endl << endl;
	cout << "Next? (y/n) :" ;										//goto next step
	cin >> input;
	
	system("clear");
	while (input != "N"){
		
		float total_time = 0 ;
		int People_in_Queue = 0;
		bool over_15mins;
		
		customer_generator(total_customers,head, E_head);
		
		if (head != NULL  && E_head == NULL) {								//if normal line has customer but express hasn't
			for(int i = 0; i < MAX_Cashier; i++){							//assign people to normal cashier
				if (machine[i].time  <= 0 && machine[i].ON_DUTY ==  true ){
					customers *Next = pop_queue(head);
					append_to_cashier(machine, Next);
					revenue += machine[i].money_used;						//store the revenue when the customer in the cashier but not queue
					Finish_payment++;										//store the customer that go to the cashier and make the payment

					
				}
			}
			initialize_array();
			save_in_array(head);
		}
		
		else if (E_head != NULL && head == NULL) {						//if express line has customer but normal hasn't
			if(express[0].time  <= 0 && express[0].ON_DUTY ==  true){ 	//assign people to normal cashier
				customers *E_Next = pop_queue(E_head);
				append_to_express(express, E_Next);
				revenue += express[0].money_used;						//store the revenue when the customer in the cashier but not queue
				Finish_payment++;										//store the customer that go to the cashier and make the payment

			}
			initialize_E_array();
			save_in_E_array(E_head);
		}
		
		else if(head == NULL && E_head == NULL)							// if both of the line don't have customer, generate one
			customer_generator(total_customers,head, E_head);
			
		else if(head != NULL && E_head != NULL){						// if both of the line  have customer, 
			for(int i = 0; i < MAX_Cashier; i++){						//assign people to normal cashier
				if (machine[i].time  <= 0 && machine[i].ON_DUTY ==  true ){
					customers *Next = pop_queue(head);
					append_to_cashier(machine, Next);
					revenue += machine[i].money_used;
					Finish_payment++;
					
				}
			}
			
			initialize_array();
			save_in_array(head);
			
			if(express[0].time  <= 0 && express[0].ON_DUTY ==  true){ 	//assign people to express cashier
				customers *E_Next = pop_queue(E_head);
				append_to_express(express, E_Next);
				revenue += express[0].money_used;
				Finish_payment++;

			}
			initialize_E_array();
			save_in_E_array(E_head);
		}
			
		Print_cashier(machine,express);
		printPlate();
		
		if (head != NULL)							// if normal queue have customer, calculate the time and people
		{
			Qtime(head, total_time);
			Qpeople(head, total_people);
		}

		cout << "After 13 seconds," << endl;		//show updated info
		cout << total_people << " people in the queue." << endl;
		cout << "The total time of the normal queue is " << total_time << "s ." << endl;
		
		for(int i = 0; i < MAX_Cashier; i++){		//show info and track of the customer when he goes to normal cashier
			if (machine[i].time  > 0 && machine[i].ON_DUTY ==  true){
				cout << "Customer No."<< machine[i].id <<" is a "<< machine[i].cust_class <<". The Customer goes to cashier "<< i+1 << " and spends $" << machine[i].money_used << ".\n";
				machine[i].time -= Time_flow;		//reduce the time every second when you input "y"
			}
			if (i == 0)			//always open cashier_1
				continue;
			if (total_people == (MAX_Cashier + 1) * i  &&  machine[i].ON_DUTY == false){		//to open a new cashier, the line should have 6, 12, 18, 24, 30 people
				machine[i].ON_DUTY = true;
				cout << "Cashier " << i + 1<< " is opened.\n";
				cout << "Register fee costs $200\n";
				Times_of_register++;															//mark down how many time did it open to calculate the cost
				
			}
			else if (total_people < 3*i &&  machine[i].ON_DUTY == true && machine[i].time <= 0){	// to close the cashier, the line should be below 3,6,9,12, so no need alway open a new cashier and save money
					machine[i].ON_DUTY = false;
					machine[i].id = 0;
					machine[i].time = 0; 
					cout << "Cashier " << i + 1<< " is closed.\n";
			}
			
		}
		if (express[0].time  > 0 && express[0].ON_DUTY ==  true){		//show info and track of the customer when he goes to express cashier
				cout << "Customer No."<< express[0].id <<" is a "<< express[0].cust_class <<". The Customer goes to Express and spends $" << express[0].money_used << ".\n";
				express[0].time -= Time_flow;
		}
		if (express[0].time  <= 0){			
			express[0].id = 0;
			express[0].time = 0; 
			
	
		}
		
		over_15mins = IS_OVER_15mins(total_time, machine);	//check overtime
		if (over_15mins == true){
			pop_tail(head);									//the last customer leave
		}


		cout << "\nNext? (Y/N) :" ;							//goto next
		cin >> input;
		if(input == "n")
			Write_To_Profit(revenue, Finish_payment, Times_of_register);			//if you don't wanna simulate anymore, store the final data to the profit.txt

		system("clear");
		
	}
}
void LOOK_Grogecy(){						//list our grocery
	string *NType = new string [36];
	string *NID = new string [36];
	string *Nname = new string [36];
	string *Nprice = new string [36];
	string *Ntime = new string [36];
		
	Read_From_Grogecy(NType,NID,Nname,Nprice, Ntime);

	int j = 0;
	
	for (int i = 0; i < 36; i++){
		cout << NType[i] <<" " << NID[i] << " "<< Nname[i] << " " << Nprice[i] << " " << Ntime[i] <<endl;
	}


	delete []NType;
	delete []NID;
	delete []Nname;
	delete []Nprice;
	delete []Ntime;
	
	
	NType = 0;
	NID = 0;
	Nname = 0;
	Nprice = 0;
	Ntime = 0;

}
void LOOK_Customer_Base(){				//list our customer
	
	
	string *NClass = new string [8];
	string *Nincome = new string [8];
	string *NFavor = new string [8];
	
	Read_From_Customer(NClass,Nincome,NFavor);

	int j = 0;
	
	for (int i = 0; i < 8; i++){
		cout << NClass[i] <<" " << Nincome[i] << " "<< NFavor[i] <<endl;
	}
	
	delete []NClass;
	delete []Nincome;
	delete []NFavor;	
		
	NClass = 0;
	Nincome = 0;
	NFavor = 0;	
}
void LOOK_Profit(){						//list our profit
		
	ifstream fin3;
	fin3.open("Profit.txt");
	if (fin3.fail()) {
		cout << "You haven't simulate !!!" 
			<< endl;
  		exit(1);
  	}


	string line;

	while ( getline(fin3, line) ) {
		cout << line << endl;
 	}

	fin3.close();
}

int main()
{
	char save;
	string option;
	srand(time(0));


		cout << "\n   +=============================================================+\n";
		cout << "   |                                                             |\n";
		cout << "   |                Welcome to MARKET PLAXX     !!!              |\n";
		cout << "   |                                                             |\n";
		cout << "   +=============================================================+\n\n\n";
		cout << "                           *** Menu ***" << endl << endl;
		cout << "[1] Start simulation" 	<< endl;
		cout << "[2] LOOK Grogecy  "  << endl;
		cout << "[3] LOOK Customer Base  "  << endl;
		cout << "[4] LOOK Profit "  << endl;
		cout << "[5] Credits" 		<< endl;
		cout << "[6] Exit" 			<< endl;
		cout << "*****************" << endl;
		cout << "Please enter Option (1-7): ";
		cin  >> option;
		
		while (option != "1" && option != "2" && option != "3" && option != "4" && option != "5" && option != "6")
		{	
			cout << "Please re-enter Option (1-6) again: ";
			cin  >> option;
		}
		
		if 		(option == "1")
		{
			system("clear"); MARKET_PLAXX();
			system("clear"); main();
		}
		else if 	(option == "2")
		{
			string Q;
			system("clear"); 
			
			do {
			LOOK_Grogecy();
			cout << "\nBACK TO MAIN? (Y/N)" ; 
			cin >> Q;} 
			while (Q != "Y") ; 
			system("clear");
			main();   
			
		}
		else if 	(option == "3")
		{
			string Q;
			system("clear"); 
			
			do {
			LOOK_Customer_Base();
			cout << "\nBACK TO MAIN? (Y/N)" ; 
			cin >> Q;} 
			while (Q != "Y") ; 
			system("clear");
			main();  
			
		}
		else if 	(option == "4")
		{
			string Q;
			system("clear"); 
			
			do {
			LOOK_Profit();
			cout << "\nBACK TO MAIN? (Y/N)" ; 
			cin >> Q;} 
			while (Q != "Y") ; 
			system("clear");
			main(); 
			
		}


		else if (option == "5")
		{	string Q;
			system("clear"); 
			do {
			cout << "NGO CHUN WING 3035553630\nAU CHUN MAN 3035558238\n\nGO BACK TO MAIN? (Y/N)" ; 
			cin >> Q;} 
			while (Q != "Y") ; 
			system("clear");
			main();
			
		}
		else if (option == "6")
		{
				system("clear");
				string Q;
				cout << "Do you want to quit MARKET PLAXX? (Y/N)" << endl;
				cin >> Q;
			
				
				if (Q ==  "Y") 
				{
					system("clear");
					cout << "Thankyou" << endl;
					cout << "Program terminates. Good bye!" << endl;
					return 0;
				}
				else if (Q ==  "N") {
					system("clear");
					return main();
				}
		}

	return 0;
}
