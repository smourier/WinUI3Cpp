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
	hstring MainWindow::MyProperty()
	{
		return _myProperty;
	}

	void MainWindow::MyProperty(hstring value)
	{
		if (_myProperty == value)
			return;

		_myProperty = value;
		RaisePropertyChanged(L"MyProperty");
	}

	void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
	{
		auto source{ single_threaded_observable_vector<TreeViewSample::FileSystemItem>() };

		wchar_t path[1024];
		GetModuleFileName(nullptr, path, _countof(path));
		PWSTR rootPath;
		PathAllocCombine(path, L"..\\..", 0, &rootPath);
		auto root = make<TreeViewSample::implementation::FileSystemItem>(rootPath);
		WINRT_IMPL_CoTaskMemFree(rootPath);
		source.Append(root);

		myTreeView().ItemsSource(source);
	}

	void MainWindow::myButton1_Click(IInspectable const&, RoutedEventArgs const&)
	{
		MyProperty(L"Changed!");
	}

	void MainWindow::myButton2_Click(IInspectable const&, RoutedEventArgs const&)
	{
		auto source{ single_threaded_observable_vector<TreeViewSample::FileSystemItem>() };
		wchar_t path[1024];
		GetModuleFileName(nullptr, path, _countof(path));
		PWSTR rootPath;
		PathAllocCombine(path, L"..\\..", 0, &rootPath);
		auto root = make<TreeViewSample::implementation::FileSystemItem>(rootPath);
		WINRT_IMPL_CoTaskMemFree(rootPath);
		for (auto const& child : root.Children())
		{
			source.Append(child);
		}
		myListView().ItemsSource(source);

		DispatcherQueue().TryEnqueue(Microsoft::UI::Dispatching::DispatcherQueuePriority::Low, [source]()
		{
			for (auto const& child : source)
			{
				child.IsSelected(child.Name().c_str()[0] == 'T');
			}
		});
	}
}

