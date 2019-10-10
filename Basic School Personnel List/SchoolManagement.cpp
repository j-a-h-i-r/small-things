#include<iostream>
#include<fstream>
#include<iomanip>   // setw()
#include<cstdlib>   // system("cls")
#include<string>
using namespace std;

const int nmWd = 20, clsWd = 3, rlWd = 4;   // Width of Data

int title_menu();
int select_menu(int x);
int lookup_menu(int x);
void display_header(int x);
void wait();
bool is_num(string x);

int choice;

class Info
{
protected:
    string name,cls;
public:
    void in() {
        cin.ignore(100, '\n');
        cout<<">> Enter Name :: "; getline(cin, name);
        cout<<"\n>> Enter Class :: "; cin>>cls;
    }
    virtual void display() = 0;     // making it an abstract class
    virtual void add() = 0;
    virtual void lookup() = 0;
    void call(int call){
        if(call==1) add();
        else if(call==2) display();
        else if(call==3) lookup();
        else if(call==0) choice = title_menu();
        else return;
    }
};
class Teacher : public Info
{
public:
    void in() { Info::in(); }
    void out(){
        cout<<setw(19)<<""<<setw(nmWd)<<left<<name<<setw(12)<<""<<setw(clsWd)<<right<<cls<<endl<<endl;
    }
    void add(){
        Teacher::in();
        if(!is_num(cls)) {cout<<"\n"<<setw(49)<<"!! Invalid Entry !!\n\n"; wait(); return;}

        ofstream tf("teacher.txt", ios_base::app);  // open file in Append Mode
        tf<<name<<endl;
        tf<<cls<<endl;
        tf.close();
    }
    void display(){
        display_header(1);
        ifstream tr("teacher.txt");
        while( getline(tr, name)){
            getline(tr, cls);
            Teacher::out();
        }
        tr.close();
        wait();
    }
    void lookup(){
        int l_t = lookup_menu(1);
        switch(l_t){
            case 1 : cout<<">> Enter Name :: "; break;
            case 2 : cout<<">> Enter Class :: "; break;  }
        string temp;
        if(l_t==1 || l_t==2) {
        cin.ignore(100, '\n');
        getline(cin, temp); }
        if(l_t == 1) {          // search  by name
            display_header(1);
            ifstream look("teacher.txt");

            while(getline(look, name)){
                getline(look, cls);
                if(name == temp) Teacher::out();
            }
            look.close();
            wait();
        }
        else if(l_t == 2){  // search by class
            if(!is_num(temp)) {cout<<"\n"<<setw(49)<<"!! Invalid Entry !!\n"; wait(); return;}
            display_header(1);
            ifstream look("teacher.txt");

            while(getline(look, name)){
                getline(look, cls);
                if(cls == temp) Teacher::out();
            }
            look.close();
            wait();
        }
        else if(l_t == 0) { return; }
    }
};

class Student : public Info
{
    string roll;
public:
    void in(){
        Info::in();
        cout<<"\n>> Enter Roll :: "; cin>>roll;
    }
    void out(){
        cout<<setw(15)<<""<<setw(nmWd)<<left<<name<<setw(10)<<""<<setw(clsWd)<<right<<cls<<setw(6)<<""<<setw(rlWd)<<roll<<endl<<endl;
    }
    void add( ){
        Student::in();
        if(!is_num(cls) || !is_num(roll)) {cout<<"\n"<<setw(49)<<"!! Invalid Entry !!\n"; wait(); return;}

        ofstream sf("student.txt", ios_base::app);
        sf<<name<<endl;
        sf<<cls<<endl;
        sf<<roll<<endl;
        sf.close();
    }
    void display(){
        display_header(2);
        ifstream sr("student.txt");

        string temp;

        while(getline(sr, name)){
            getline(sr, cls);
            getline(sr, roll);
            Student::out();
        }
        sr.close();
        wait();
    }
    void lookup(){
        int l_s = lookup_menu(2);
        switch(l_s){
            case 1 : cout<<">> Enter Name :: "; break;
            case 2 : cout<<">> Enter Class :: "; break;
            case 3 : cout<<">> Enter Roll :: "; break;
        }
        string temp;
        if(l_s==1 || l_s==2 || l_s==3){
            cin.ignore(100, '\n');
            getline(cin, temp);
        }
        if(l_s == 1) {              // search  by name
            display_header(2);
            ifstream look("student.txt");

            while(getline(look, name)){
                getline(look, cls);
                getline(look, roll);
                if(name == temp) Student::out();
            }
            look.close();
            wait();
        }
        else if(l_s == 2){          // search by class
            if(!is_num(temp)) {cout<<"\n"<<setw(49)<<"!! Invalid Entry !!\n"; wait(); return;}
            display_header(2);
            ifstream look("student.txt");

            while(getline(look, name)){
                getline(look, cls);
                getline(look, roll);
                if(cls == temp) Student::out();
            }
            look.close();
            wait();
        }
        else if(l_s == 3) {
            if(!is_num(temp)) {cout<<"\n"<<setw(49)<<"!! Invalid Entry !!\n"; wait(); return;}
            display_header(2);
            ifstream look("student.txt");

            while(getline(look, name)){
                getline(look, cls);
                getline(look, roll);
                if(roll == temp) Student::out();
            }
            look.close();
            wait();
        }
        else if(l_s == 0) { return;}
    }
};

int main()
{
    Teacher teacher;
    Student student;
    int selection;
    choice = title_menu();          // show title Screen

    while(choice != 0){
        if(choice == 1) {           // teacher menu
            selection = select_menu(1);
            teacher.call(selection);
            continue;
        }
        else if (choice == 2){      // student menu
            selection = select_menu(2);
            student.call(selection);
            continue;
        }
        else choice = title_menu();
    }
    return 0;
}

            /***  Function Definition   ***/

int title_menu(){
    system("cls");

    cout<<setw(57)<<"Press Corresponding Number To Select"<<endl;
    cout<<setw(57)<<"------------------------------------"<<endl;
    cout<<setw(47)<<"1 :: Teacher Menu"<<endl;
    cout<<setw(47)<<"2 :: Student Menu"<<endl<<endl;
    cout<<setw(43)<<"0 :: Exit"<<endl<<endl;

    int i;
    cout<<setw(38)<<" ";
    cin>>i;
    return i;
}
int select_menu(int i){
    system("cls");

    if(i==1) {
        cout<<setw(50)<<"Select Menu :: Teacher"<<endl;
        cout<<setw(50)<<"----------------------"<<endl;
    }
    else if(i==2){
        cout<<setw(50)<<"Select Menu :: Student"<<endl;
        cout<<setw(50)<<"----------------------"<<endl;
    }
    cout<<setw(45)<<"1 :: Add Info"<<endl;
    cout<<setw(45)<<"2 :: View All"<<endl;
    cout<<setw(47)<<"3 :: Search Info"<<endl<<endl;
    cout<<setw(44)<<"0 :: Back"<<endl<<endl;

    int select;
    cout<<setw(39)<<" ";
    cin>>select;
    return select;
}
int lookup_menu(int i){
    system("cls");

    if(i==1) cout<<setw(48)<<"Search :: Teacher"<<endl;
    else if(i==2) cout<<setw(48)<<"Search :: Student"<<endl;
    cout<<setw(49)<<"-------------------"<<endl;
    cout<<setw(48)<<"1 :: Search By Name"<<endl;
    cout<<setw(49)<<"2 :: Search By Class"<<endl;
    if(i == 2) cout<<setw(48)<<"3 :: Search By Roll"<<endl<<endl;
    else if(i == 1) cout<<endl;
    cout<<setw(44)<<"0 :: Back"<<endl<<endl;

    int s;
    cout<<setw(39)<<" ";
    cin>>s;
    return s;
}
void display_header(int i){
    system("cls");
    if(i==1){
        cout<<setw(52)<<":: Teacher Information ::"<<endl<<endl;
        cout<<setw(30)<<"Name"<<setw(25)<<"Class"<<endl;
        cout<<setw(18)<<""<<string(nmWd,'-')<<setw(18)<<string(7,'-')<<endl;
    }
    if(i==2){
        cout<<setw(52)<<":: Student Information ::"<<endl<<endl;
        cout<<setw(26)<<"Name"<<setw(23)<<"Class"<<setw(10)<<"Roll"<<endl;
        cout<<setw(14)<<""<<string(nmWd,'-')<<setw(16)<<string(7,'-')<<setw(10)<<string(6,'-')<<endl;
    }
}
void wait(){
    cout<<"\n>> Press ENTER To Continue ::  ";
    cin.sync();
    cin.ignore(100, '\n');
}
bool is_num(string s){
    for (int i=0; i<s.length(); i++) if(!isdigit(s[i])) return false;
    return !s.empty() && true;
}
