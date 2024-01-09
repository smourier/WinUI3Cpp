#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::TreeViewSample::implementation
{
	int32_t MainWindow::MyProperty()
	{
		throw hresult_not_implemented();
	}

	void MainWindow::MyProperty(int32_t /* value */)
	{
		throw hresult_not_implemented();
	}

	void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
	{
		auto source{ single_threaded_observable_vector<TreeViewSample::FileSystemItem>() };
		
		wchar_t path[1024];
		GetModuleFileName(nullptr, path, _countof(path));
		PWSTR out;
		PathAllocCombine(path, L"..\\..", 0, &out);
		auto root = make<TreeViewSample::implementation::FileSystemItem>(out);
		WINRT_IMPL_CoTaskMemFree(out);
		source.Append(root);

		myTreeView().ItemsSource(source);
	}
}
