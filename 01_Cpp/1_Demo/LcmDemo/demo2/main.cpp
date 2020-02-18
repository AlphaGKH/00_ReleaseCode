#include <stdio.h>

#include <lcm/lcm-cpp.hpp>
#include "exlcm/example_t.hpp"

#include <iostream>
#include <thread>
#include <mutex>

class Listener
{
public:
    Listener(std::string channel_name) {
        Init(channel_name);
        is_inited_ = true;
    }

    ~Listener() {
        if(listener_thread_ && listener_thread_->joinable()){
            listener_thread_->join();
        }
    }

    void OnMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
                   const exlcm::example_t *msg){
        {
            std::lock_guard<std::mutex> lock(count_mutex_);

            count_ = msg->timestamp;
        }
        std::cout << "Subscribe_id: " << std::this_thread::get_id() << " count: " << count_ << std::endl;
    }

    bool Init(std::string channel_name){
        if (!lcm_.good()){
            std::cout << "Listener Lcm Error!" << std::endl;
            return false;
        }

        lcm_.subscribe(channel_name, &Listener::OnMessage,this);

//        while (0 == lcm_.handle()) {
//            // Do nothing
//        }
        return true;
    }

    void GetCount(int* count){
        std::lock_guard<std::mutex> lock(count_mutex_);
        *count = count_;
    }

    void Start(){
        if(!is_inited_){
            std::cout << "Listener Not Init!" << std::endl;
            return;
        }

        listener_thread_ = std::make_unique<std::thread>(&Listener::ListenerRun,this);

    }

    void Stop(){
        if(listener_thread_ && listener_thread_->joinable()){
            listener_thread_stoped_ = true;
            listener_thread_->join();
        }

    }

private:
    void ListenerRun(){
        while (!listener_thread_stoped_) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(50));
            if (0 != lcm_.handle()) {
                std::cout << "Fail Listener!" << std::endl;
                continue;
            }
        }
    }



private:
    lcm::LCM lcm_;
    std::mutex count_mutex_;
    long count_ = 0;

    bool listener_thread_stoped_ = false;
    std::unique_ptr<std::thread> listener_thread_;

    bool is_inited_ = false;

};

class Process
{
public:
    Process() {}
    ~Process() {
        if(process_thread_ && process_thread_->joinable()){
            process_thread_->join();
        }
    }

    void Init(){
        std::string channel_name = "EXAMPLE";
        listener_ = std::make_unique<Listener>(channel_name);
        is_inited_ = true;
    }

    void Start(){

        if(listener_){
            listener_->Start();
        }

        if(!is_inited_){
            std::cout << "Process Not Init!" << std::endl;
            return;
        }

        process_thread_ = std::make_unique<std::thread>(&Process::ProcessRun,this);
    }

    void Stop(){
        if(process_thread_ && process_thread_->joinable()){
            processs_thread_stoped_ = true;
            process_thread_->join();

        }
    }

private:

    bool RunOnce(){
        int number;
        listener_->GetCount(&number);
        std::cout << "Process_thread_id: " << std::this_thread::get_id() << " count is: " << number << std::endl;
        return true;
    }

    void ProcessRun(){
        while (!processs_thread_stoped_) {
            std::this_thread::yield();
            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(500));
            if (!RunOnce()) {
                std::cout << "Fail Process!" << std::endl;
                continue;
            }
        }
    }



private:
    std::unique_ptr<Listener> listener_;
    bool is_inited_ = false;
    bool processs_thread_stoped_ = false;
    std::unique_ptr<std::thread> process_thread_;


};



int main()
{

    Process p;
    p.Init();
    p.Start();

//    Listener l;

    return 0;
}
