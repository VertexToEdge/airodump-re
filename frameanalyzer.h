#ifndef FRAMEANALYZER_H
#define FRAMEANALYZER_H

#include <customanalyzer.h>
#include <apinfo.h>
#include <stinfo.h>


typedef std::map<std::string,std::any> callbackResult;

typedef std::shared_ptr<callbackResult> pCallbackResult;
typedef std::function<void(BinContainer,APInfo&,STInfo&)> funcCallback;


class FrameAnalyzer:public CustomAnalyzer
{
private:
    std::map<u_int32_t,std::tuple<funcCallback,pCallbackResult>> callbacks;
public:
    FrameAnalyzer();
};

#endif // FRAMEANALYZER_H
