#ifndef CUSTOMANALYZER_H
#define CUSTOMANALYZER_H

#include <pch.h>
#include <bincontainer.h>

typedef std::map<std::string,std::any> callbackResult;

typedef std::shared_ptr<callbackResult> pCallbackResult;
typedef std::function<void(BinContainer,callbackResult&)> funcCallback;

template<typename codeType,typename FuncType,typename ResultType>
class CustomAnalyzer
{
private:
    std::map<codeType,std::tuple<FuncType,std::shared_ptr<ResultType>>> callbacks;

public:
    void registerCallback(codeType code, FuncType f);
    void unregisterCallback(codeType code);
    callbackResult getCallbackResult(codeType code);
    bool callCallback(codeType code,BinContainer data);
    bool callCallback(codeType code,BinContainer data,ResultType &userResult);

    bool isExist(codeType code);

    CustomAnalyzer();
};

template<typename codeType,typename FuncType,typename ResultType>
CustomAnalyzer<codeType, FuncType, ResultType>::CustomAnalyzer()
{

}

template<typename codeType,typename FuncType,typename ResultType>
void  CustomAnalyzer<codeType, FuncType, ResultType>::registerCallback(codeType code, FuncType f){
    std::shared_ptr<ResultType> Result(new ResultType);
    this->callbacks[code] = std::make_tuple(f,Result);
}
template<typename codeType,typename FuncType,typename ResultType>
void  CustomAnalyzer<codeType, FuncType, ResultType>::unregisterCallback(codeType code){
    auto iterCode = this->callbacks.find(code);
    if(iterCode != this->callbacks.end()){
        this->callbacks.erase(iterCode);
    }
}

template<typename codeType,typename FuncType,typename ResultType>
bool  CustomAnalyzer<codeType, FuncType, ResultType>::callCallback(codeType code,BinContainer data){
    auto iterCode = this->callbacks.find(code);
    if(iterCode != this->callbacks.end()){
        FuncType f = std::get<0>(iterCode->second);
        std::shared_ptr<ResultType> result = std::get<1>(iterCode->second);
        f(data,*result);
        return true;
    }
    else{
        return false;
    }
}

template<typename codeType,typename FuncType,typename ResultType>
bool CustomAnalyzer<codeType, FuncType, ResultType>::callCallback(codeType code,BinContainer data,ResultType &userResult){
    auto iterCode = this->callbacks.find(code);
    if(iterCode != this->callbacks.end()){
        FuncType f = std::get<0>(iterCode->second);
        //std::shared_ptr<ResultType> result = std::get<1>(iterCode->second);
        f(data,userResult);
        return true;
    }
    else{
        return false;
    }
}



template<typename codeType,typename FuncType,typename ResultType>
callbackResult  CustomAnalyzer<codeType, FuncType, ResultType>::getCallbackResult(codeType code){
    return *std::get<1>(this->callbacks[code]);
}

template<typename codeType,typename FuncType,typename ResultType>
bool  CustomAnalyzer<codeType, FuncType, ResultType>::isExist(codeType code){
    return this->callbacks.find(code) != this->callbacks.end();
}

#endif // CUSTOMANALYZER_H
