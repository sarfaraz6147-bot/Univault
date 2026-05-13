//Muhammad Mobeen Ahmed             25i-0972            CS-A
//Sarfaraz Hussain                  25i-0921            CS-B
#include"pf_project.cpp"
using namespace std;
void readConfig(int& department_num, int& floor_num, int& room_num) 
{
    ifstream fin("config.txt");
    if (fin.is_open())
    {
        fin>>department_num>>floor_num>>room_num;
        fin.close();
        cout<<"Config loaded:"<<department_num<<" depts, "<<floor_num<<" floors, "<<room_num<<" rooms."<<endl;
    } 
    else 
    {
        cout<<"config.txt not found.Please enter values:"<<endl;
        cout<<"Number of departments:";  
        cin>>department_num;
        cout<<"Floors per department:";
        cin>>floor_num;
        cout<<"Rooms per floor:";  
        cin>>room_num;
        ofstream fout("config.txt");
        fout<<department_num<<"\n"<<floor_num<<"\n"<<room_num<<"\n";
        fout.close();
        cout<<"config.txt created for next time.\n";
    }
}
int*** initCampus(int department_num, int floor_num, int room_num) 
{
    //one pointer per department
    int*** grid=new int**[department_num];
    for(int dept=0;dept<department_num;dept++) 
    {
        //one pointer per floor
        *(grid+dept)=new int*[floor_num];
        for(int floor=0;floor<floor_num;floor++) 
        {
            //allocate room integers
            *(*(grid+dept)+floor)=new int[room_num];
            //set every room to 0
            for(int room=0;room<room_num;room++) 
            {
                *(*(*(grid+dept)+floor)+room)=0;
            }
        }
    }
    cout<<"Campus grid ready: "<<department_num<<" depts x "<<floor_num<<" floors x "<<room_num<<" rooms.\n";
return grid;
}
void freeCampus(int*** grid, int department_number, int floor_number) 
{
    for(int d=0;d<department_number;d++) 
    {
        for(int f=0;f<floor_number;f++) 
        {
            delete[] *(*(grid+d)+f);
        }
     delete[] *(grid+d);
    }
    delete[] grid;
}
//Module-2 
char** growNames(char** oldArray, int currentSize)
{
    char** newArray=new char*[currentSize+1];
    for(int i=0;i<currentSize;i++)
        *(newArray+i)=*(oldArray+i);
    delete[] oldArray;
    *(newArray+currentSize)=nullptr;
    return newArray;
}
int* growInt(int* oldArray, int currentSize)
{
    int* newArray=new int[currentSize+1];
    for(int i=0;i<currentSize;i++)
        *(newArray+i)=*(oldArray+i);
    delete[] oldArray;
    return newArray;
} 
float* growFloat(float* oldArray, int currentSize)
{
    float* newArray=new float[currentSize+1];
    for (int i=0;i<currentSize;i++)
        *(newArray+i)=*(oldArray+i);
    delete[] oldArray;
    return newArray;
}
void enrollStudent(int*** grid, char** &names, int* &ids, float* &gpas, int* &statuses, int &totalStudents, int totalDepts, int totalFloors, int totalRooms)
{
    char name[100];
    cout<<"\nEnter student name :"; 
    cin>>name;
    //Give student an ID
    int newID=1001+totalStudents; 
    //Grow all 4 arrays by 1
    names=growNames(names, totalStudents);
    ids=growInt(ids, totalStudents);
    gpas=growFloat(gpas, totalStudents);
    statuses=growInt(statuses, totalStudents);
    //Save name on heap
    *(names+totalStudents)=new char[strlen(name)+1];
    strcpy(*(names+totalStudents), name);
    //Save other info
    *(ids+totalStudents)=newID;
    *(gpas+totalStudents) = 2.50f;
    *(statuses+totalStudents)=0; 
    //Pick a random room
    int dept=rand()%totalDepts;
    int floor=rand()%totalFloors;
    int room=rand()%totalRooms;
    int skipped=0;
    bool found=false; 
    //Check if random room is empty
    if(*(*(*(grid+dept)+floor)+room)==0)
    {
        found=true;
    }
    else
    {
        //Room taken, search for next empty room
        for(int d=0;d<totalDepts && !found;d++)
        for(int f=0;f<totalFloors && !found;f++)
        for(int r=0;r<totalRooms && !found;r++)
        {
            skipped++;
            if (*(*(*(grid+d)+f)+r)==0)
            {
                dept=d;
                floor=f;
                room=r;
                found=true;
            }
        }
    }
    //Campus full check
    if(!found)
    {
        cout << "Campus is full!\n";
        delete[] *(names+totalStudents);
        return;
    } 
    //Assign room to student
    *(*(*(grid+dept)+floor)+room)=newID;
    //Count student
    totalStudents++; 
    //Show result
    cout<<"Enrolled! Name: "<<name<<" | ID: "<<newID<<" | Dept: "<<dept<< " Floor: "<<floor<<" Room: "<<room<<"\n"; 
    if(skipped > 0)
        cout << "Skipped " << skipped << " occupied room(s).\n";
} 
//Module-3
void runSimulation(int*** grid, char**& names, int*& ids, float*& gpas, int*& statuses, int& numStudents, int nd, int nf, int nr)
{
    for(int i=0;i<numStudents;i++)
    {
        unsigned char flags=0;
        int event=rand()%10+1;
        if(event>=1 && event<=5)
        {   
            flags|=1;
            *(gpas+i)=*(gpas+i)+(0.1);
            if(*(gpas+i)>4.00)
            {
                *(gpas+i)=4.00;
            }
        }
        else if(event>=6 && event<=8)
        {

        }
        else if(event==9)
        {   
            flags |=2;
            *(statuses+i)=1;
            *(gpas+i)=*(gpas+i)-0.2;
            if(*(gpas+i)<0.00)
            {
                *(gpas+i)=0.00;
            }
        }
        else if(event==10)
        {   
            flags |=4;
            *(statuses+i)=2;
            for(int d=0;d<nd;d++)
            {
                for(int f=0;f<nf;f++)
                {
                    for(int r=0;r<nr;r++)
                    {   
                        if(*(*(*(grid+d)+f)+r)==*(ids+i))
                        {    
                            *(*(*(grid+d)+f)+r)=0;
                        }
                    }
                }
            }
            //free name memory
            delete[] *(names+i);
            for(int j=i;j<numStudents-1;j++)
            {
                *(names+j)=*(names+j+1);
                *(ids+j)=*(ids+j+1);
                *(gpas+j)=*(gpas+j+1);
                *(statuses+j)=*(statuses+j+1);
            }
            numStudents--;
                char** new_names=new char*[numStudents];
                int* new_ids=new int[numStudents];
                float* new_gpas=new float[numStudents];
                int* new_statuses=new int[numStudents];
                for(int k=0;k<numStudents;k++) 
                {
                    *(new_names+k)=*(names+k);
                    *(new_ids+k)=*(ids+k);
                    *(new_gpas+k)=*(gpas+k);
                    *(new_statuses+k)=*(statuses+k);
                }
        //Deleting old parallel arrays
                delete[] names; 
              delete[] ids; 
                delete[] gpas; 
                delete[] statuses;
        //Pointing to the new shrunken memory
                names=new_names; 
                ids=new_ids; 
                gpas=new_gpas; 
                statuses=new_statuses;
                if(flags & 4)
                {
                    cout<<"Graduated! Student removed from registry"<<endl;
                }
        i--; 
        }
    }
}
//MODULE-4
void showDashboard(int*** grid, char** names, int* ids, float* gpas, int* statuses, int totalStudents, int totalDepts, int totalFloors,int totalRooms,int* currentDept, int* currentFloor, int step)
{
    system("cls");

    cout<< "===========================================================\n";
    cout << "          UniVault . LIVE RESOURCE DASHBOARD\n";
    cout << "===========================================================\n";
   cout<<"[DEPT: "<<*currentDept<< "] [FLOOR: "<< *currentFloor<< "] [ROOMS: " << totalRooms
         << "] [STEP: "  << step << "]\n";
    cout << "-----------------------------------------------------------\n";
    cout << left<<setw(14)<<"ROOM ADDR"<<setw(8)<< "ID"<<setw(15) <<"NAME"<< setw(8)<<"GPA"<< "STATUS\n";
    cout << "-----------------------------------------------------------\n";

    for (int r=0;r<totalRooms;r++)
    {
 int occupantID = *(*(*(grid + *currentDept)+*currentFloor)+r);
 int*roomAddr= *(*(grid + *currentDept) + *currentFloor) + r;

        if(occupantID==0)
        {
 cout<<left<<setw(14)<< (void*)roomAddr<< setw(8)<< "----"<< setw(15) << "----------"<< setw(8)<< "0.00"<< "EMPTY\n";
        }
        else
        {
            // find which student has this ID
            int index= -1;
            for (int s = 0; s<totalStudents; s++)
            {
                if (*(ids + s)== occupantID)
                {
                    index = s;
                    break;
                }
            }
            char* label;
        if(*(statuses+index)== 0) 
        label=(char*)"STUDYING";
        else if(*(statuses+index)==1) 
        label=(char*)"WARNING";
        else 
        label=(char*)"GRADUATING";
cout<<left<<setw(14)<<(void*)roomAddr<< setw(8)<< *(ids+index)<<setw(15)<<*(names+index)<<setw(8)<<*(gpas+index)<<label<< "\n";
        }
    }
    cout<<"-----------------------------------------------------------\n";
    cout<<"COMMANDS: [E] Enroll | [T] Step | [J] Jump | [F] Find | [S] Save | [X] Exit\n";
    cout<<"===========================================================\n";
}
//Module-5 
void findStudent(int*** grid, char** names, int* ids, float* gpas, int* statuses, int numStudents, int nd, int nf, int nr)
{   
    int search_id;
    cout<<"Enter student ID to search: ";
    cin>>search_id;
    int found=-1;
    for(int i=0;i<numStudents;i++)
    {
        if(*(ids+i)==search_id)
        {
            found=i;
            break;
        }
    }
    if(found==-1)
    {
        cout<<"No record found!"<<endl;
        return;
    }
    cout<<"\n--- Student Found ---"<<endl;
    cout<<"ID:"<<*(ids+found)<<endl;
    cout<<"Name:"<<*(names+found)<<endl;
    cout<<"GPA:"<<*(gpas+found)<<endl;
    cout<<"Status:"<<*(statuses+found)<<endl;
    for(int d=0;d<nd;d++)
    {
        for(int f=0;f<nf;f++)
        {
            for(int r=0;r<nr;r++)
            {
                if(*(*(*(grid+d)+f)+r)==search_id)
                cout<<"Room: D"<<d<<" F"<<f<<" R"<<r;
            }
        }
    }
}
void jump_location(int* currentDept, int* currentFloor, int nd, int nf) 
{
    int temp_depart, temp_floor;
    cout<<"Enter Department index (0 to "<<nd - 1<< "): ";
    cin>>temp_depart;
    cout<<"Enter Floor index (0 to "<<nf-1<<"): ";
    cin>>temp_floor; 
    if(temp_depart>=0 && temp_depart<nd) 
    {
        *currentDept=temp_depart;
    } 
    else 
    {
        cout<<"Invalid Department index!"<<endl;
    }
    if(temp_floor>=0 && temp_floor<nf) 
    {
        *currentFloor=temp_floor;
    } 
    else 
    {
        cout<<"Invalid Floor index!"<<endl;
    }
}
// module -6
void saveData(int*** grid, char** names, int* ids, float* gpas, int* statuses,int totalStudents, int totalDepts, int totalFloors, int totalRooms)
{
    ofstream file("database.txt");
    file<<totalStudents<<"\n";
    for (int i=0;i<totalStudents;i++)
    {
        int d=0, f=0, r=0;
        for (int td=0;td<totalDepts;td++)
        for (int tf=0;tf<totalFloors;tf++)
        for (int tr=0;tr<totalRooms;tr++)
        {
            if(*(*(*(grid+td)+tf)+tr)==*(ids+i))
            {
                d=td;
                f=tf;
                r=tr;
            }
        }
        file<<*(ids+i)<<" "<<*(names+i)<<" "<<*(gpas+i)<<" "<<*(statuses+i)<<" "<<d<<" "<<f<<" "<<r<<"\n";
    }
    file.close();
    cout<<"Saved!\n";
}
void loadData(int*** grid, char** &names, int* &ids, float* &gpas, int* &statuses, int &totalStudents, int totalDepts, int totalFloors, int totalRooms)
{
    ifstream file("database.txt");
    // if no file found, start fresh
    if (!file.is_open())
    {
        cout<<"No save file found. Starting fresh.\n";
        return;
    }
    int total;
    file>>total;
    // if 0 students saved, nothing to load
    if(total==0)
    {
        file.close();
        return;
    }
    delete[] names;
    delete[] ids;
    delete[] gpas;
    delete[] statuses;
    //NOW create fresh arrays of the right size
    totalStudents=total;
    names=new char*[totalStudents];
    ids=new int[totalStudents];
    gpas=new float[totalStudents];
    statuses=new int[totalStudents];
    // read each student record
    for (int i=0;i<totalStudents;i++)
    {
        char tempName[100];
        int  id, status, d, f, r;
        float gpa;
        file>>id>>tempName>>gpa>>status>>d>>f>>r;
        // find name length manually
        int len=0;
        while (tempName[len] != '\0')
            len++;
        // allocate exact memory for name
        *(names+i)=new char[len+1];
        int j=0;
        while(tempName[j]!='\0')
        {
            *(*(names+i)+j)=tempName[j];
            j++;
        }
        *(*(names+i)+j)='\0';
        *(ids+i)=id;
        *(gpas+i)=gpa;
        *(statuses+i)=status;
        // Now Using pointer aritematic restore the students in grid
        *(*(*(grid+d)+f)+r)=id;
    }
    file.close();
    cout<<"Loaded! Students: "<<totalStudents<<"\n";
}
int main()
{   srand((unsigned)time(0));
    int department_num=0, floor_num=0, room_num=0;
    readConfig(department_num, floor_num, room_num);
    int*** grid=initCampus(department_num, floor_num, room_num);
    // Student arrays (start empty)
    char** names=new char*[0];
    int* ids=new int[0];
    float* gpas=new float[0];
    int* statuses=new int[0];
    int totalStudents=0;
    int step=0;
    int currentDept=0;
    int currentFloor=0;
    // Menu
    loadData(grid, names, ids, gpas, statuses, totalStudents, department_num, floor_num, room_num);
    showDashboard(grid, names, ids, gpas, statuses, totalStudents,department_num, floor_num, room_num,&currentDept, &currentFloor, step);
    char choice;
    while (true)
    {   
       
        cout<<"\n[E] Enroll [T] step [J] Jump [F] Find [S] Save [X] Exit: ";
        cin>>choice;
        if(choice=='E' || choice=='e')
        {
            enrollStudent(grid,names,ids,gpas,statuses,totalStudents,department_num,floor_num,room_num);
                cout<<"\nPress Enter to view dashboard...";
                cin.ignore();
                cin.get();
            showDashboard(grid, names, ids, gpas, statuses, totalStudents,department_num, floor_num, room_num,&currentDept, &currentFloor, step);
        }
        else if(choice=='S' || choice=='s')
        {
            saveData(grid, names, ids, gpas, statuses, totalStudents, department_num, floor_num, room_num);
        }
        else if(choice=='T' || choice=='t')
        {
            runSimulation(grid, names, ids, gpas, statuses, totalStudents, department_num, floor_num, room_num);
            step++;
            showDashboard(grid, names, ids, gpas, statuses, totalStudents,department_num, floor_num, room_num,&currentDept, &currentFloor, step);
        }
        else if(choice=='P' || choice=='p')
        {
            for (int i=0;i<totalStudents;i++)
            {
                cout<<"ID: "<<*(ids+i)<<" | Name: "<<*(names+i)<<" | GPA: "<<*(gpas+i)<<" | Status: "<<*(statuses+i)<<"\n";
            }
            cout<<"Total: "<<totalStudents<<"\n";
        }
        else if(choice=='F' || choice=='f') 
        {
            findStudent(grid, names, ids, gpas, statuses, totalStudents, department_num, floor_num, room_num);
        } 
        else if(choice=='J' || choice=='j') 
        {
            jump_location(&currentDept, &currentFloor, department_num, floor_num);
            showDashboard(grid, names, ids, gpas, statuses, totalStudents,department_num, floor_num, room_num,&currentDept, &currentFloor, step);

        }
        else if(choice=='X' || choice=='x')
        {   
            saveData(grid, names, ids, gpas, statuses, totalStudents, department_num, floor_num, room_num);
            cout<<"Goodbye!\n";
            break;
        }
    }
    // Cleanup
    for(int i=0;i<totalStudents;i++) 
    delete[] *(names+i);
    delete[] names;
    delete[] ids;
    delete[] gpas;
    delete[] statuses; 
    for(int d=0; d<department_num;d++)
    {
        for(int f=0;f<floor_num;f++)
            delete[] *(*(grid+d)+f);
        delete[] *(grid+d);
    }
    delete[] grid;
return 0;
}