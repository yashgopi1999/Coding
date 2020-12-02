#include<bits/stdc++.h>
#include<chrono>
#include<unistd.h>
#include<sys/types.h>
using namespace std::chrono;
using namespace std;

class key_value {

    public :

    int unique_id;
    string path;
    vector<string> keys;
   
    
    key_value(void);

    key_value(string);

    void Create(string ,string ,int );

    void Read(string);

    void Delete(string);

    inline vector<string> parse(string);

};


key_value::key_value(std::string directory){
        unique_id = getpid();
        path = directory + path = std::to_string(unique_id) + "_key_value_store.txt";
    }

void key_value::Create(std::string key,std::string value,int time_to_live_in_seconds){

        if(std::find(keys.begin(),keys.end(),key) != keys.end()){
            std::cout<<"**error**"<<std::endl<<"key already present"<<std::endl;
            return;
        }
        
        std::ifstream read(path,std::ios::binary);
        read.seekg(0,std::ios::end);
        if(read.tellg() >= 1024*1024){
            std::cout<<"*error**"<<std::endl<<"exceeded maximum storage capacity"<<std::endl;
            return;
        }
        std::ofstream write;
        write.open(path);
        auto current_time = std::chrono::system_clock::now();
        current_time = time_point_cast<std::chrono::seconds>(current_time);
        auto dur = current_time.time_since_epoch();
        dur = duration_cast<seconds>(dur);
        std::string text = key + "," + value + "," + std::to_string(time_to_live_in_seconds) + "," + std::to_string(dur.count());

        write<<text<<std::endl;

        write.close();

        keys.push_back(key);
        return;
    }

void key_value::Read(std::string key){

        if(std::find(keys.begin(),keys.end(),key) == keys.end()){

            std::cout<<"**error**"<<std::endl<<"invalid key"<<std::endl;
            return;
        }

        std::ifstream read(path);
        std::string text;
        std::vector<std::string> parsed_text;
        while(read.is_open() && !read.eof()){

            getline(read,text);
            
            if(text.substr(0,key.length()) == key){
                parsed_text = parse(text);

                for(auto it : parsed_text){
                    std::cout<<it<<endl;
                }

                if(parsed_text[2] != "0") {

                auto current_time = std::chrono::system_clock::now();
                current_time = time_point_cast<std::chrono::seconds>(current_time);
                auto dur = current_time.time_since_epoch();
                dur = duration_cast<seconds>(dur);
                
                long time_lapse,start_time;
                std::stringstream ss;
                ss<<parsed_text[2];
                ss>>time_lapse;
                
                if(time_lapse <= dur.count() - start_time){
                    std::cout<<"**error**"<<std::endl<<"the key has expired"<<std::endl;
                    return;
                }
                
                std::cout<<parsed_text[1]<<std::endl;
                }
                else {
                    cout<<parsed_text[1]<<endl;
                }

            }

        }

        return;

    }

void key_value::Delete(std::string key){

        auto find_key  = std::find(keys.begin(),keys.end(),key);

        if(find_key == keys.end()){
            std::cout<<"**error**"<<std::endl<<"invalid key"<<std::endl;
        } 
        keys.erase(find_key);        

        return;
    }

inline std::vector<std::string> key_value::parse(std::string text){

        std::vector<std::string> parsed_text;

        int previous = 0;

        for(int i = 0 ; i < text.length();i++){
            if(text[i] == ','){
                parsed_text.push_back(text.substr(previous,i - previous));
                previous = i+1;
            }
        }
        parsed_text.push_back(text.substr(previous,text.length()-previous));

        return parsed_text;

    } 

key_value::key_value(void){
     unique_id = getpid();
     path = std::to_string(unique_id) + "_key_value_store.txt";

}
