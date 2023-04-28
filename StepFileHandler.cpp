#include "StepFileHandler.h"
#include <STEPControl_Reader.hxx>
//! see \opencascade-7.7.0\samples\qt\Interface\src\Translate.cxx
Handle(TopTools_HSequenceOfShape) MyStepFileHandler::importSTEP(const std::string& file)
{
    Handle(TopTools_HSequenceOfShape) aSequence = new TopTools_HSequenceOfShape;
    TCollection_AsciiString  aFilePath = file.data();
    STEPControl_Reader aReader;
    IFSelect_ReturnStatus status = aReader.ReadFile(aFilePath.ToCString());
    if (status != IFSelect_RetDone)
    {
        return aSequence;
    }

    //Interface_TraceFile::SetDefault();
    bool failsonly = false;
    aReader.PrintCheckLoad(failsonly, IFSelect_ItemsByEntity);

    int nbr = aReader.NbRootsForTransfer();
    aReader.PrintCheckTransfer(failsonly, IFSelect_ItemsByEntity);
    for (Standard_Integer n = 1; n <= nbr; n++)
    {
        aReader.TransferRoot(n);
    }

    int nbs = aReader.NbShapes();
    if (nbs > 0)
    {
        for (int i = 1; i <= nbs; i++)
        {
            TopoDS_Shape shape = aReader.Shape(i);
            aSequence->Append(shape);
        }
    }

    return aSequence;
}
