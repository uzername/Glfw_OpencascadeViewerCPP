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

#ifndef _GlfwOcctView_Header
#define _GlfwOcctView_Header

#include "GlfwOcctWindow.h"

#include <AIS_InteractiveContext.hxx>
#include <AIS_ViewController.hxx>
#include <V3d_View.hxx>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


#include "MyFileInfoStruct.h"

#include "StepFileHandler.h"
// see static Standard_Integer AIS_Shape::SelectionMode(const TopAbs_ShapeEnum 	aShapeType)
typedef enum SelectionMode {
	SHAPE = 0,
	VERTEX = 1,
	EDGE = 2
} SelectModeCurrent;
//! Sample class creating 3D Viewer within GLFW window.
class GlfwOcctView : protected AIS_ViewController
{
public:
  //! Default constructor.
	GlfwOcctView();

  //! Destructor.
	~GlfwOcctView();

  //! Main application entry point.
  void run();
  //! show OpenGL info to window , used in initDemoScene
  bool showOpenGLinfo = false;
  // selection changed - react on this
  void OnSelectionChanged(const Handle(AIS_InteractiveContext)& theCtx,
	  const Handle(V3d_View)& theView);
private:

  //! Create GLFW window.
  void initWindow (int theWidth, int theHeight, const char* theTitle);

  //! Create 3D Viewer.
  void initViewer();

  //! Fill 3D Viewer with a DEMO items.
  void initDemoScene();

  //! Application event loop.
  void mainloop();

  //! Clean up before .
  void cleanup();

  //! init Dear ImGui interface
  void initUI();

  //! handle Dear ImGui interface
  void processUI();

  //! cleanup Dear ImGui interface
  void cleanupUI();

  //! UI related entries, list box mainly
  int currentItem;
  std::vector< char* > listboxItems;
  int currentItemsCount;
  bool selectionChanged; // not used

  //! Add shape to scene
  bool displayShSequence(
	  const Handle(TopTools_HSequenceOfShape)& shapes);
 
  //! selection type changed
  void changeSelectionType(int indxSelection) ;


  //! Add new file btn handler
  void AddFileBtnHandler();

  void ClearFileListBtnHandler();
//! @name GLWF callbacks
private:
  //! Window resize event.
  void onResize (int theWidth, int theHeight);

  //! Mouse scroll event.
  void onMouseScroll (double theOffsetX, double theOffsetY);

  //! Mouse click event.
  void onMouseButton (int theButton, int theAction, int theMods);

  //! Mouse move event.
  void onMouseMove (int thePosX, int thePosY);

//! @name GLWF callbacks (static functions)
private:

  //! GLFW callback redirecting messages into Message::DefaultMessenger().
  static void errorCallback (int theError, const char* theDescription);

  //! Wrapper for glfwGetWindowUserPointer() returning this class instance.
  static GlfwOcctView* toView (GLFWwindow* theWin);

  //! Window resize callback.
  static void onResizeCallback (GLFWwindow* theWin, int theWidth, int theHeight)
  { toView(theWin)->onResize (theWidth, theHeight); }

  //! Frame-buffer resize callback.
  static void onFBResizeCallback (GLFWwindow* theWin, int theWidth, int theHeight)
  { toView(theWin)->onResize (theWidth, theHeight); }

  //! Mouse scroll callback.
  static void onMouseScrollCallback (GLFWwindow* theWin, double theOffsetX, double theOffsetY)
  { 
	  ImGuiIO& io = ImGui::GetIO();
	  if (!io.WantCaptureMouse) {
	  toView(theWin)->onMouseScroll (theOffsetX, theOffsetY); 
	  }
  }

  //! Mouse click callback.
  static void onMouseButtonCallback (GLFWwindow* theWin, int theButton, int theAction, int theMods)
  { 
	  ImGuiIO& io = ImGui::GetIO();
	  if (!io.WantCaptureMouse){
	  toView(theWin)->onMouseButton (theButton, theAction, theMods); 
	  }
  }

  //! Mouse move callback.
  static void onMouseMoveCallback (GLFWwindow* theWin, double thePosX, double thePosY)
  { 
	  ImGuiIO& io = ImGui::GetIO();
	  if (!io.WantCaptureMouse) {
      toView(theWin)->onMouseMove ((int )thePosX, (int )thePosY); 
	  }
	  else {
		  toView(theWin)->myView->Redraw();
	  }
  }

private:

  Handle(GlfwOcctWindow) myOcctWindow;
  Handle(V3d_View) myView;
  Handle(AIS_InteractiveContext) myContext;

  std::vector<MyFileInfo> fileInfoList;
  int item_current_idx = -1;
  int item_prev_idx = -1;

  SelectModeCurrent currentSelectionModeShapes = SelectModeCurrent::SHAPE;
  const char* items_combo[3] ;
  int item_current_idx_combo; // Here we store our selection data as an index.
  const char* combo_preview_value;  // Pass in the preview value visible before opening the combo (it could be anything)

  std::string selectionDescriptor = "SELECTION INFO";
  bool previousWasPoint = false;
  gp_Pnt previousPoint;
};


#endif // _GlfwOcctView_Header
