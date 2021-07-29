# XamlListDetail&mdash;a Windows App SDK 0.8.1 port of the UWP XamlMasterDetail sample app

This repo contains the result of taking the [UWP XamlMasterDetail sample app](https://docs.microsoft.com/samples/microsoft/windows-universal-samples/xamlmasterdetail/), and porting it to the Windows App SDK 0.8.1. In the process, we change the name of the project to **XamlListDetail** (and we change the default namespace name to match).

I'll be referring to the UWP XamlMasterDetail sample app project as the *source project*, and I'll be referring to XamlListDetail as the *target project*.

## Install the Windows App SDK 0.8.1 VSIX

In [Visual Studio](https://visualstudio.microsoft.com/downloads/), click **Extensions** > **Manage Extensions** > **Installed**, search for *Project Reunion* (which is the code name of the Windows App SDK), and download the Project Reunion extension. Close and reopen Visual Studio, and follow the prompts to install the extension. For more info, see [0.8.1 is now available!](https://github.com/microsoft/WindowsAppSDK/releases/tag/v0.8.1).

## Create a new project

In Visual Studio, create a new project from the **Blank App, Packaged (WinUI 3 in Desktop)** project template. You can find that project template in the **Create a new project** dialog by choosing language: *C++*; platform: *Project Reunion*; project type: *Desktop* (which is referring to Win32 in this context) or *WinUI*. Name the project *XamlListDetail*, uncheck **Place solution and project in the same directory**, and target the most recent release (not preview) of the client operating system (19041 at time of writing).

You'll now see two projects in Solution Explorer&mdash;one is qualified as **(Desktop)**, and the other as **(Package)**. We'll be making changes only to the *Desktop* project in this walkthrough.

## Refactor the IDL into one file

The source project has all of its IDL in one file, named `Project.idl`. So we'll follow that pattern in the target project.

Add a new **Midl File (.idl)** item to the project. Name the new item `Project.idl`. Delete the default contents of `Project.idl`.

Move all the IDL from `MainWindow.idl` into `Project.idl`, save that, and then delete `MainWindow.idl`.

Remove the dummy property from `Project.idl` and from `MainWindow.xaml.h` and `.cpp`. Remove the event handler from those files, too. In `MainWindow.xaml`, replace the content with just `<Grid/>`. Confirm that you can build and run at this stage.

## Port **ItemViewModel** and **Item**

The main page is **MasterDetailPage**. But its XAML has dependencies on **MasterDetailApp::ViewModels::ItemViewModel** (which in turn depends on **MasterDetailApp::Data::Item**). So let's port those types before porting **MasterDetailPage**. In fact, we'll also need to port **ItemsDataSource** and **DetailPage** before **MasterDetailPage**.

Open `Project.idl` for editing. From `Project.idl` in the source project, copy the declarations of **Item** and **ItemViewModel** (including the namespaces) to the target project. Fix up the namespaces (change all occurrences of **MasterDetailApp** to **XamlListDetail**).

Save the file, and build the project. The build won't entirely succeed yet, but the build will generate the source code files (stubs) that we need to get started implementing **Item** and **ItemViewModel**.

Right-click the target project node and click **Open Folder in File Explorer**. This opens the project folder in File Explorer. There, copy the stub files `Data.Item.h` and `.cpp`, and `ViewModels.ItemViewModel.h` and `.cpp`, from the `\XamlListDetail\XamlListDetail\XamlListDetail\Generated Files\sources` folder and into the target project folder, which is `\XamlListDetail\XamlListDetail\XamlListDetail`. In **Solution Explorer**, with the project node selected, make sure **Show All Files** is toggled on. Right-click the stub files that you copied, and click **Include In Project**.

You'll see a `static_assert` at the top of the files you copied, which you'll need to remove. Confirm that you can build again (but don't run yet).

Now let's port the implementation of **MasterDetailApp.Data.Item** from `Item.cpp`. Copy the private data members from `Item.cpp` into `Data.Item.h`. And copy the method implementations from `Item.cpp` into the existing method stubs in `Data.Item.cpp`. In the member initializer list that you copied, change *id* to *itemId*. Be sure to delete `throw hresult_not_implemented();` from the constructor.

Follow similar steps to port the implementation of **MasterDetailApp.ItemViewModel.Item** from `ItemViewModel.cpp` to `ViewModels.ItemViewModel.h` and `.cpp`. In this case there's a non-default constructor that we didn't declare in the IDL, so copy that across also. Fix up the namespaces (change all occurrences of **MasterDetailApp** to **XamlListDetail**).

Add `#include <winrt/Windows.Globalization.DateTimeFormatting.h>` to pch.h, and confirm that you can build (but don't run yet).

## Port **ItemsDataSource**

**ItemsDataSource** is a helper class; there's no need for IDL nor XAML.

Copy the `ItemsDataSource.h` file into the target project folder, and include it in the project. Fix up the namespaces (change all occurrences of **MasterDetailApp** to **XamlListDetail**).

We'll also need to port across ItemsDataSource. No need for XAML here, just copy `ItemsDataSource.h` across, and fix up the namespace.

## Begin porting **DetailPage**

In this section we'll begin porting **DetailPage**; and we'll be finishing that up in a later section.

Add a new item to the target project (**WinUI** > **Blank Page (WinUI 3)**) named `DetailPage.xaml`.

Move the IDL out of `DetailPage.idl` and into `Project.idl`; delete `DetailPage.idl`. Remove the dummy property from the IDL you copied, and add the two IDL properties of **DetailPage** from the source project.

From `pch.h`, copy over the following includes.

```cppwinrt
#include <winrt/Microsoft.UI.Xaml.Media.Animation.h>
#include <winrt/MasterDetailApp.h>
#include <winrt/MasterDetailApp.Data.h>
#include <winrt/MasterDetailApp.ViewModels.h>
#include <ItemsDataSource.h>
```

Copy the contents of `DetailPage.xaml`, `DetailPage.h`, and `DetailPage.cpp` from the source project into the target project. Make the following find/replacements (match case and whole word) in the contents of all of the source code you just copied.

Changes for improved naming:

* MasterDetailApp => XamlListDetail

Changes due to the difference between UWP and Reunion/WinUI/Win32 platform and tooling:

* `#include "DetailPage.h"` => `#include "DetailPage.xaml.h"`
* `Windows::UI::Xaml` => `Microsoft::UI::Xaml`
* `Windows.UI.Xaml` => `Microsoft.UI.Xaml`
* `Windows::UI::Core::WindowSizeChangedEventArgs` => `Microsoft::UI::Xaml::WindowSizeChangedEventArgs`

Also make the following edits.

* In the definition of **DetailPage::Window_SizeChanged**, qualify the second parameter as type **Microsoft::UI::Xaml::WindowSizeChangedEventArgs**.
* Remove code in **DetailPage::OnNavigatedTo** that makes use of the **SystemNavigationManager** class. **SystemNavigationManager** isn't relevant for Win32 (desktop) apps.
* Delete **DetailPage::OnNavigatedFrom**,  **DetailPage::DetailPage_BackRequested**, and **DetailPage::OnBackRequested**.

Confirm that you can build (but don't run yet).

## Port **ListDetailPage**

Add a new item to the target project (**WinUI** > **Blank Page (WinUI 3)**) named `ListDetailPage.xaml`.

Move the IDL out of `ListDetailPage.idl` and into `Project.idl`; delete `ListDetailPage.idl`. Remove the dummy property from the IDL you copied, and add the IDL property of **ListDetailPage** from the source project.

From `pch.h`, copy over the following include.

```cppwinrt
#include <winrt/Windows.UI.Core.h>
```

Copy the contents of `ListDetailPage.xaml`, `ListDetailPage.h`, and `ListDetailPage.cpp` from the source project into the target project. Make the following find/replacements (match case and whole word) in the contents of all of the source code you just copied.

Changes for improved naming:

* MasterDetailPage => ListDetailPage
* MasterDetailPageT => ListDetailPageT
* MasterDetailApp => XamlListDetail
* MasterListViewItemTemplate => ListViewItemTemplate
* MasterColumn => ListColumn
* MasterListView => ListView
* MasterListView_ItemClick => ListView_ItemClick

Changes due to the difference between UWP and Reunion/WinUI/Win32 platform and tooling:

* `#include "ListDetailPage.h"` => `#include "ListDetailPage.xaml.h"`
* `Windows::UI::Xaml` => `Microsoft::UI::Xaml`
* `Windows.UI.Xaml` => `Microsoft.UI.Xaml`

Also make the following edits.

* In the definition of **ListDetailPage::Window_SizeChanged**, qualify the second parameter as type **Microsoft::UI::Xaml::WindowSizeChangedEventArgs**.
* Delete code in **ListDetailPage::OnNavigatedTo** that makes use of the **SystemNavigationManager** class. **SystemNavigationManager** isn't relevant for Win32 (desktop) apps.
* Delete **ListDetailPage::OnNavigatedFrom**,  **ListDetailPage::DetailPage_BackRequested**, and **ListDetailPage::OnBackRequested**.
* Delete the *m_backRequestedEventRegistrationToken* data member.

Confirm that you can build (but don't run yet).

## Port **App**

In `App.xaml` in the source project, you'll find a **<ResourceDictionary.ThemeDictionaries>** element. Copy that element (the opening and closing tags, and the inner XML) and paste it into the same place inside `App.xaml` in the target project. This theme **ResourceDictionary** contains styles that the app depends on.

As part of the port, we need to add a few pieces to the App class (`App.xaml.h` and `.cpp`). Let's start by porting over the code that sets up the app ready for the navigation that the other pages in the app will be doing.

Copy over the declaration and definition of the **CreateRootFrame** and **OnNavigationFailed** methods. Also copy over the code inside the **OnLaunched** event handler, except for the line that activates the window, since you already have that (paste the code in between the two lines of code that area already in **OnLaunched**). As the last line of the constructor, add `DetailPage::RegisterDependencyProperties();`.

Delete **OnSuspending** from `App.xaml.h`.

So that the **App** class can expose the main window to the rest of the app, make the following additions and edits.

```cppwinrt
// App.xaml.h
...
#include "DetailPage.xaml.h"
...
struct App : AppT<App>
{
    ...
    static winrt::Microsoft::UI::Xaml::Window Window() { return window; }

private:
    static winrt::Microsoft::UI::Xaml::Window window;
};
```

Finish porting the code you copied by making these changes:

* MasterDetailApp => XamlListDetail
* MasterDetailPage => ListDetailPage
* `Windows::UI::Xaml` => `Microsoft::UI::Xaml`
* `Window::Current()` => `window`

Confirm that you can build (but don't run yet).

## Finalize **DetailPage**

In `DetailPage.xaml.h`, remove the private *m_sizeChangedEventRegistrationToken* field, and replace it with `winrt::Microsoft::UI::Xaml::Window::SizeChanged_revoker m_event_revoker;`.

In `DetailPage.xaml.cpp`, add `#include <App.xaml.h>`.

Also in `DetailPage.xaml.cpp`, find `m_sizeChangedEventRegistrationToken = Window::Current().SizeChanged({ this, &DetailPage::Window_SizeChanged });`, and replace that with `m_event_revoker = App::Window().SizeChanged(winrt::auto_revoke, { this, &DetailPage::Window_SizeChanged } );`.

Apart from the line of code mentioned above, delete every other mention of `SizeChanged` in `DetailPage.xaml.cpp`. Also remove every mention of *m_sizeChangedEventRegistrationToken*.

Delete the **PageRoot_Unloaded** method from the `DetailPage.xaml`, `.h`, and `.cpp` files.

In **ShouldGoToWideState**, change `Window::Current().Bounds().Width` to `App::Window().Bounds().Width`.

Confirm that you can build (but don't run yet).

## Finalize **MainWindow**

In `MainWindow.xaml`, replace the **Grid** with `<local:ListDetailPage/>`.

## Test

Now you can build, run, and test. Resize the window, and confirm that the correct visual states are applied, and that selection and navigation are functioning as expected.
