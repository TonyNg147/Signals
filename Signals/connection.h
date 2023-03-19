#ifndef __IOSTREAM__
#include <iostream>
#endif  // __IOSTREAM__
#include <functional>
#include <memory>
#include <list>
#include <mutex>
#include <shared_mutex>
template<typename _SIG>
class Signal{};
        struct ConnectState{
            bool _inConnect = true;
        }; 
        typedef std::shared_ptr<ConnectState> ConnectObject;
template<typename _SIG,typename... Args>
class Signal<_SIG(Args...)>
{
    private:
        #define FUNC_PROT std::function<_SIG(Args...)>
        #define INS_REF  std::weak_ptr<ConnectState>
        #define LIST_FUNC_REF std::list<std::pair<INS_REF,FUNC_PROT>>
        LIST_FUNC_REF m_listConnections;
        std::shared_mutex    m_threadProtected;
    public:
        Signal() = default;
        ConnectObject connect(FUNC_PROT func);
        void operator() (Args...);
};
template<typename _SIG,typename... Args>
std::shared_ptr<ConnectState> Signal<_SIG(Args...)>::connect(FUNC_PROT func){
    std::unique_lock<std::shared_mutex> lock(m_threadProtected);
    if (!func) return ConnectObject();
    ConnectObject connectInstance = std::make_shared<ConnectState>();
    INS_REF connectRef = connectInstance;
    std::pair<INS_REF,FUNC_PROT> pair{connectRef,func};
    m_listConnections.push_back(pair);
    return connectInstance;
}
template<typename _SIG,typename... Args>
void Signal<_SIG(Args...)>::operator() (Args... args) {
    std::shared_lock<std::shared_mutex> lock(m_threadProtected);
    auto iter = m_listConnections.begin();
    while (iter!=m_listConnections.end()){
        auto ins = (*iter);
        auto ref = ins.first.lock();
        if (ref){
            ins.second(args...);
            ++iter;
        }else{
            m_listConnections.erase(iter++);
            printf("Erase element\n");
        }
    };
}