// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
struct dep_packets {
    int message_dep; // It is Considered as Default Arguments and no Error is Raised
    int destination;
    int departure;
    int vc;
    int priority;
};

struct arr_packets {
    int message_arr; // It is Considered as Default Arguments and no Error is Raised
    int source;
    int arrival;
    int vc;
};

int main(int argc, char *argv[])
{
    string line;
    ifstream myfile(argv[1]);
    /*
    vector<vector<int>> dep;
    vector<vector<int>> arr;
    */
   int message_completed=0;
   int per_vc_th[4];
   int per_vc_lat[4];
   for(int i=-0;i<4;i++)
   {
       per_vc_th[i]=0;
       per_vc_lat[i]=0;
   }
   int initial_time=0;
   int final_time=0;
   int total_latency=0;
   int load=-1;
   int prev=0;
   //unit is flits
   int standard_message_length=8;
    vector<vector<dep_packets*>> d_terminal;
    vector<vector< arr_packets*>> a_terminal;
    int num_of_nodes = 16;
    for (int id = 0; id < num_of_nodes; id++)
    {
        vector<int> vald;
        vector<int> vala;
        //dep.push_back(vald);
        //arr.push_back(vala);
        vector<dep_packets*> samd;
        d_terminal.push_back(samd);
        vector<arr_packets*> sama;
        a_terminal.push_back(sama);
    }

    if (myfile.is_open())
    {   int check=0;
        while (getline(myfile, line))
        {
            check++;
            if (line.find("Departure") != string::npos)
            {
                //means it is sender
               string source=line.substr(line.find("de:") + 5, line.find("Mes") - line.find("de:") -5);
                //cout<<source<<"ssss\n";
                int source_i=stoi(source);
                //cout<<"source"<<source_i<<"\n";
                //cout << line.substr(line.find("e: ") + 2, line.find("Des") - line.find("e: ") - 3) << "\n";
                string message_ds = line.substr(line.find("ge: ") + 4, line.find("Des") - line.find("ge: ") - 3); //<<"\n";
                int message_d = stoi(message_ds);
                //cout<<message_ds<<"sssssssssssssss\n";
                string destination_ds = line.substr(line.find("n: ") + 2, line.find("Dep") - line.find("n: ") - 3);
                int destination_d=stoi(destination_ds);
                //cout<<destination_d<<"sdsd\n";
                string departure_ds = line.substr(line.find("me: ")+4, line.length() - line.find("me: "));
                //cout<<departure_ds<<"sddsd\n";
                int departure_d=stoi(departure_ds);
                //vector<int> curr_data={message_d,destination_d,departure_d};
                //dep[source_i].push_back(message_d);
                //dep[source_i].push_back(destination_d);
                //dep[source_i].push_back(departure_d);
                string source_vc_s=line.substr(line.find("VC:")+4,line.find("Pri")-line.find("VC:")-4);
                //cout<<source_vc_s<<"\n";
                int source_vc=stoi(source_vc_s);
                string priority_s=line.substr(line.find("ty")+4,line.length()-line.find("ty"));
                //cout<<priority_s<<"\n";
                int priority=stoi(priority_s);
                dep_packets* outgoing=new  dep_packets;
                outgoing->message_dep=message_d;
                outgoing->destination=destination_d;
                outgoing->departure=departure_d ;
                outgoing->vc=source_vc;
                outgoing->priority=priority;
                if(check==1)
                {
                    initial_time=departure_d;
                    prev=departure_d;
                }
                if(departure_d==prev)
                {
                    load++;
                }
                prev=departure_d;
                //struct dep_packets outgoing;
                //outgoing.message_dep=message_d;
                //outgoing.destination=destination_d;
                //outgoing.departure=departure_d;
                d_terminal[source_i].push_back(outgoing);/////core fault here
                //d_terminal.insert(d_terminal.begin() + source_i, outgoing);




                //cout << line << "\n";
            }
            else if (line.find("Arrival") != string::npos)
            {
                //means it is receiver
                //cout<<"aaa";
                string destination=line.substr(line.find("de:") + 5, line.find("Mes") - line.find("de:") -7);
                //cout<<destination<<"ssss\n";
                int destination_i=stoi(destination);
                //cout<<"source"<<destination_i<<"\n";//cprrect


                //cout << line.substr(line.find("ge: ") + 2, line.find("Sour") - line.find("ge: ") - 3) << "\n";
                string message_as = line.substr(line.find("ge: ") + 4, line.find("Sour") - line.find("ge: ") - 5); //<<"\n";
                int message_a = stoi(message_as);
                //cout<<message_a<<"mess\n";
                string source_as = line.substr(line.find("ce: ") + 4, line.find("Arr") - line.find("ce: ") - 5);
                int source_a=stoi(source_as);
                //cout<<source_as<<"sdsd\n";
                string arrival_as = line.substr(line.find("me: ")+4, line.length() - line.find("me: "));
                //cout<<arrival_as<<"sddsd\n";
                int arrival_a=stoi(arrival_as);
                //vector<int> curr_data={message_a,source_a,arrival_a};
                //arr[destination_i].push_back(message_a);
                //arr[destination_i].push_back(source_a);
                //arr[destination_i].push_back(arrival_a);
                string arrival_vc_s=line.substr(line.find("VC:")+4,line.length()-line.find("VC:")-4);
                //cout<<arrival_vc<<"sss\n";
                int arrival_vc=stoi(arrival_vc_s);
                arr_packets* incoming=new arr_packets;
                incoming->message_arr=message_a;
                incoming->source=source_a;
                incoming->arrival=arrival_a;
                incoming->vc=arrival_vc;
                final_time=arrival_a;
                a_terminal[destination_i].push_back(incoming);  ////core fault segmentation here
                //a_terminal.insert(a_terminal.begin()+destination_i,incoming);
            }
        }
        myfile.close();

        //
        cout<<" Negative time => packets not routed to destination\n\n\n";
        for(int i=0;i<d_terminal.size();i++)
        {    
            int source;
            int dest;
            int dept;
            int arrv;
            int messg;
            int vc;

            for(int j=0;j < d_terminal[i].size();j++)
            {
             source=i;
             dest=d_terminal[i][j]->destination;
             dept=d_terminal[i][j]->departure;
             messg=d_terminal[i][j]->message_dep;
             vc=d_terminal[i][j]->vc;
             arrv;
             int routed=0;
            for(int k=0;k<a_terminal[dest].size();k++)
            {
                if(a_terminal[dest][k]->message_arr==messg && a_terminal[dest][k]->source==source) 
                {
                    routed=1;
                    message_completed++;
                    per_vc_th[a_terminal[dest][k]->vc]++;
                    arrv=a_terminal[dest][k]->arrival;
                    break;
                }
            }
            if(routed==0)
            {
                arrv=0;
            }
            cout<<"Message "<<messg<<" from "<<source<<" to "<<dest<<" in time "<<arrv-dept<<"\n";
            total_latency+=arrv-dept;
            per_vc_lat[d_terminal[i][j]->vc]+=arrv-dept;
            }
        }
    cout<<"Debug output\n\n";
    cout<<"Overall System Load: "<<load/16<<"\n";
    cout<<"Overall Average Latency: "<<double(total_latency)/double(message_completed)<<"\n";
    cout<<"Overall Throughput: "<<double(message_completed*standard_message_length)/double(final_time-initial_time)<<" filts/cycle\n\n";
    cout<<"Per VC Throughput and latency and messages :"<<"\n";
    for(int i=0;i<4;i++)
    {
        cout<<"VC"<<i<<": "<<double((per_vc_th[i]*standard_message_length))/double(final_time-initial_time)<<" filts/cycle\n";
        cout<<"average latency: "<<double(per_vc_lat[i])/double(per_vc_th[i])<<"\n";
        cout<<"messages: "<<per_vc_th[i]<<"\n";
    }

    /*
    cout<<"DEPARTURES\n";
    for (int i = 0; i < dep.size(); i++)
    {   cout<<"for node"<<i<<"\n";
        for (int j = 0; j < dep[i].size(); j++)
        {
            cout << dep[i][j] << " ";
            if(j%3==2){
                cout<<"\n";
            }
        }   
        cout << endl;
    }
    cout<<"ARRIVAL\n";
     for (int i = 0; i < arr.size(); i++)
    {   cout<<"for node"<<i<<"\n";
        for (int j = 0; j < arr[i].size(); j++)
        {
            cout << arr[i][j] << " ";
            if(j%3==2){
                cout<<"\n";
            }
        }   
        cout << endl;
    }
    */
   /*
    cout<<"DEPARTURES\n";
    for(int i=0;i<9;i++){
        cout<<"node"<<i<<"\n";
        for(int j=0;j<d_terminal[i].size();j++){
                cout<<d_terminal[i][j]->message_dep<<" "<<d_terminal[i][j]->destination<<" "<<d_terminal[i][j]->departure;
                cout<<"\n";

        }



    }
    cout<<"ARRIVAL\n";
    for(int i=0;i<9;i++){
        cout<<"node"<<i<<"\n";
        for(int j=0;j<a_terminal[i].size();j++){
                cout<<a_terminal[i][j]->message_arr<<" "<<a_terminal[i][j]->source<<" "<<a_terminal[i][j]->arrival;
                cout<<"\n";

        }



    }
*/

    }

    else
        cout << "Unable to open file";

    return 0;
}
