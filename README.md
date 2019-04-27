# Cash register system

Cash register system
Setting: We are the IT department of a chain grocery store company and we are going to design an advanced cash register system for our stores.

Problem Statement:

Vision: Our system aims to optimize the efficiency in the process of settling bills of grocery stores in terms of reducing the waiting time as well as increasing the profitability.

Issue Statement: According to the customers' opinion, the original cashier system cannot afford the increasing customer flow. The original cashier system is up to our sales assistant's visual observation to decide, there are rules or guidelines of our company to handle the quening problem. But the sales assistant usually cannot follow as they hard to decide by their owns. Therefore the customers usually need to use more than 15 minutes to make a settlement and sometimes they will leave without buying anything becasue of the long waiting time. Therefore, we need to change our cash register system to be decide by the program. Which our employees can follow it easily and avoid the excessive waiting time and customer loss.

Method: We will use a C++ program as the main body of our system. It is able to read from grocery file including information such as the grocery's type, Id, name, amount and its price; Also, it is able to calculate the processing time of one customer spent in the cashier by knowing what they buy, so it can process the whole line's waiting time and decide whether to open a new cashier to use under the situation. Also, the system is able to sort customers into the cashier that just finished the transaction if more than one cashier is opened. So the customer can reduce waiting time and our employees can follow the decision made by the system. We also want to add a express line for grocery amount under 5, so this line can afford more customers and have a faster transaction time. But if the express line is too long, the customer can still go to normal line if it is faster than the express line. With reading the customer's grocery, we can calculate the revenue, and the cost by registing new cashier by marking how many times they are registed, Therefore we can know the profit and hope to maximize it by controlling how many cashier to be registed while solving the quenuing problem and recovering the loyalty of our customers.

Assumption:

After going to a grocery store, we observe a average time of customers joining the queue, it is about 13 seconds and we decide to assume each round process is 13 seconds.
We assume there is 8 types of customers and generate the basket to them due to their class, income, favor.
We assume there is 36 sample product for us to test the system.
Due to the size of the grocery store, we only have 5 normal cashiers and 1 express cashiers.
Also a queue with 3 rows and 5 column will be the normal queue, and an express queue with 7 space next to it.
We assume the grocery_basket can only afford 100 products.
Assume the cashier 1 and express cashier will open for whole day.
Functionalities and Features:

Read our store grocery list and customer list.
Simulate our store environment as print the queue and cashier.
To generate random customer with random grocery according to the information in the customer list.
Append these random customer into the queue according to the amount of grocery in their beasket.
Control the cashiers to open when the customers exceed a specific amount.
Close the cashies when the customers below a specific amount.
Check whether the program would cause a long waiting time, if ys, simulate the customer will leave.
Calculate the total time, customers, cost to analysis our Performance by outputing data to a txt file for recording.
Input/Output specifications:

In the MENU, we only expect the user input the number 1 to 6 to choose what they want to do.
In the choice 1. Market PLAXX Simulation, we expect the user input "Y" to continue the simulation and "N" to quit the simulation.
Same input "Y" to continue and "N" to quit in other choices.
Most important, please download the both "grocery.txt" and "customer.txt" as they are the input file for the program!
Other than displaying the cashier, queue, customers and these information on the screen, a output txt file would be created after you ENTER "N" in choice 1. Market PLAXX Simulation. And once you enter again the simulation, it would be reset.
Compilation and Execution Instructions:

Please put the txt files and the cpp file together when compiling and executing the cpp file.
The cpp file is a full version program, no header file is created.
