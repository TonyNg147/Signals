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
/**
 * This class is intended to implement the signal slot mechanism
 * One signsl when invoked will emit all the connected slots
 *
 * ...
 *
 * @tparam _SIG The return type of function
 * @tparam Args The argument list 
 *              Argument list can accept as many parameters as possible
 */
template<typename _SIG,typename... Args>
class Signal<_SIG(Args...)>
{
    private:
        #define FUNC_PROT std::function<_SIG(Args...)>
        #define INS_REF  std::weak_ptr<ConnectState>
        #define LIST_FUNC_REF std::list<std::pair<INS_REF,FUNC_PROT>>
        LIST_FUNC_REF m_listConnections;                // The connected list 
        std::shared_mutex    m_threadProtected;         // To protect shared environment 
                                                        // in case of multithreading
    public:
        Signal() = default;
        /*
            @brief    This function is used to connect slot
            @details  The connected slots (functions) will be called after
                      the invocation of the registered function
            @param    func The function
        */
        ConnectObject connect(FUNC_PROT func);
        /*
            @brief   The call method of the function
            @param   The variadic parameters
        */
        void operator() (Args...);
};
/*

*/
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