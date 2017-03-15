#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <fstream>
#include <conio.h>
#include <string>
#include <vector>
#include <ctime>
#include <time.h>
#include <algorithm>
#include <sstream>

#define getch() _getch()
using namespace std;

struct Item
{
	string Id;
	string Name;
	int Price;
	string Time;
	time_t DateTime;
};
vector<Item> add_item(string dbName);
vector<Item> delete_item_from_file(vector<Item> items, string dbName);
vector<Item> read_from_file(string dbName);
vector<Item> add_to_bucket(vector<Item> items);
vector<Item> remove_from_bucket();
vector<Item> buckets;
void write_to_file(vector<Item> items, string dbName);
void show_screen();
void show_management();
void show_billing();
void show_history();
void history_day();
void history_week();
void history_month();

string db_management = "db_management.txt";
string db_billing = "db_billing.txt";
string db_history = "db_history.txt";


void create_database(string dbName){
	fstream  fs;
	fs.open(dbName, fstream::in | fstream::out);
	if (fs.is_open())
	{
		cout << "Database is existed!!!";
	}
	else{
		string createFlg;
		cout << "Database "+ dbName +"is not exist, do you want to create a new database? (y/n) \t";
		cin >> createFlg;
		if (createFlg == "Y" || createFlg == "y"){
			fs.clear();
			fs.open(dbName, fstream::out);
		}
	}
	fs.close();
}
void ask_exit(string c){
asking:
	string choice;
	cout << "Are you sure you want to exit <y/n>? :   ";
	cin >> choice;
	if (choice == "y")
	{
		if (c == "HOME"){
			exit(0);
		}
		else{
			show_screen();
		}

	}
	else if (choice == "n"){
		if (c == "Management"){
			show_management();
		}
		else if (c == "Billing"){
			show_billing();
		}
		else if (c == "History"){
			show_history();
		}
		else{
			show_screen();
		}
	}

	else{
		goto asking;
	}
}

void show_management(){
management:
	int choice;
	system("cls");
	cout << "SUPERMARKET BILLING SYSTEM\n";
	cout << "A. Management" << endl;
	cout << "\t1.Add new item" << endl;
	cout << "\t2.Delete item" << endl;
	cout << "\t3.Exit" << endl;
	cout << "\tEnter your choice:";
	cin >> choice;
	switch (choice){
	case 1:
		write_to_file(add_item(db_management), db_management);
		goto management;
		break;
	case 2: delete_item_from_file(read_from_file(db_management), db_management);
		goto management;
		break;
	case 3: ask_exit("Management");
		break;
	default:
		goto management;
		break;
	}
}

void show_billing(){
	
billing:
	int choice;
	vector<Item> items;
	vector<Item> existed;
	fstream  fs;
	system("cls");
	cout << "SUPERMARKET BILLING SYSTEM\n";
	cout << "B. Billing" << endl;
	cout << "\t1.Add item to the bucket" << endl;
	cout << "\t2.Remove item in the bucket" << endl;
	cout << "\t3.Checkout" << endl;
	cout << "\t4.Exit" << endl;
	cout << "\tEnter your choice:";
	cin >> choice;
	switch (choice){
	case 1:
		items = read_from_file(db_management);
		buckets = add_to_bucket(items);
		existed = read_from_file(db_billing);
	 	existed.insert(existed.end(), buckets.begin(), buckets.end());
		write_to_file(existed, db_billing);
		cout << "Add successfully! Press any key to return...";
		getch();
		goto billing;
		break;
	case 2:
		buckets = read_from_file(db_billing);
		delete_item_from_file(buckets,  db_billing);
		getch();
		goto billing;
		break;
	case 3:
		existed = read_from_file(db_history);
	 	existed.insert(existed.end(), buckets.begin(), buckets.end());
		write_to_file(existed, db_history);
		fs.open(db_billing, fstream::out | std::ofstream::trunc);
		fs.close();
		cout << "Checked out! Press any key to return...";
		getch();
		goto billing;
		break;
	case 4: ask_exit("Billing");
		break;
	default:
		goto billing;
		break;
	}
}
void history_day(){
	vector<Item> items;
	items = read_from_file(db_history);
	system("cls");
	int bday, bmonth, byear;
	int aday, amonth, ayear;
	int TotalDay = 0;
	cout << "SHOW TOTAL INCOME OF EACH DAY..." << endl;
	for (int i = 0; i < items.size(); i++){
		if (sscanf(items.at(i).Time.c_str(), "%4d-%2d-%2d", &byear, &bmonth, &bday)) {
			TotalDay = items.at(i).Price;
			cout << to_string(byear) + "-" + to_string(bmonth) + "-" + to_string(bday) << endl;
			cout << "------------------------------------" << endl;
			cout << "\t" + items.at(i).Id + "\t" + items.at(i).Name + "\t" + to_string(items.at(i).Price) << endl;
			for (int j = i + 1; j < items.size(); j++){
				if (sscanf(items.at(j).Time.c_str(), "%4d-%2d-%2d", &ayear, &amonth, &aday)) {
					if (aday == bday && amonth == bmonth && ayear == byear){
						i = j;
						TotalDay += items.at(j).Price;
						cout << "\t" + items.at(j).Id + "\t" + items.at(j).Name + "\t" + to_string(items.at(j).Price) << endl;
					}
				}
			}
			cout << "\t\t\tTOTAL INCOME: " + to_string(TotalDay) << endl;
			cout << "------------------------------------" << endl;
		}
	}
}

void history_week(){
	vector<Item> items;
	int TotalWeek = 0;
	items = read_from_file(db_history);
	system("cls");
	cout << "SHOW TOTAL INCOME OF EACH WEEK..." << endl;
	for (int i = 0; i < items.size(); i++){
		int year1, month1, day1;
		char buffer1[4];
		struct tm * timeinfo1;
		timeinfo1 = localtime(&items.at(i).DateTime);
		strftime(buffer1, 4, "%W", timeinfo1);
		sscanf(items.at(i).Time.c_str(), "%4d-%2d-%2d", &year1, &month1, &day1);
		TotalWeek = items.at(i).Price;

		cout << "Year: " +to_string(year1) + " - " + "Week: "+buffer1 << endl;
		cout << "------------------------------------" << endl;
		cout << "\t" + items.at(i).Id + "\t" + items.at(i).Name + "\t" + to_string(items.at(i).Price) << endl;

		for (int j = i + 1; j < items.size(); j++){
			int year2, month2, day2;
			char buffer2[4];
			struct tm * timeinfo2;
			timeinfo2 = localtime(&items.at(j).DateTime);
			strftime(buffer2, 4, "%W", timeinfo2);
			sscanf(items.at(j).Time.c_str(), "%4d-%2d-%2d", &year2, &month2, &day2);
			if (year1 == year2 && strcmp(buffer1, buffer2)==0){
				i = j;
				TotalWeek += items.at(j).Price;
				cout << "\t" + items.at(j).Id + "\t" + items.at(j).Name + "\t" + to_string(items.at(j).Price) << endl;
			}
			else{
				break;
			}
		}

		cout << "\t\t\tTOTAL INCOME: " + to_string(TotalWeek) << endl;
		cout << "------------------------------------" << endl;
	}
	
}

void history_month(){
	vector<Item> items;
	items = read_from_file(db_history);
	system("cls");
	int bday, bmonth, byear;
	int aday, amonth, ayear;
	int TotalMonth = 0;
	cout << "SHOW TOTAL INCOME OF EACH MONTH..." << endl;
	for (int i = 0; i < items.size(); i++){
		if (sscanf(items.at(i).Time.c_str(), "%4d-%2d-%2d", &byear, &bmonth, &bday)) {
			TotalMonth = items.at(i).Price;
			cout << to_string(byear) + "-" + to_string(bmonth) << endl;
			cout << "------------------------------------" << endl;
			cout << "\t" + items.at(i).Id + "\t" + items.at(i).Name + "\t" + to_string(items.at(i).Price) << endl;
			for (int j = i + 1; j < items.size(); j++){
				if (sscanf(items.at(j).Time.c_str(), "%4d-%2d-%2d", &ayear, &amonth, &aday)) {
					if (amonth == bmonth && ayear == byear){
						i = j;
						TotalMonth += items.at(j).Price;
						cout << "\t" + items.at(j).Id + "\t" + items.at(j).Name + "\t" + to_string(items.at(j).Price) << endl;
					}
				}
			}
			cout << "\t\t\tTOTAL INCOME: " + to_string(TotalMonth) << endl;
			cout << "------------------------------------" << endl;

		}
	}
}
void show_history(){
history:
	int choice;
	system("cls");
	cout << "SUPERMARKET HISTORY SYSTEM\n";
	cout << "C. History" << endl;
	cout << "\t1.Show total income for each day" << endl;
	cout << "\t2.Show total income for each week" << endl;
	cout << "\t3.Show total income for each month" << endl;
	cout << "\t4.Exit" << endl;
	cout << "\tEnter your choice:";
	cin >> choice;
	switch (choice){
	case 1:
		history_day();
		getch();
		goto history;
		break;
	case 2:
		history_week();
		getch();
		goto history;
		break;
	case 3:
		history_month();
		getch();
		goto history;
		break;
	case 4: ask_exit("History");
		break;
	default:
		goto history;
		break;
	}
}
void show_screen(){
showScreen:
	char mainChoice;
	string choice;
	system("cls");
	cout << "SUPERMARKET BILLING SYSTEM\n";
	cout << "A. Management" << endl;
	cout << "B. Billing" << endl;;
	cout << "C. History" << endl;
	cout << "D. Exit" << endl;
	cout << "Enter your choice:";
	cin >> mainChoice;
	switch (mainChoice)
	{
	case 'A':
		show_management();
		break;
	case 'B':
		show_billing();
		break;
	case 'C':
		show_history();
		break;
	case 'D':
		ask_exit("HOME");
		break;
	default:
		goto showScreen;
		break;
	}

}
vector<Item> add_to_bucket(vector<Item> items){
	int i = -1;
	cout << "(Enter 0 to exit)\n";
	while (i!=0){
		cout << "Enter No. to add to bucket: ";
		cin >> i;
		if (i > 0 && i <= items.size()){
			buckets.push_back(items.at(i-1));
		}
	}
	int total=0;
	cout << "Your bucket:" << endl;
	for (int i = 0; i < buckets.size(); i++){
		cout << "\t" + buckets.at(i).Id + "\t" + buckets.at(i).Name + "\t" + to_string(buckets.at(i).Price) <<endl;
		total += buckets.at(i).Price;
	}
	cout << "\t\tTOTAL: " + to_string(total) << endl;
	return buckets;
}

vector<Item> remove_from_bucket(){
	cout << "Your bucket:" << endl;
	for (int i = 0; i < buckets.size(); i++){
		cout << "\t" + buckets.at(i).Id + "\t" + buckets.at(i).Name + "\t" + to_string(buckets.at(i).Price) << endl;
	}
	int i = -1;
	cout << "(Enter 0 to exit)\n";
	while (i != 0){
		cout << "Enter No. to remove: ";
		cin >> i;
		if (i > 0 && i <= buckets.size()){
			buckets.erase(buckets.begin()+i-1);
		}
	}
	cout << "Your bucket:" << endl;
	int total=0;
	for (int i = 0; i < buckets.size(); i++){
		cout << "\t" + buckets.at(i).Id + "\t" + buckets.at(i).Name + "\t" + to_string(buckets.at(i).Price) << endl;
		total += buckets.at(i).Price;
	}
	cout << "\t\tTOTAL: " + to_string(total) << endl;
	return buckets;

}
vector<Item> delete_item_from_file(vector<Item> items, string dbName){
	int i = -1;
	cout << "(Enter 0 to exit)\n";
	while (i < 0 || i > items.size()){
		cout << "Enter No. to delete/remove item: ";
		cin >> i;
	} 	
	if (i == 0){

	}
	else{
		fstream  fs;
		fs.open(dbName, fstream::out | std::ofstream::trunc);
		fs.close();
		items.erase(items.begin() + i-1);
		cout << "Delete successfully!" << endl;
		write_to_file(items, dbName);
	}
	return items;
}

vector<Item> read_from_file(string dbName){
	Item item;
	vector<Item> vItems;
	int year, month, day;
	fstream  fs;
	fs.open(dbName, fstream::in | fstream::out);
	cout << "List items:" << endl;
	cout << "\tNo.\t\t ID \t\t Name \t\t Price" << endl;
	cout << "-------------------------------------------------------------------" << endl;
	if (fs.is_open()){
		int count = 0;
		ifstream infile(dbName);	
		while (infile ){
			string price;
			getline(infile, item.Id);
			if (item.Id.empty()) break;
			getline(infile, item.Name);
			getline(infile, price); 
			item.Price = stoi(price.c_str());
			getline(infile, item.Time);
			if (sscanf(item.Time.c_str(), "%4d-%2d-%2d", &year, &month, &day)) {
				tm tmp = tm();
				tmp.tm_mday = day;
				tmp.tm_mon = month;
				tmp.tm_year = year - 1900;
				item.DateTime = mktime(&tmp);
			}
			count++;
			cout << "\t"+to_string(count) + " \t\t " + item.Id + " \t\t " + item.Name + " \t\t " + to_string(item.Price) + "\n";
			vItems.push_back(item);
		}
	}
	sort(vItems.begin(), vItems.end(), [](const Item& lhs, const Item& rhs)
	{
		return lhs.DateTime < rhs.DateTime;
	});
	return vItems;
}
void write_to_file(vector<Item> items, string dbName){
	writetofile:
	char buff[20];
	time_t t = time(NULL);
	strftime(buff, 20, "%Y-%m-%d", localtime(&t));
	fstream  fs;
	fs.open(dbName, fstream::in | fstream::out);
	if (fs.is_open()){
		for (int i = 0; i < items.size(); i++){
			fs << items[i].Id + "\n";
			fs << items[i].Name + "\n";
			fs << to_string(items[i].Price) + "\n";
			fs << string(buff) + "\n";
		}
	}
	else{
		create_database(dbName);
		goto writetofile;
	}
	fs.close();
	getch();
}
vector<Item> add_item(string dbName){
	vector<Item> existed = read_from_file(dbName);
	vector<Item> vItems;
	while (true){
		Item item;
		system("cls");
		cin.sync();
		cout << "\tPlease add new item (Enter 0 to exit)" << endl;
		cout << "Please add item ID		: "; getline(cin, item.Id);
		if (item.Id == "0") break;
		cout << "Please add item Name		: "; getline(cin, item.Name);
		cout << "Please add item Price 	: "; cin >> item.Price;
		cout << item.Id;
		getch();
		vItems.push_back(item);
	}; 
	existed.insert(existed.end(), vItems.begin(), vItems.end());
	cout << "Add Successfully!!! Press any key..." << endl;
	return existed;

}

int _tmain(int argc, _TCHAR* argv[])
{
	system("cls");
	show_screen();
	getch();

}
