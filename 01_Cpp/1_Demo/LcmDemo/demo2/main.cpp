#include <iostream>
#include <thread>
#include <mutex>
#include <list>

#include "lcm/lcm-cpp.hpp"

#include "exlcm/example_t.hpp"

#include "time/rate.h"

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

        lcm_ = nullptr;
    }

    std::shared_ptr<exlcm::example_t> GetLatestObserved() const {
        std::lock_guard<std::mutex> lock(msg_mutex_);
        if (observed_queue_.empty()) {
            return nullptr;
        }

        return observed_queue_.front();
    }

    bool Init(std::string channel_name){
        if (!lcm_.good()){
            std::cout << "Listener Lcm Error!" << std::endl;
            return false;
        }

        lcm_.subscribe(channel_name, &Listener::OnMessage,this);
        return true;
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

    void Observe() {
        std::lock_guard<std::mutex> lock(msg_mutex_);
        observed_queue_ = received_queue_;

        return;
    }

    bool Empty() const {
        std::lock_guard<std::mutex> lock(msg_mutex_);
        return received_queue_.empty();
    }

private:
    void ListenerRun(){
        while (!listener_thread_stoped_) {
            std::this_thread::yield();
//            std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(50));
            int res = lcm_.handleTimeout(200);
            if (res > 0) {
                continue;
            }
            else if (res == 0) {
                std::cout << "channel time out" << std::endl;
            }
            else {
                std::cout << "error ocuur!" << std::endl;
            }
        }
    }

    void OnMessage(const lcm::ReceiveBuffer *rbuf, const std::string &chan,
                   const exlcm::example_t *msg){

            auto msg_ptr = std::make_shared<exlcm::example_t>(*msg);
            Enqueue(msg_ptr);

//            uint64_t delay_time = Time::Now().ToNanosecond() - msg_ptr->timestamp;

//            std::cout << "delay time: " << delay_time << std::endl;

    }

    void Enqueue(const std::shared_ptr<exlcm::example_t>& msg_ptr){
        if(depth_ == 0){
            return;
        }

        std::lock_guard<std::mutex> lock(msg_mutex_);

        received_queue_.push_front(msg_ptr);

        while (received_queue_.size() > depth_) {
            received_queue_.pop_back();
        }

    }



private:
    lcm::LCM lcm_;
    mutable std::mutex msg_mutex_;

    bool listener_thread_stoped_ = false;
    std::unique_ptr<std::thread> listener_thread_;

    bool is_inited_ = false;

    std::list<std::shared_ptr<exlcm::example_t>> received_queue_;
    std::list<std::shared_ptr<exlcm::example_t>> observed_queue_;
    uint64_t depth_ = 10;

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

        listener_->Observe();

        if(listener_->Empty()){
            return false;
        }

        auto msg = listener_->GetLatestObserved();

        uint64_t delay_time = Time::Now().ToNanosecond() - msg->timestamp;

        std::cout << "delay time: " << delay_time << std::endl;

        return true;
    }

    void ProcessRun() {
        Rate rate(10.0);
        while (!processs_thread_stoped_) {
            std::this_thread::yield();
            if (!RunOnce()) {
                std::cout << "Fail Process!" << std::endl;
                continue;
            }
            rate.Sleep();
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

    return 0;
}
