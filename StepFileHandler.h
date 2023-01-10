#ifndef MY_STEFILEHEADER
#define MY_STEFILEHEADER
#include <TopTools_HSequenceOfShape.hxx>
#include <string>
class MyStepFileHandler {
public:
    Handle(TopTools_HSequenceOfShape)         importSTEP(const std::string&);
};
#endif // !MY_STEFILEHEADER