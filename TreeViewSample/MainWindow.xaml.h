#pragma once

#include "MainWindow.g.h"
#include "FileSystemItem.h"
#include "FileSystemTemplateSelector.h"

WINRT_EXPORT namespace winrt
{
	// this is needed as there's no implicit conversion for binding back between an Object/IInspectable (Content) and a String/hstring
    // <Button Content="{x:Bind MyProperty, Mode=TwoWay}" .. />
	inline hstring to_hstring(IInspectable value)
	{
		auto pv = value.try_as<IPropertyValue>();
		if (pv && pv.Type() == PropertyType::String)
			return pv.GetString();

		return L"???";
	}
}

namespace winrt::TreeViewSample::implementation
{
	struct MainWindow : MainWindowT<MainWindow>
	{
		MainWindow()
		{
			// Xaml objects should not call InitializeComponent during construction.
			// See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
		}

		hstring MyProperty();
		void MyProperty(hstring value);

		void myButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
		void myButton1_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);

		event_token PropertyChanged(PropertyChangedEventHandler const& handler) { return _propertyChanged.add(handler); }
		void PropertyChanged(event_token token) { _propertyChanged.remove(token); }

	private:
		hstring _myProperty = L"Click me";
		event<PropertyChangedEventHandler> _propertyChanged;

		void RaisePropertyChanged(hstring propertyName)
		{
			_propertyChanged(*this, PropertyChangedEventArgs(propertyName));
		}
	};
}

namespace winrt::TreeViewSample::factory_implementation
{
	struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
	{
	};
}
