#pragma once

#include "FileSystemTemplateSelector.g.h"

using namespace winrt::Windows::Foundation;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::TreeViewSample::implementation
{
	struct FileSystemTemplateSelector : FileSystemTemplateSelectorT<FileSystemTemplateSelector>
	{
		FileSystemTemplateSelector()
		{
		}

		// public properties (XAML usable)
		DataTemplate FileTemplate()
		{
			return _fileTemplate;
		}
		void FileTemplate(DataTemplate const& value)
		{
			_fileTemplate = value;
		}

		DataTemplate FolderTemplate()
		{
			return _folderTemplate;
		}
		void FolderTemplate(DataTemplate const& value)
		{
			_folderTemplate = value;
		}

		// IDataTemplateSelectorOverrides
		DataTemplate SelectTemplateCore(IInspectable const& item, DependencyObject const& /*container*/)
		{
			return SelectTemplateCore(item);
		}

		DataTemplate SelectTemplateCore(IInspectable const& item)
		{
			auto fs = item.as<TreeViewSample::FileSystemItem>();
			return fs.IsFolder() ? _folderTemplate : _fileTemplate;
		}

	private:
		DataTemplate _fileTemplate;
		DataTemplate _folderTemplate;
	};
}

namespace winrt::TreeViewSample::factory_implementation
{
	struct FileSystemTemplateSelector : FileSystemTemplateSelectorT<FileSystemTemplateSelector, implementation::FileSystemTemplateSelector>
	{
	};
}
