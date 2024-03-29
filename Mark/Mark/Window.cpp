#include "stdafx.h"
#include "Window.h"
#include <Mark/ResourceManager.h>
#include <Mark/Time.h>
#include <direct.h>

Mark::Window::Window(int width, int height, std::string windowName, int framerateLimit)
{
	m_wnd = new sf::RenderWindow(sf::VideoMode(width, height), windowName);
	m_wnd->setFramerateLimit(framerateLimit);
	m_wnd->setVerticalSyncEnabled(false);

	m_wndEvents = new std::unordered_map<sf::Event::EventType, std::vector<WEventDetails*>>();
	Mark::Time::Instance().Restart();
	//Load all managers

	//Creating directory for resources
	_mkdir("Resources");
	Mark::ResourceManager::Instance().Init("Resources/");
	
}


Mark::Window::~Window()
{
	m_wndEvents->clear();

	delete m_wndEvents;
	m_wndEvents = nullptr;
	delete m_wnd;
	m_wnd = nullptr;

	//Unload managers(Reverse order)
	Mark::ResourceManager::Instance().Unload();
}

void Mark::Window::Clear()
{
	m_wnd->clear(sf::Color(0.f, 0.f, 0.f, 0.f));
}
void Mark::Window::Draw(const sf::Drawable& target)
{
	m_wnd->draw(target);
}
void Mark::Window::Draw(const sf::Vertex* vertices, size_t vCount, sf::PrimitiveType type)
{
	m_wnd->draw(vertices, vCount, type);
}
void Mark::Window::Display()
{
	m_wnd->display();
	Mark::Time::Instance().Restart();
}

void Mark::Window::HandleWindowEvents()
{
	
	sf::Event e;
	while (m_wnd->pollEvent(e))
	{
		if (m_wndEvents->find(e.type) != m_wndEvents->end())
		{
			for (auto& i : m_wndEvents->at(e.type))
			{
				if (i->eventKey >= 0)
				{
					if (e.key.code == i->eventKey)
					{
						(this->*i->callback)();
					}
				}
				else
				{
					(this->*i->callback)();
				}
			}
		}
	}
	
	
}

bool Mark::Window::AddWindowEvent(const sf::Event::EventType& type, void (Window::*func)(),int key)
{

	if (m_wndEvents->find(type) != m_wndEvents->end())
	{
		m_wndEvents->at(type).push_back(new Mark::WEventDetails(key, func));
		
	}
	else
	{
		Mark::WEventDetails* details = new Mark::WEventDetails(key, func);
		std::vector<WEventDetails*> v;
		v.push_back(details);
		m_wndEvents->emplace(type, v);
	}

	return true;
}

bool Mark::Window::IsOpen() const
{
	return m_wnd->isOpen();
}

void Mark::Window::Close()
{
	m_wnd->close();
}
