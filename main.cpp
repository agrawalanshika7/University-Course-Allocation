
#include <bits/stdc++.h>
using namespace std;

map<int,vector<int>> master; //key - "new" course id; value - vector of profs
map<int,vector<int> > prof_assignment; //key is prof id; value - vector of course id //current allotments at each point of time
set<int> not_assign_pid; //set of profs who cannot be assigned that particular course due to a clash
map<int,double> prof_potential; // max courses can be taken by prof, first element prof id second potential
set<int> not_assign_cid;


//RECURSIVE SELF-CORRECTING ASSIGN FUNCTION
int assign(int cid,int flag) //flag=0: 0.5 course to be given;  flag=1: 1 full course to be given
{
    int firsthalf_profid = -1; //to know which prof to remove if only half a course was alloted

    if (flag==0)
    { 
        for (int  i = 0; i < master[cid].size(); i++)
        {
             int prof_id = master[cid][i]; // prof_id of prof who can potentially take the course 
            if(prof_potential[prof_id] >=0.5 && not_assign_pid.find(prof_id) == not_assign_pid.end())
            {
                prof_potential[prof_id] -=0.5;
                prof_assignment[prof_id].push_back(cid);
                // cout << prof_id << "   " << cid << endl;
                return 1; 
            }
        }
    }    
    if(flag == 1)
    {
        
        for (int  i = 0; i < master[cid].size(); i++)
        {
            
            int prof_id = master[cid][i]; // prof_id of prof who can potentially take the course 
            
            if(prof_potential[prof_id] >=0.5 && not_assign_pid.find(prof_id) == not_assign_pid.end())
            {
                prof_potential[prof_id] -=0.5;
                prof_assignment[prof_id].push_back(cid);
                // cout << prof_id << "   " << cid << endl;
                flag = 0;
                firsthalf_profid = prof_id;
                break;
            }
        }
        if(flag == 0) 
        {
            for (int  i = 0; i < master[cid].size(); i++)
            {
                int prof_id = master[cid][i]; // prof_id of prof who can potentially take the course 
                if(prof_potential[prof_id] >=0.5 && not_assign_pid.find(prof_id) == not_assign_pid.end())
                {
                    prof_potential[prof_id] -=0.5;
                    prof_assignment[prof_id].push_back(cid);
                    // cout << prof_id << "   " << cid << endl;
                    return 1;
                }
            }
        }
    }

    //  RECURSIVE CALL TO ASSIGN
    if(flag == 0)
    {
        not_assign_cid.insert(cid);
        for (int  i = 0; i < master[cid].size(); i++)
        {
            int prof_id = master[cid][i]; // prof_id of prof who can potentially take the course 

            if(not_assign_pid.find(prof_id) == not_assign_pid.end()) //does not go into recursion for already rec ursed profs in not_assign_pid
            {
                for (int  i = 0; i < prof_assignment[prof_id].size(); i++)
                {
                    if(not_assign_cid.find(prof_assignment[prof_id][i]) == not_assign_cid.end())
                    {
                        not_assign_pid.insert(prof_id);
                        int is_assigned = assign(prof_assignment[prof_id][i] ,0);
                        not_assign_pid.erase(not_assign_pid.find(prof_id));
                        if(is_assigned == 1)
                        {
                            prof_assignment[prof_id].push_back(cid);
                            // cout << prof_id << "   " << cid << endl;
                            prof_assignment[prof_id].erase(prof_assignment[prof_id].begin()+i);
                            // not_assign_cid.erase(not_assign_cid.find(cid));
                            return 1;
                        }
                        else
                        {
                            not_assign_cid.insert(prof_assignment[prof_id][i]);
                        }
                    }
                }
            }
            
        }
        
        //not_assign_cid.erase(not_assign_cid.find(cid));
        if(firsthalf_profid!=-1)
        {
            prof_assignment[firsthalf_profid].erase(prof_assignment[firsthalf_profid].end()-1);
        }
    }
    if(flag == 1)
    {
        double amount_course_assigned=0;
        

        not_assign_cid.insert(cid);
        for (int  i = 0; i < master[cid].size(); i++)
        {
            int prof_id = master[cid][i]; // prof_id of prof who can potentially take the course 

            if(not_assign_pid.find(prof_id) == not_assign_pid.end()) //does not go into recursion for already rec ursed profs in not_assign_pid
            {
                for (int  i = 0; i < prof_assignment[prof_id].size(); i++)
                {
                    if(not_assign_cid.find(prof_assignment[prof_id][i]) == not_assign_cid.end())
                    {
                        not_assign_pid.insert(prof_id);
                        int is_assigned = assign(prof_assignment[prof_id][i] ,0);
                        not_assign_pid.erase(not_assign_pid.find(prof_id));
                        if(is_assigned == 1)
                        {
                            firsthalf_profid=prof_id;
                            prof_assignment[prof_id].push_back(cid);
                            // cout << prof_id << "   " << cid << endl;
                            prof_assignment[prof_id].erase(prof_assignment[prof_id].begin()+i);
                            //not_assign_cid.erase(not_assign_cid.find(cid));  //ERASED RECENTLY
                            amount_course_assigned=0.5;
                            break;
                        }
                    }
                }
                if(amount_course_assigned==0.5) break;
            }
            
        }
        not_assign_cid.erase(not_assign_cid.find(cid));
        if(amount_course_assigned==0) return -1;
        else
        {
            not_assign_cid.insert(cid);
            for (int  i = 0; i < master[cid].size(); i++)
            {
                int prof_id = master[cid][i]; // prof_id of prof who can potentially take the course 

                if(not_assign_pid.find(prof_id) == not_assign_pid.end()) //does not go into recursion for already rec ursed profs in not_assign_pid
                {
                    for (int  i = 0; i < prof_assignment[prof_id].size(); i++)
                    {
                        if(not_assign_cid.find(prof_assignment[prof_id][i]) == not_assign_cid.end())
                        {
                            not_assign_pid.insert(prof_id);
                            int is_assigned = assign(prof_assignment[prof_id][i] ,0);
                            not_assign_pid.erase(not_assign_pid.find(prof_id));
                            if(is_assigned == 1)
                            {
                                prof_assignment[prof_id].push_back(cid);
                                // cout << prof_id << "   " << cid << endl;
                                prof_assignment[prof_id].erase(prof_assignment[prof_id].begin()+i);
                                //not_assign_cid.erase(not_assign_cid.find(cid)); //ERASED RECENTLY
                                amount_course_assigned=1;
                                break;
                            }
                        }
                    }
                    if(amount_course_assigned==1) break;
                }
                
            }
            not_assign_cid.erase(not_assign_cid.find(cid));

            if(amount_course_assigned==1) return 1;
            else
            {
                prof_assignment[firsthalf_profid].erase(prof_assignment[firsthalf_profid].end()-1); //FOCUSPOINT  
            }

        }

    }
    return -1;
         
}

int main()
{


    // PART 1 : THE STORING OF INPUT FILES INTO MANY SMALL USEFUL PIECES

     // input from txt
    std::ifstream inputFile("input.txt");

    if (!inputFile) {
        std::cerr << "Error opening the file." << std::endl;
        return 1;
    }

    // Redirect cin to read from the file
    std::streambuf *orig_cin = std::cin.rdbuf(inputFile.rdbuf());
    int num_of_profs;
    cin>> num_of_profs;
    cin >> ws; //skips first input line

    // declared as global map<int,double> prof_potential; // max courses can be taken by prof, first element prof id second potential
    map<int,string> prof_id; //this stores id of prof as key and stored prof name as value
    map<string,int> rev_prof_id; // this is used when i want to find id of a prof
    map<int,string> course_id; //init after the master input for loop 
    map<string,int> rev_course_id; // used when i want id of a course 
    set<string> courses; //to find unique total courses offered 

    map<int,vector<int>> prof_courses;// key of map is prof id, value is all the courses(id) prof take
    map<int,vector<int>> prof_fd_cdc; // key of map is prof id , vector<int> is vector of course id offered by prof
    map<int,vector<int>> prof_hd_cdc; // same as above
    map<int,vector<int>> prof_fd_elec; // same as above
    map<int,vector<int>> prof_hd_elec; // same as above


    vector<vector<string> > input ; 
    for (int  i = 0; i < num_of_profs ; i++)
    {

       
        //// 
        string inputLine;
        getline(cin, inputLine);
        vector<string> prof_deets;//this string contain all the inputs as declared in input.txt
        istringstream tokenStream(inputLine);
        string token;
        while (getline(tokenStream, token, ',')) { prof_deets.push_back(token); }
        ////
        //removing leading and lagging space from each token
        for(string &st: prof_deets)
        {
            size_t start = st.find_first_not_of(" \t\n\r\f\v");
            if (start == string::npos) {
                st = "";
            } else {
                size_t end = st.find_last_not_of(" \t\n\r\f\v");
                st = st.substr(start, end - start + 1);
            }
        }

        input.push_back(prof_deets);

        prof_id[i+1]=prof_deets[0];
        rev_prof_id[prof_deets[0]] = i+1;

        prof_potential[i+1] = stod(prof_deets[1]);
        
        for (int  i = 2; i < prof_deets.size(); i++)
        {
            int num_of_given_type_course = stoi(prof_deets[i]);
            while(num_of_given_type_course!=0)
            {
                i++;
                courses.insert(prof_deets[i]);// inserting all the courses in courses set to help make id for courses
                num_of_given_type_course--;
            }
        }           
        prof_deets.clear();
    }

    
    //course_id map init
    int c_id=1;
    for(string s: courses)
    {
        course_id[c_id]=s;
        rev_course_id[s] = c_id;
        c_id++;
    }
    
    //master vector to individual map data copy
    int flag=0; // this is used to distinguish between maps of types of courses
    for (int  i = 0; i < input.size() ; i++)
    {
        flag=0;
        for (int  j = 2; j < input[i].size(); j++)
        {
            flag++;
            int num_of_given_type_course = stoi(input[i][j]);
            while(num_of_given_type_course!=0)
            {
                j++;
                prof_courses[rev_prof_id[input[i][0]]].push_back(rev_course_id[input[i][j]]);
                if(flag==1)
                {//this just stores course ids of fd cdc of a particular prof(id)
                    prof_fd_cdc[rev_prof_id[input[i][0]]].push_back(rev_course_id[input[i][j]]);                   
                }
                else if(flag==2)
                {
                    //same as above
                    prof_hd_cdc[rev_prof_id[input[i][0]]].push_back(rev_course_id[input[i][j]]);
                }
                else if(flag==3)
                {//same as above
                    prof_fd_elec[rev_prof_id[input[i][0]]].push_back(rev_course_id[input[i][j]]);
                }
                else if(flag==4)
                {//same as above
                    prof_hd_elec[rev_prof_id[input[i][0]]].push_back(rev_course_id[input[i][j]]);
                }
                num_of_given_type_course--;
            }
        }
        
    }

    map<int, int> new_cid; // key is old course id and value is revised course id, order of new cid -> fdcdc,hdcdc,fdelec,hdelec
    map<int,int > rev_new_cid; //key is revised course id and value is old course id,
    int newid_allocator =  1; //value of current new id to be alloted 
    int column_num = 0;//column currently in use of (say fdcdc map)
    int count = 0;//allows to traverse map and breaks once all elements are traveresed(all vectors and all elements in them)
    while(count < prof_fd_cdc.size())
    {
        count = 0;
        for(auto it = prof_fd_cdc.begin() ; it != prof_fd_cdc.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++; //count increments seeing a vector size less than column num which ensures complete traversal of that vector 
            else
            {
                if(new_cid[(*it).second[column_num]] == 0) //checks if id has been alloted previously
                {
                    new_cid[(*it).second[column_num]] = newid_allocator; //makes newid
                    rev_new_cid[newid_allocator] = (*it).second[column_num];
                    newid_allocator++;
                }
            }
        }
        column_num++;
    }
    //same as above for rest
    column_num = 0;
    count = 0;
    while(count < prof_hd_cdc.size())
    {
        count = 0;
        for(auto it = prof_hd_cdc.begin() ; it != prof_hd_cdc.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {
                if(new_cid[(*it).second[column_num]] == 0)
                {
                    new_cid[(*it).second[column_num]] = newid_allocator;
                    rev_new_cid[newid_allocator] = (*it).second[column_num];
                    newid_allocator++;
                }
            }
        }
        column_num++;
    }
    column_num = 0;
    count = 0;
    while(count < prof_fd_elec.size())
    {
        count = 0;
        for(auto it = prof_fd_elec.begin() ; it != prof_fd_elec.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {
                if(new_cid[(*it).second[column_num]] == 0)
                {
                    new_cid[(*it).second[column_num]] = newid_allocator;
                    rev_new_cid[newid_allocator] = (*it).second[column_num];
                    newid_allocator++;
                }
            }
        }
        column_num++;
    }
    column_num = 0;
    count = 0;
    while(count < prof_hd_elec.size())
    {
        count = 0;
        for(auto it = prof_hd_elec.begin() ; it != prof_hd_elec.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {
                if(new_cid[(*it).second[column_num]] == 0)
                {
                    new_cid[(*it).second[column_num]] = newid_allocator;
                    rev_new_cid[newid_allocator] = (*it).second[column_num];
                    newid_allocator++;
                }
            }
        }
        column_num++;
    }

    
    // PART - 2 : THE MAN ALGORITHM BEGINS 

    // init master map
    //using above traversal route but updating master at end
    column_num = 0;
    count = 0;
    while(count < prof_fd_cdc.size())
    {
        count = 0;
        for(auto it = prof_fd_cdc.begin() ; it != prof_fd_cdc.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {//master updated here to have key as new course id and value as a vector of profs who can take that course
                //also priority order of the profs are maintained due to new course id
                master[new_cid[(*it).second[column_num]]].push_back((*it).first); 
            }
        }
        column_num++;
    }
    column_num = 0;
    count = 0;
    while(count < prof_hd_cdc.size())
    {
        count = 0;
        for(auto it = prof_hd_cdc.begin() ; it != prof_hd_cdc.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {
                master[new_cid[(*it).second[column_num]]].push_back((*it).first); 
            }
        }
        column_num++;
    }
    column_num = 0;
    count = 0;
    while(count < prof_fd_elec.size())
    {
        count = 0;
        for(auto it = prof_fd_elec.begin() ; it != prof_fd_elec.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {
                master[new_cid[(*it).second[column_num]]].push_back((*it).first); 
            }
        }
        column_num++;
    }
    column_num = 0;
    count = 0;
    while(count < prof_hd_elec.size())
    {
        count = 0;
        for(auto it = prof_hd_elec.begin() ; it != prof_hd_elec.end(); it++ )
        {
            if( (*it).second.size() <= column_num) count++;
            else
            {
                master[new_cid[(*it).second[column_num]]].push_back((*it).first); 
            }
        }
        column_num++;
    }

    //PART 3- THE MATURING OF THE MAN ALGORITHM
    
    for(auto pr: master)
    {
        not_assign_cid.clear();
        assign(pr.first,1);
    }

    //PART 4- I AM TOO TIRED TO THINK OF OMETHING


    ofstream outputFile("output.txt");

    if (!outputFile) {
        cerr << "Error opening the file for output." << endl;
        return 1;
    }
    streambuf *orig_cout = cout.rdbuf(outputFile.rdbuf());

    cout<<"Courses:     Professors in order of priority:"<< endl;
    for(auto pr : master)
    {
        cout<<course_id[rev_new_cid[pr.first]]<<"           ";
        for (int  i = 0; i < pr.second.size() ; i++)
        {
            cout<<prof_id[pr.second[i]]<<" ";
        }
        cout<<endl;        
    }
    cout<<endl;

    for(auto pr: prof_assignment)
    {
        map<int,int> assigned;
        for (int  i = 0; i < pr.second.size() ; i++)
        {
            assigned[pr.second[i]]++;
            
        }
        
        cout << prof_id[pr.first] << " is assigned the following:" << endl;
        int i=1;
        for (auto it = assigned.begin(); it != assigned.end() ; it++)
        {
            if(it->second==1) cout<<"\t"<<i<<". Half a course of "<<course_id[rev_new_cid[it->first]]<<endl;
            else cout<<"\t"<<i<<". Full course of "<<course_id[rev_new_cid[it->first]]<<endl;
            i++;
        }
        cout<<endl; 
    }




    outputFile.close();
    cout.rdbuf(orig_cout);

    return 0;


}
