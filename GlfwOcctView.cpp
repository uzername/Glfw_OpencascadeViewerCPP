// Copyright (c) 2019 OPEN CASCADE SAS
//
// This file is part of the examples of the Open CASCADE Technology software library.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

// WinAPIUtils has complicated file open dialog. WinAPIDialog2 has basic file open dialog
// Complicated file open dialog does not work well , it gives error 'Ordinal 344 not found in DLL'. well winapi is hard
// it is solvable : https://stackoverflow.com/a/43215416/5128696

//#include "WinAPIUtils.h" 
#include "WinAPIDialog2.h"


#include "GlfwOcctView.h"

#include <AIS_Shape.hxx>
#include <Aspect_Handle.hxx>
#include <Aspect_DisplayConnection.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <Message.hxx>
#include <Message_Messenger.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <iostream>

#include <GLFW/glfw3.h>



namespace
{
    
  //! Convert GLFW mouse button into Aspect_VKeyMouse.
  static Aspect_VKeyMouse mouseButtonFromGlfw (int theButton)
  {
    switch (theButton)
    {
      case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
      case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
      case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
    }
    return Aspect_VKeyMouse_NONE;
  }

  //! Convert GLFW key modifiers into Aspect_VKeyFlags.
  static Aspect_VKeyFlags keyFlagsFromGlfw (int theFlags)
  {
    Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
    if ((theFlags & GLFW_MOD_SHIFT) != 0)
    {
      aFlags |= Aspect_VKeyFlags_SHIFT;
    }
    if ((theFlags & GLFW_MOD_CONTROL) != 0)
    {
      aFlags |= Aspect_VKeyFlags_CTRL;
    }
    if ((theFlags & GLFW_MOD_ALT) != 0)
    {
      aFlags |= Aspect_VKeyFlags_ALT;
    }
    if ((theFlags & GLFW_MOD_SUPER) != 0)
    {
      aFlags |= Aspect_VKeyFlags_META;
    }
    return aFlags;
  }
}

// ================================================================
// Function : GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::GlfwOcctView()
{
}

// ================================================================
// Function : ~GlfwOcctView
// Purpose  :
// ================================================================
GlfwOcctView::~GlfwOcctView()
{
}

// ================================================================
// Function : toView
// Purpose  :
// ================================================================
GlfwOcctView* GlfwOcctView::toView (GLFWwindow* theWin)
{
  return static_cast<GlfwOcctView*>(glfwGetWindowUserPointer (theWin));
}

// ================================================================
// Function : errorCallback
// Purpose  :
// ================================================================
void GlfwOcctView::errorCallback (int theError, const char* theDescription)
{
  Message::DefaultMessenger()->Send (TCollection_AsciiString ("Error") + theError + ": " + theDescription, Message_Fail);
}

// ================================================================
// Function : run
// Purpose  :
// ================================================================
void GlfwOcctView::run()
{
  initWindow (800, 600, "glfw occt");
  initViewer();
  //initDemoScene();
  if (myView.IsNull())
  {
    return;
  }

  myView->MustBeResized();
  myOcctWindow->Map();
  initUI();
  mainloop();
  cleanupUI();
  cleanup();
}

void GlfwOcctView::OnSelectionChanged(const Handle(AIS_InteractiveContext)& theCtx, const Handle(V3d_View)& theView)
{
// on selection changed handler. 
// see \opencascade-7.7.0\samples\qt\Common\src\ApplicationCommon.cxx, OnSelectionChanged
// also https://stackoverflow.com/q/69475538/5128696
// DARK RITE OF RTTI in C++ is occuring here
int selectedItemsCount = theCtx->NbSelected();
selectionDescriptor = "SELECTION INFO. ITEMS: ";
selectionDescriptor.append(std::to_string(selectedItemsCount));
if (selectedItemsCount) {
    for (theCtx->InitSelected(); theCtx->MoreSelected(); theCtx->NextSelected()) {
        opencascade::handle<AIS_InteractiveObject> obbjj = theCtx->SelectedInteractive();
        if (obbjj->Type() == AIS_KindOfInteractive_Shape) {
           opencascade::handle<AIS_Shape> shapeFromSelection = opencascade::handle<AIS_Shape>::DownCast(obbjj);
           if (shapeFromSelection->Type() == AIS_KindOfInteractive_Shape) {
               //opencascade::handle< TopoDS_Shape> shapeReal (& shapeFromSelection->Shape() );
               //opencascade::handle<TopoDS_Vertex> vertexDistilled = opencascade::handle<TopoDS_Vertex>::DownCast(shapeReal);
               TopoDS_Shape shapeGeometric = shapeFromSelection->Shape();
               // NOT WORKING!
               opencascade::handle<TopoDS_TShape> underlineShape = shapeGeometric.TShape();
               TopAbs_ShapeEnum valll = underlineShape->ShapeType();
               if (valll == TopAbs_ShapeEnum::TopAbs_VERTEX) {
                   opencascade::handle<TopoDS_TVertex> shapeFromSelection = opencascade::handle<TopoDS_TVertex>::DownCast(underlineShape);
                   
                   
               }
           }
        }

    }
}
}

// ================================================================
// Function : initWindow
// Purpose  :
// ================================================================
void GlfwOcctView::initWindow (int theWidth, int theHeight, const char* theTitle)
{
  glfwSetErrorCallback (GlfwOcctView::errorCallback);
  glfwInit();
  const bool toAskCoreProfile = true;
  if (toAskCoreProfile)
  {
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
#if defined (__APPLE__)
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  myOcctWindow = new GlfwOcctWindow (theWidth, theHeight, theTitle);
  glfwSetWindowUserPointer       (myOcctWindow->getGlfwWindow(), this);
  // window callback
  glfwSetWindowSizeCallback      (myOcctWindow->getGlfwWindow(), GlfwOcctView::onResizeCallback);
  glfwSetFramebufferSizeCallback (myOcctWindow->getGlfwWindow(), GlfwOcctView::onFBResizeCallback);
  // mouse callback
  glfwSetScrollCallback          (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseScrollCallback);
  glfwSetMouseButtonCallback     (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseButtonCallback);
  glfwSetCursorPosCallback       (myOcctWindow->getGlfwWindow(), GlfwOcctView::onMouseMoveCallback);
}

// ================================================================
// Function : initViewer
// Purpose  :
// ================================================================
void GlfwOcctView::initViewer()
{
  if (myOcctWindow.IsNull()
   || myOcctWindow->getGlfwWindow() == nullptr)
  {
    return;
  }

  Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver (myOcctWindow->GetDisplay(), false);
  aGraphicDriver->SetBuffersNoSwap(true);

  Handle(V3d_Viewer) aViewer = new V3d_Viewer (aGraphicDriver);
  aViewer->SetDefaultLights();
  aViewer->SetLightOn();
  aViewer->SetDefaultTypeOfView (V3d_PERSPECTIVE);
  aViewer->ActivateGrid (Aspect_GT_Rectangular, Aspect_GDM_Lines);
  myView = aViewer->CreateView();
  myView->SetImmediateUpdate (false);
  myView->SetWindow (myOcctWindow, myOcctWindow->NativeGlContext());
  myView->ChangeRenderingParams().ToShowStats = false;
  myContext = new AIS_InteractiveContext (aViewer);
}

// ================================================================
// Function : initDemoScene
// Purpose  :
// ================================================================
void GlfwOcctView::initDemoScene()
{
  if (myContext.IsNull())
  {
    return;
  }

  myView->TriedronDisplay (Aspect_TOTP_LEFT_LOWER, Quantity_NOC_GOLD, 0.08, V3d_WIREFRAME);

  gp_Ax2 anAxis;
  anAxis.SetLocation (gp_Pnt (0.0, 0.0, 0.0));
  Handle(AIS_Shape) aBox = new AIS_Shape (BRepPrimAPI_MakeBox (anAxis, 50, 50, 50).Shape());
  myContext->Display (aBox, AIS_Shaded, 0, false);
  anAxis.SetLocation (gp_Pnt (25.0, 125.0, 0.0));
  Handle(AIS_Shape) aCone = new AIS_Shape (BRepPrimAPI_MakeCone (anAxis, 25, 0, 50).Shape());
  myContext->Display (aCone, AIS_Shaded, 0, false);

  if (this->showOpenGLinfo==true) {
  TCollection_AsciiString aGlInfo;
  {
    TColStd_IndexedDataMapOfStringString aRendInfo;
    myView->DiagnosticInformation (aRendInfo, Graphic3d_DiagnosticInfo_Basic);
    for (TColStd_IndexedDataMapOfStringString::Iterator aValueIter (aRendInfo); aValueIter.More(); aValueIter.Next())
    {
      if (!aGlInfo.IsEmpty()) { aGlInfo += "\n"; }
      aGlInfo += TCollection_AsciiString("  ") + aValueIter.Key() + ": " + aValueIter.Value();
    }
  }
  Message::DefaultMessenger()->Send (TCollection_AsciiString("OpenGL info:\n") + aGlInfo, Message_Info);
  }
}

// ================================================================
// Function : mainloop
// Purpose  :
// ================================================================
void GlfwOcctView::mainloop()
{
  while (!glfwWindowShouldClose (myOcctWindow->getGlfwWindow()))
  {
    // glfwPollEvents() for continuous rendering (immediate return if there are no new events)
    // and glfwWaitEvents() for rendering on demand (something actually happened in the viewer)
    //glfwPollEvents();
    glfwWaitEvents();
    if (!myView.IsNull())
    {               
        
        FlushViewEvents(myContext, myView, true);
        processUI();
        glfwSwapBuffers(myOcctWindow->getGlfwWindow());
    }
    
  }
}

// ================================================================
// Function : cleanup
// Purpose  :
// ================================================================
void GlfwOcctView::cleanup()
{
  if (!myView.IsNull())
  {
    myView->Remove();
  }
  if (!myOcctWindow.IsNull())
  {
    myOcctWindow->Close();
  }
  glfwTerminate();
}

void GlfwOcctView::initUI()
{
    items_combo[0] = "SHAPE";
    items_combo[1] = "VERTEX";
    items_combo[2] = "EDGE";
    item_current_idx_combo = 0;
    combo_preview_value = items_combo[item_current_idx_combo];

    // Setup Dear ImGui context
    const char* glsl_version = "#version 330";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(myOcctWindow->getGlfwWindow(), true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
}

void GlfwOcctView::processUI()
{
    // feed inputs to dear imgui, start new frame 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("STEP files");   
    if (ImGui::Button("Add File")) {
        AddFileBtnHandler();
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear List")) {
        ClearFileListBtnHandler();
    }
    if (ImGui::BeginListBox("##FilesListbox", ImVec2(-FLT_MIN, -FLT_MIN))) {		
        //https://github.com/ocornut/imgui/blob/master/imgui_demo.cpp

        for (int n = 0; n < fileInfoList.size(); n++)
        {
            const bool is_selected = (item_current_idx == n);
            
            if (ImGui::Selectable(fileInfoList[n].resultFileName.c_str(), is_selected))
                item_current_idx = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
                if (item_current_idx!=item_prev_idx) {
                    item_prev_idx = item_current_idx;
                    // process STEP file here
                    MyStepFileHandler fHandler;
                    Handle(TopTools_HSequenceOfShape) shapes = fHandler.importSTEP(fileInfoList[n].resultFileNameFull);
                    displayShSequence( shapes);
                }
            }

            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(fileInfoList[n].resultFileNameFull.c_str());
        }

	    ImGui::EndListBox();
    }
    
    ImGui::End();
    ImGui::SetNextWindowPos(ImVec2(100,10),ImGuiCond_::ImGuiCond_FirstUseEver);
    ImGui::Begin("Selection");
    ImGuiComboFlags flags_combo = 0;
    if (ImGui::BeginCombo("##SelectionTypes", combo_preview_value, flags_combo))
    {
        for (int n = 0; n < IM_ARRAYSIZE(items_combo); n++)
        {
            const bool is_selected_combo = (item_current_idx_combo == n);
            if (ImGui::Selectable(items_combo[n], is_selected_combo)){
                    item_current_idx_combo = n;
                    combo_preview_value = items_combo[item_current_idx_combo];
                    // SELECTION TYPE CHANGED
                    changeSelectionType(item_current_idx_combo);
                }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected_combo)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    ImGui::Text(selectionDescriptor.c_str());
    ImGui::End();
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GlfwOcctView::cleanupUI()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool GlfwOcctView::displayShSequence( const Handle(TopTools_HSequenceOfShape)& shapes)
{

    myContext->RemoveAll(false);
    if (shapes.IsNull() || !shapes->Length())
        return false;

    for (int i = 1; i <= shapes->Length(); i++)
        myContext->Display(new AIS_Shape(shapes->Value(i)), AIS_Shaded,-1, false);
    
    myContext->Activate((int)currentSelectionModeShapes);
    myView->FitAll(false);
    myContext->UpdateCurrentViewer();
    return true;
}

void GlfwOcctView::changeSelectionType(int indxSelection)
{
    myContext->Deactivate((int)currentSelectionModeShapes);
    currentSelectionModeShapes = (SelectionMode) indxSelection;
    myContext->Activate((int)currentSelectionModeShapes);
}

void GlfwOcctView::AddFileBtnHandler()
{
    MyFileInfo rsltFile;
    bool rslt= basicFileOpen2((HWND)myOcctWindow->NativeHandle(), &rsltFile);
    if (rslt) {
        fileInfoList.push_back(rsltFile);
    }
    
}

void GlfwOcctView::ClearFileListBtnHandler()
{

}

// ================================================================
// Function : onResize
// Purpose  :
// ================================================================
void GlfwOcctView::onResize (int theWidth, int theHeight)
{
  if (theWidth  != 0
   && theHeight != 0
   && !myView.IsNull())
  {
    myView->Window()->DoResize();
    myView->MustBeResized();
    myView->Invalidate();
    myView->Redraw();
  }
}

// ================================================================
// Function : onMouseScroll
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseScroll (double theOffsetX, double theOffsetY)
{
  if (!myView.IsNull())
  {
    UpdateZoom (Aspect_ScrollDelta (myOcctWindow->CursorPosition(), int(theOffsetY * 8.0)));
  }
}

// ================================================================
// Function : onMouseButton
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseButton (int theButton, int theAction, int theMods)
{
  if (myView.IsNull()) { return; }

  const Graphic3d_Vec2i aPos = myOcctWindow->CursorPosition();
  if (theAction == GLFW_PRESS)
  {
    PressMouseButton (aPos, mouseButtonFromGlfw (theButton), keyFlagsFromGlfw (theMods), false);
  }
  else
  {
    ReleaseMouseButton (aPos, mouseButtonFromGlfw (theButton), keyFlagsFromGlfw (theMods), false);
  }
}

// ================================================================
// Function : onMouseMove
// Purpose  :
// ================================================================
void GlfwOcctView::onMouseMove (int thePosX, int thePosY)
{
  const Graphic3d_Vec2i aNewPos (thePosX, thePosY);
  if (!myView.IsNull())
  {
    UpdateMousePosition (aNewPos, PressedMouseButtons(), LastMouseFlags(), false);
  }
}
