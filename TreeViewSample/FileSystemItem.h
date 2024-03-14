#pragma once

#include "FileSystemItem.g.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Foundation::Collections;
using namespace winrt::Microsoft::UI::Xaml;
using namespace winrt::Microsoft::UI::Xaml::Data;

namespace winrt::TreeViewSample::implementation
{
	struct FileSystemItem : FileSystemItemT<FileSystemItem>
	{
		FileSystemItem(hstring const& fullPath)
		{
			_isSelected = true;
			PWSTR out;
			check_hresult(PathAllocCanonicalize(fullPath.c_str(), PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH, &out));
			_isFolder = GetFileAttributes(out) & FILE_ATTRIBUTE_DIRECTORY;
			auto name = wcsrchr(out, L'\\');
			_name = name + 1;
			_fullPath = out;
			WINRT_IMPL_CoTaskMemFree(out);
		}

		// public properties (XAML usable)
		bool IsFolder() const { return _isFolder; }
		hstring FullPath() { return _fullPath; }
		hstring Name() { return _name; }

		bool IsSelected() const { return _isSelected; }
		void IsSelected(bool const& selected){
			if (selected == _isSelected)
				return;
			
			_isSelected = selected;
			RaisePropertyChanged(L"IsSelected");
		}

		IObservableVector<TreeViewSample::FileSystemItem> Children()
		{
			auto children{ single_threaded_observable_vector<TreeViewSample::FileSystemItem>() };
			if (_isFolder)
			{
				std::wostringstream pattern;
				auto fullPath = _fullPath.c_str();
				pattern << fullPath << L"\\*";

				WIN32_FIND_DATA data;
				auto handle = FindFirstFile(pattern.str().c_str(), &data);
				if (handle != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (!wcscmp(data.cFileName, L".") || !wcscmp(data.cFileName, L".."))
							continue;

						PWSTR out;
						if (FAILED(PathAllocCombine(fullPath, data.cFileName, PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH, &out)))
							continue;

						auto item = make<TreeViewSample::implementation::FileSystemItem>(out);
						WINRT_IMPL_CoTaskMemFree(out);
						children.Append(item);
					} while (FindNextFile(handle, &data));
					FindClose(handle);
				}
			}
			return children;
		}

		// public events
		event_token PropertyChanged(PropertyChangedEventHandler const& handler) { return _propertyChanged.add(handler); }
		void PropertyChanged(event_token token) { _propertyChanged.remove(token); }

	private:
		event<PropertyChangedEventHandler> _propertyChanged;
		hstring _fullPath;
		hstring _name;
		bool _isFolder;
		bool _isSelected;

		void RaisePropertyChanged(hstring propertyName)
		{
			_propertyChanged(*this, PropertyChangedEventArgs(propertyName));
		}
	};
}

namespace winrt::TreeViewSample::factory_implementation
{
	struct FileSystemItem : FileSystemItemT<FileSystemItem, implementation::FileSystemItem>
	{
	};
}
