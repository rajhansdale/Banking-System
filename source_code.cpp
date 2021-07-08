#include<iostream>
#include<fstream>
#include<cstdlib>
#include<vector>
#include<exception>
#include<map>

using namespace std;
#define MIN_BALANCE 500

class InsufficientFunds : public exception {
    public:
    const char* what() const throw() {
        return "\tCannot Process the Withdraw!! Insufficient Funds!!";
    }
};

class Account
{
private:
	long accountNumber;
	string firstName;
	string lastName;
	float balance;
	static long NextAccountNumber;
public:
	Account() {}
	Account(string fname, string lname, float balance);
	long getAccNo() { return accountNumber; }
	string getFirstName() { return firstName; }
	string getLastName() { return lastName; }
	float getBalance() { return balance; }

	void Deposit(float amount);
	void Withdraw(float amount);
	static void setLastAccountNumber(long accountNumber);
	static long getLastAccountNumber();
	friend ofstream& operator<<(ofstream& ofs, Account& acc);
	friend ifstream& operator>>(ifstream& ifs, Account& acc);
	friend ostream& operator<<(ostream& os, Account& acc);
};

long Account::NextAccountNumber = 0;

class Bank
{
private:
	map<long, Account> accounts;
public:
	Bank();
	Account OpenAccount(string fname, string lname, float balance);
	void BalanceEnquiry(long accountNumber);
	void Deposit(long accountNumber);
	void Withdraw(long accountNumber);
	void CloseAccount(long accountNumber);
	void ShowAllAccounts();
	~Bank();
};

Account::Account(string fname, string lname, float balance)
{
	NextAccountNumber++;
	accountNumber = NextAccountNumber;
	firstName = fname;
	lastName = lname;
	this->balance = balance;
}

void Account::Deposit(float amount)
{
	balance += amount;
}

void Account::Withdraw(float amount)
{
	if (balance - amount < MIN_BALANCE)
		throw InsufficientFunds();
	balance -= amount;
}

void Account::setLastAccountNumber(long accountNumber)
{
	NextAccountNumber = accountNumber;
}

long Account::getLastAccountNumber()
{
	return NextAccountNumber;
}

ofstream& operator<<(ofstream& ofs, Account& acc)
{
	ofs << acc.accountNumber << endl;
	ofs << acc.firstName << endl;
	ofs << acc.lastName << endl;
	ofs << acc.balance << endl;
	return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc)
{
	ifs >> acc.accountNumber;
	ifs >> acc.firstName;
	ifs >> acc.lastName;
	ifs >> acc.balance;
	return ifs;

}

ostream& operator<<(ostream& os, Account& acc)
{
	os << "\tFirst Name : " << acc.getFirstName() << endl;
	os << "\tLast Name : " << acc.getLastName() << endl;
	os << "\tAccount Number : " << acc.getAccNo() << endl;
	os << "\tBalance : " << acc.getBalance() << endl;
	return os;
}

Bank::Bank()
{
	Account account;
	ifstream infile;
	infile.open("Bank.data");
	if (!infile)
	{
		cout<<"Error in Opening! File Not Found!!"<<endl;
		return;
	}
	while (!infile.eof())
	{
		infile >> account;
		accounts.insert(pair<long, Account>(account.getAccNo(), account));
	}
	Account::setLastAccountNumber(account.getAccNo());

	infile.close();
}

Account Bank::OpenAccount(string fname, string lname, float balance)
{
	Account account(fname, lname, balance);
	accounts.insert(pair<long, Account>(account.getAccNo(), account));
	return account;
}

void Bank::BalanceEnquiry(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr != accounts.end())
	{
		cout << "\tAccount Number  : " << itr->second.getAccNo();
		cout << "\n\tFirst Name      : " << itr->second.getFirstName();
		cout << "\n\tLast Name       : " << itr->second.getLastName();
		cout << "\n\tAccount Balance : " << itr->second.getBalance();
	}
	else
		cout<<"\n\tNo account found !!";
}

void Bank::Deposit(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr != accounts.end())
	{
		float amount;
		cout << "\tEnter Amount : ";
		cin >> amount;
		itr->second.Deposit(amount);
		cout << "\n\tAccount Number  : " << itr->second.getAccNo();
		cout << "\n\tFirst Name      : " << itr->second.getFirstName();
		cout << "\n\tLast Name       : " << itr->second.getLastName();
		cout << "\n\tAccount Balance : " << itr->second.getBalance();
		cout << "\n\tAmount deposited !!";
	}
	else
		cout << "\n\tNo account found !!";
}

void Bank::Withdraw(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	if (itr != accounts.end())
	{
		float amount;
		cout << "\tEnter Amount : ";
		cin >> amount;
		try{
		itr->second.Withdraw(amount);
		cout << "\n\tAccount Number  : " << itr->second.getAccNo();
		cout << "\n\tFirst Name      : " << itr->second.getFirstName();
		cout << "\n\tLast Name       : " << itr->second.getLastName();
		cout << "\n\tAccount Balance : " << itr->second.getBalance();
		cout << "\n\tAmount withdrawn!!";
		}catch(InsufficientFunds e)
    {
        cout<<e.what()<<endl;
    }
	}
	else
		cout << "\n\tNo account found !!";
}

void Bank::CloseAccount(long accountNumber)
{
	map<long, Account>::iterator itr = accounts.find(accountNumber);
	cout << "\n\tACCOUNT DELETED SUCCESSFULLY !!" << itr->second;
	accounts.erase(accountNumber);
}

void Bank::ShowAllAccounts()
{
	map<long, Account>::iterator itr;
	for (itr = accounts.begin();itr != accounts.end();itr++)
	{
		cout << "Account " << itr->first << endl << itr->second << endl;
	}
}

Bank::~Bank()
{
	ofstream outfile;
	outfile.open("Bank.data", ios::trunc);

	map<long, Account>::iterator itr;
	for (itr = accounts.begin();itr != accounts.end();itr++)
	{
		outfile << itr->second;
	}
	outfile.close();
}

int main()
{
	Bank b;
	Account acc;

	int choice,flag=0;
	string fname, lname,pswd;
	long accountNumber;
	float balance;

	cout << "\tEnter password : ";
	cin >> pswd;

	do {
		if (pswd == "bank@0210")
		{
			flag = 1;
			do
			{
				cout << "\n\t\t\t\t-----------Banking System-----------" << endl;
				cout << "\n\t\t\t\t1. Open an Account";
				cout << "\n\t\t\t\t2. Balance Enquiry";
				cout << "\n\t\t\t\t3. Deposit";
				cout << "\n\t\t\t\t4. Withdrawal";
				cout << "\n\t\t\t\t5. Close an Account";
				cout << "\n\t\t\t\t6. Show All Accounts";
				cout << "\n\t\t\t\t7. Quit";
				cout << "\n\n\tEnter your choice : ";
				cin >> choice;
				cout << endl;
				switch (choice)
				{
				case 1:
					cout << "\tEnter First Name: ";
					cin >> fname;
					cout << "\tEnter Last Name: ";
					cin >> lname;
					cout << "\tEnter initial Balance: ";
					cin >> balance;
					acc = b.OpenAccount(fname, lname, balance);
					cout << endl << "\n\tCONGRATULATIONS !! YOUR ACCOUNT HAS BEEN CREATED SUCCESSFULLY." << endl;
					cout << acc;
					break;

				case 2:
					cout << "\tEnter Account Number:";
					cin >> accountNumber;
					b.BalanceEnquiry(accountNumber);
					break;

				case 3:
					cout << "\tEnter Account Number:";
					cin >> accountNumber;
					b.Deposit(accountNumber);
					break;

				case 4:
					cout << "\tEnter Account Number:";
					cin >> accountNumber;
					b.Withdraw(accountNumber);
					break;

				case 5:
					cout << "\tEnter Account Number:";
					cin >> accountNumber;
					b.CloseAccount(accountNumber);
					cout << endl << "\tAccount is Closed" << endl;
					cout << acc;

				case 6:
					b.ShowAllAccounts();
					break;

				case 7: b.~Bank(); exit(0);

				default:
					cout << "\n\tEnter correct choice !!";
					break;
				}
			} while (choice != 7);
		}

		else
		{
			cout << "\tIncorrect password !!\n\n\tPlease re-enter the password : ";
			cin >> pswd;
			flag = 1;
		}
	} while (flag != 0);

	return 0;
}

