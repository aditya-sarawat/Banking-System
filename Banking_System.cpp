#include<iostream>
#include<fstream>
#include<cctype>
#include<iomanip>

using namespace std;


class account
{
    int acc_no;

    char name[50];

    int deposit;

    char type;

public:
    void create_account();	    //function to get data from user
    void show_account() const;	//function to show data on screen
    void modify();	            //function to add new data
    void dep(int);	            //function to accept amount and add to balance amount
    void draw(int);	            //function to accept amount and subtract from balance amount
    void report() const;	    //function to show data in tabular format
    int retacno() const;	    //function to return account number
    int retdeposit() const;     //function to return balance amount
    char rettype() const;	    //function to return type of account
};

void account::create_account()
{
    cout<<"\nEnter The account No. :";
    cin >> acc_no;
    cout<<"\nEnter The Name of The account Holder : ";
    cin.ignore();
    cin.getline(name,50);
    cout<<"\nEnter Type of The account (C/S) : ";
    cin>>type;
    type=toupper(type);
    cout<<"\nEnter The Initial amount(>=500 for Saving and >=1000 for current ) : ";
    cin>>deposit;
    cout<<"\n\nAccount Created...";
}

void account::show_account() const
{
    cout << "\nAccount No. : " << acc_no;
    cout<<"\nAccount Holder Name : ";
    cout<<name;
    cout<<"\nType of Account : "<<type;
    cout<<"\nBalance amount : "<<deposit;
}


void account::modify()
{
    cout << "\nAccount No. : " << acc_no;
    cout<<"\nModify Account Holder Name : ";
    cin.ignore();
    cin.getline(name,50);
    cout<<"\nModify Type of Account : ";
    cin>>type;
    type=toupper(type);
    cout<<"\nModify Balance amount : ";
    cin>>deposit;
}


void account::dep(int x)
{
    deposit+=x;
}

void account::draw(int x)
{
    deposit-=x;
}

void account::report() const
{
    const char separator    = ' ';
    const int nameWidth     = 15;

    cout << left << setw(nameWidth) << setfill(separator) << acc_no;
    cout << left << setw(nameWidth) << setfill(separator) << name;
    cout << left << setw(nameWidth) << setfill(separator) << type;
    cout << left << setw(nameWidth) << setfill(separator) << deposit << endl;
}


int account::retacno() const
{
    return acc_no;
}

int account::retdeposit() const
{
    return deposit;
}

char account::rettype() const
{
    return type;
}

void write_account();	            //function to write record in binary file
void display_sp(int);	            //function to display account details given by user
void modify_account(int);	        //function to modify record of file
void delete_account(int);	        //function to delete record of file
void display_all();		            //function to display all account details
void deposit_withdraw(int, int);    // function to deposit/withdraw amount for given account
void intro();	                    //introductory screen function



int main()
{
    char ch;
    int num;
    intro();
    do
    {
        cout<<"\n\nMAIN MENU";
        cout<<"\n01. NEW ACCOUNT";
        cout<<"\n02. DEPOSIT AMOUNT";
        cout<<"\n03. WITHDRAW AMOUNT";
        cout<<"\n04. BALANCE ENQUIRY";
        cout<<"\n05. ALL ACCOUNT HOLDER LIST";
        cout<<"\n06. CLOSE AN ACCOUNT";
        cout<<"\n07. MODIFY AN ACCOUNT";
        cout<<"\n08. EXIT";
        cout<<"\nSelect Your Option (1-8) ";
        cin>>ch;

        switch(ch)
        {
            case '1':
                write_account();
                break;
            case '2':
                cout<<"\nEnter The account No. : "; cin>>num;
                deposit_withdraw(num, 1);
                break;
            case '3':
                cout<<"\nEnter The account No. : "; cin>>num;
                deposit_withdraw(num, 2);
                break;
            case '4':
                cout<<"\nEnter The account No. : "; cin>>num;
                display_sp(num);
                break;
            case '5':
                display_all();
                break;
            case '6':
                cout<<"\nEnter The account No. : "; cin>>num;
                delete_account(num);
                break;
            case '7':
                cout<<"\nEnter The account No. : "; cin>>num;
                modify_account(num);
                break;
            case '8':
                cout<<"\nThanks for using bank management system";
                break;
            default :cout<<"\a";
        }

        cin.ignore();
        cin.get();
    }while(ch!='8');

    return 0;
}



void write_account()
{
    account ac{};
    ofstream outFile;
    outFile.open("account.dat",ios::binary|ios::app);
    ac.create_account();
    outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
    outFile.close();
}


void display_sp(int n)
{
    account ac{};
    bool flag=false;
    ifstream inFile;
    inFile.open("account.dat",ios::binary);
    if(!inFile)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    cout<<"\nBALANCE DETAILS\n";

    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()==n)
        {
            ac.show_account();
            flag=true;
        }
    }
    inFile.close();
    if(!flag)
        cout<<"\nAccount number does not exist!";
}



void modify_account(int n)
{
    bool found=false;
    account ac{};
    fstream File;
    File.open("account.dat",ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    while(!File.eof() && !found)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            ac.show_account();
            cout<<"\n\nEnter The New Details of account"<<endl;
            ac.modify();
            int pos=(-1)*static_cast<int>(sizeof(account));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
            cout<<"\nRecord Updated";
            found=true;
        }
    }
    File.close();
    if(!found)
        cout<<"\n\n Record Not Found ";
}


void delete_account(int n)
{
    account ac{};
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.dat",ios::binary);
    if(!inFile)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    outFile.open("Temp.dat",ios::binary);
    inFile.seekg(0,ios::beg);
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        if(ac.retacno()!=n)
        {
            outFile.write(reinterpret_cast<char *> (&ac), sizeof(account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.dat");
    rename("Temp.dat","account.dat");
    cout<<"\nRecord Deleted ..";
}


void display_all()
{
    account ac{};
    ifstream inFile;
    inFile.open("account.dat",ios::binary);
    if(!inFile)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    cout<<"\nACCOUNT HOLDER LIST\n";
    cout<<"----------------------------------------------------\n";
    cout<< left << setw(15) <<"A/c no." << setw(15) << "NAME" << setw(15) << "Type" << setw(15) << "Balance\n";
    cout<<"----------------------------------------------------\n";
    while(inFile.read(reinterpret_cast<char *> (&ac), sizeof(account)))
    {
        ac.report();
    }
    inFile.close();
}


void deposit_withdraw(int n, int option)
{
    int amt;
    bool found=false;
    account ac{};
    fstream File;
    File.open("account.dat", ios::binary|ios::in|ios::out);
    if(!File)
    {
        cout<<"File could not be open !! Press any Key...";
        return;
    }
    while(!File.eof() && !found)
    {
        File.read(reinterpret_cast<char *> (&ac), sizeof(account));
        if(ac.retacno()==n)
        {
            ac.show_account();
            if(option==1)
            {
                cout<<"\n\n\tTO DEPOSIT AMOUNT ";
                cout<<"\n\nEnter The amount to be deposited";
                cin>>amt;
                ac.dep(amt);
            }
            if(option==2)
            {
                cout<<"\n\n\tTO WITHDRAW AMOUNT ";
                cout<<"\n\nEnter The amount to be withdraw";
                cin>>amt;
                int bal=ac.retdeposit()-amt;
                if((bal<500 && ac.rettype()=='S') || (bal<1000 && ac.rettype()=='C'))
                    cout<<"Insufficiency balance";
                else
                    ac.draw(amt);
            }
            int pos=(-1)*static_cast<int>(sizeof(ac));
            File.seekp(pos,ios::cur);
            File.write(reinterpret_cast<char *> (&ac), sizeof(account));
            cout<<"\n\n\t Record Updated";
            found=true;
        }
    }
    File.close();
    if(!found)
        cout<<"\nRecord Not Found ";
}


void intro()
{
    cout<<"\n\n            BANK Management System                \n";
    cout<<"\n           MADE BY : Aditya Sarawat               \n\n";
    cout<<"Enter any key to start.\n> ";
    cin.get();
}