# The XamlListDetail Project Reunion 0.5 sample retargeted from UWP

This repo contains the result of taking the [UWP XamlListDetail sample app](https://github.com/microsoft/Windows-universal-samples/tree/master/Samples/XamlMasterDetail), and retargeting it to Reunion/WinUI/Win32.

## Install the Project Reunion 0.5 VSIX

In Visual Studio, click **Extensions** > **Manage Extensions**, search for *Project Reunion*, and download the Project Reunion extension. Close and reopen Visual Studio, and follow the prompts to install the extension. For more info, see [Project Reunion 0.5](https://github.com/microsoft/ProjectReunion/releases/tag/0.5.0).

## Create a new project

In Visual Studio, create a new project from the **Blank App, Packaged (WinUI 3 in Desktop)** project template. You can find that by choosing language: C++; platform: Project Reunion; project type: Desktop (which is referring to Win32 in this context). Name the project *XamlListDetail*, uncheck **Place solution and project in the same directory**, and target the most recent release (not preview) of the platform (currently 19041).

There are actually two projects in Solution Explorer&mdash;one is qualified as **(Desktop)**, and the other as **(Package)**. I'll be referring only to the *Desktop* project in this walkthrough.

## Refactor the IDL into one file

Add a new **Midl File (.idl)** item to the project. Name the new item `Project.idl`. Delete the default contents of `Project.idl`.

Move all the IDL from `MainWindow.idl` into `Project.idl`, save that, and then delete `MainWindow.idl`.

Remove the dummy property from `Project.idl` and from `MainWindow.xaml.h` and `.cpp`. Remove the event handler from those files, too. In `MainWindow.xaml`, replace the content with just `<Grid/>`. Confirm you can build and run at this stage.

===


The main page is MasterDetailPage. But its XAML has dependencies on MasterDetailApp::ViewModels::ItemViewModel (which in turn depends on MasterDetailApp.Data.Item), so let's port those types across before porting the XAML. We'll also need to port across ItemsDataSource, and DetailPage before MasterDetailPage.

Edit Project.idl, and copy the declarations of Item and ItemViewModel into it (fixing up the namespace names everywhere). Build to generate implementation stubs (errors are expected). Show All Files, then copy Data.Item.h and .cpp and ViewModels.ItemViewModel.h and .cpp into the project folder then add them to the project. Delete the static asserts. Confirm you can build again.

Next let's port the code across for MasterDetailApp.Data.Item. Copy the private data members from Item.cpp over to Data.Item.h. And copy the implementations from Item.cpp over to Data.Item.cpp. It's all very straightforward. Except, change id and m_id back to itemId and m_itemId like they're meant to be called.

Port the code across for MasterDetailApp::ViewModels::ItemViewModel. Copy the private data members from ItemViewModel.cpp over to ViewModels.ItemViewModel.h. And copy the implementations from ItemViewModel.cpp over to ViewModels.ItemViewModel.h. You'll need to add the constructor manually, since that wasn't in the IDL for some reason. For this type to compile, add the following to pch.h:
```cppwinrt
#include <winrt/Windows.Globalization.DateTimeFormatting.h>
#include "winrt/Microsoft.UI.Xaml.Media.Animation.h"
#include "winrt/Windows.UI.Core.h"
```

Next create a new item (WinUI > Blank Page (WinUI 3)) named ListDetailPage.xaml. Move the IDL out of ListDetailPage.idl and into Project.idl; delete ListDetailPage.idl. Remove the dummy property from the IDL (and from the .h and .cpp). Remove the event handler from those files, too. In the xaml file, replace the content with just `<Grid x:Name="layoutRoot"/>`. Confirm you can build and run at this stage.

We'll also need to port across ItemsDataSource. No need for xaml here, just copy ItemsDataSource.h across, and fix up the namespace.

Copy the XAML from DetailPage.xaml over into DetailPage.xaml, including any necessary xaml attributes. Copy/replace all (match case and full word): MasterListViewItemTemplate => ListViewItemTemplate; MasterColumn => ListColumn; MasterListView => theListView; MasterListView_ItemClick => ListView_ItemClick.

Change Windows::UI::Core::WindowSizeChangedEventArgs to Microsoft::UI::Xaml::WindowSizeChangedEventArgs

Copy the XAML from MasterDetailPage.xaml over into ListDetailPage.xaml, including any necessary xaml attributes. Copy/replace all (match case and full word): MasterListViewItemTemplate => ListViewItemTemplate; MasterColumn => ListColumn; MasterListView => theListView; MasterListView_ItemClick => ListView_ItemClick.

In ListDetailPage.xaml.h, add `#include "ViewModels.ItemViewModel.h"`. Copy over rest of IDL for ListDetailPage, fixing up namespaces.

Copy code from MasterDetailPage.h and cpp over to ListDetailPage, fixing up namespace as you go. Change Windows::UI::Xaml to Microsoft::UI::Xaml, and add `#include "winrt/Microsoft.UI.Xaml.Media.Animation.h"` to pch.h. Don't need `#include <algorithm>` here.

In App.cpp add:
```cppwinrt
#include "ListDetailPage.xaml.h"
#include "DetailPage.xaml.h"
```

In MainWindow.xaml, replace the Grid with `<local:ListDetailPage/>`.

ISSUES: DetailPage::ShouldGoToWideState() calls Window::Current().Bounds().Width. Need an alternative for that. Also need to solve Window::Current().SizeChanged.

ISSUES: Looks like the TextBlock theme resources ListBodyTextBlockStyle and ListCaptionAltTextblockStyle are missing from WinUI 3. I instead set the style to BodyTextBlockStyle, and added a Gray foreground attribute and that looked identical to the app I was porting. But that's a workaround. Btw, folks can debug missing resource keys via debug output in the Output pane.

Remember to add `DetailPage::RegisterDependencyProperties();` to App.xaml.cpp.

Delete from DetailPage.xaml.h and .cpp (SystemNavigationManager is not for Win32 apps):
event_token m_backRequestedEventRegistrationToken{};
AND
// Register for hardware and software back request from the system
auto systemNavigationManager = SystemNavigationManager::GetForCurrentView();
m_backRequestedEventRegistrationToken =
    systemNavigationManager.BackRequested({ this, &DetailPage::DetailPage_BackRequested });
systemNavigationManager.AppViewBackButtonVisibility(AppViewBackButtonVisibility::Visible);

For the same reason, also delete DetailPage::OnNavigatedFrom.

TRYING THIS FOR instead of Window Current.
App header
static winrt::Microsoft::UI::Xaml::Window Window() { return window; }
private:
    static winrt::Microsoft::UI::Xaml::Window window;

cpp
winrt::Microsoft::UI::Xaml::Window App::window{ nullptr }; 

In DetailPage, change all Window.Current to stuff like `return App::Window().Bounds().Width >= 720.0;`.

In DetailPage, change the revoker to `winrt::Microsoft::UI::Xaml::Window::SizeChanged_revoker m_event_revoker;`. And `m_event_revoker = App::Window().SizeChanged(winrt::auto_revoke, { this, &DetailPage::Window_SizeChanged } )` and remove the code manually revoking.



===
## Add asset files

In Solution Explorer, in the Package project under **Images**, add a new folder (not a filter) named *SampleMedia*. Open the Package project folder in File Explorer and place [the four images](https://github.com/stevewhims/RetargetToReunionCppWin32/tree/master/RetargetToReunionCppWin32/RetargetToReunionCppWin32%20(Package)/Images/SampleMedia) in that folder.

Back in Solution Explorer, in the Package project, add the existing images to the **SampleMedia** folder.

## UI

VisualStateManager requires us to add a user control. To the Desktop project, add a new item of type **User Control (WinUI)**. Name it *VSMUC.xaml*.

From [RetargetToReunionCppUWP](https://github.com/stevewhims/RetargetToReunionCppUWP) in `MainPage.xaml`, copy the contents of the **Page** element. In `VSMUC.xaml`, replace the contents of the **Page** element with the content you copied.

* In the markup you pasted, change the name of the **Button** click event handler from **ClickHandler** to **myButton_Click**.
* In the **Image** **Source** urls, change `Assets` to `Images`.
* In the **TextBlock**, change `(UWP)` to `(Win32)`.

In `VSMUC.xaml.cpp`, delete the line of code inside **VSMUC::myButton_Click**.

In `MainWindow.xaml`, replace the contents of the **Window** element with the following markup.

```xaml
<!-- MainWindow.xaml -->
<local:VSMUC/>
```

In `MainWindow.xaml.cpp`, delete the line of code inside **MainWindow::myButton_Click**.

## Test

Before working on file activation, confirm that at this point the project builds and runs.

## Add a file type association

Open `Package.appxmanifest`. In **Declarations**, choose **File Type Associations**, and click **Add**. Set the following properties.

**Display name**: Hello File 2
**Name**: hellofile2
**File type**: .hi2

To register the file type association, build the app, launch it, and close it.

## Imperative code

### File type association feature

There's no need to add code to `App.h` nor `App.cpp`.

### Visual states

From [RetargetToReunionCppUWP](https://github.com/stevewhims/RetargetToReunionCppUWP) in `MainPage.h`, copy the declaration of **SizeChangedHandler**. Paste that into `VSMUC.xaml.h`.

In `MainPage.cpp`, copy the code you added to the constructor, and the definition of **SizeChangedHandler**. Paste that into `VSMUC.xaml.cpp`.

In the code you pasted, change the `Windows::` namespace to `Microsoft::`; and change `MainPage::` to `VSMUC::`.

### Button click handler

In `VSMUC.xaml.h` and `VSMUC.xaml.cpp`, change the return type of **myButton_Click** to **winrt::fire_and_forget**.

From [RetargetToReunionCppUWP](https://github.com/stevewhims/RetargetToReunionCppUWP) in `MainPage.cpp`, copy the definition of **ClickHandler**. Paste that into `VSMUC.xaml.cpp`.

To port that code, make the following changes.

* In `pch.h`, add the following.
  
  ```cppwinrt
  #include <Shobjidl.h>
  #include <winrt/Windows.Storage.h>
  #include <winrt/Windows.Storage.AccessCache.h>
  #include <winrt/Windows.Storage.Pickers.h>
  #include <winrt/Windows.Storage.Search.h>
  #include <winrt/Windows.UI.Core.h>
  #include <winrt/Windows.UI.Popups.h>
  ```
* In `App.xaml.h` and `App.xaml.cpp`, add the following.
  
  ```cppwinrt
  // App.xaml.h
  ...
  struct App : AppT<App>
  {
    ...
    static HWND WindowHandle() { return m_hWnd; }

  private:
    ...
    static HWND m_hWnd;
  };
  ...

  // App.xaml.cpp
  ...
  HWND App::m_hWnd = 0;
  ...
  struct __declspec(uuid("EECDBF0E-BAE9-4CB6-A68E-9598E1CB57BB")) __declspec(novtable)
    IWindowNative : ::IUnknown
  {
    virtual HRESULT __stdcall get_WindowHandle(HWND* hwnd) = 0;
  };
  ...
  void App::OnLaunched(LaunchActivatedEventArgs const&)
  {
    ...
    auto windowNative { window.as<::IWindowNative>() };
    HWND hWnd { 0 };
    windowNative->get_WindowHandle(&hWnd);
    App::m_hWnd = hWnd;
  }
  ...
  ```
* In `VSMUC.xaml.h` and `VSMUC.xaml.cpp`, add the following.

  ```cppwinrt
  // VSMUC.xaml.h
  ...
  #include <App.xaml.h>
  ...
  struct VSMUC : VSMUCT<VSMUC>
  {
    ...
    private:
      static void ParentDialogToWindow(Windows::Foundation::IInspectable const&);
  };
  ...

  // VSMUC.xaml.cpp
  ...
  void VSMUC::ParentDialogToWindow(IInspectable const& dialog)
  {
    // Helper function for non-CoreWindow.
    auto initializeWithWindow = dialog.as<::IInitializeWithWindow>();
    initializeWithWindow->Initialize(App::WindowHandle());
  }
  ...
  ```
* In `VSMUC.xaml.cpp`, immediately after declaring *showDialog*, add the line of code `VSMUC::ParentDialogToWindow(showDialog);`
* In `VSMUC.xaml.cpp`, immediately after declaring *folderPicker*, add the line of code `VSMUC::ParentDialogToWindow(folderPicker);`
* In `VSMUC.xaml.cpp`, comment out `co_await winrt::resume_foreground(this->Dispatcher());`. **TODO**: find out how to use **Microsoft::System::DispatcherQueue** instead.

## Test again

In your local **Documents** folder (or anywhere else, if you like), create a new **Text Document** file, and name it *MyHelloFile2.hi2*.

Double-click on the file, and confirm that the app launches and displays the UI. With the launched app still running, double-click on the file a second time, and confirm that a second instance is launched (multi-instancing).

Resize the window, and confirm that the correct visual states are applied.

Click the button, dismiss the message dialog, pick a folder containing files, and confirm that the first three filenames are displayed in the list box.
