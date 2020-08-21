#include "Game.h"

#include <dshow.h>

#define ONE_SECOND 10000000

Game::Game() {
}


Game::~Game() {
}

void Game::initialize() {
	m_main_window.create(sf::VideoMode(800, 600), "JAM 01");
	m_player.setFillColor(sf::Color::Green);
	m_player.setRadius(10.0);
}

void Game::run() {
	IGraphBuilder*	pGraph = NULL;
	IMediaControl*	pControl = NULL;
	IMediaEvent*	pEvent = NULL;
	IMediaSeeking*	pSeek = NULL;

	HRESULT hr = CoInitialize(NULL);
	if(FAILED(hr)) {
		printf("ERROR - Could not initialize COM library");
		return;
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
						  IID_IGraphBuilder, (void**)&pGraph);

	if(FAILED(hr)) {
		printf("ERROR - Could not create the Filter Graph Manager.");
		return;
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);
	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void**)&pSeek);

	hr = pGraph->RenderFile(L"C:\\Users\\couch\\Videos\\test_vid.avi", NULL);

	if(SUCCEEDED(hr)) {
		hr = pControl->Run();
		//if(SUCCEEDED(hr)) {
		//	long evCode;
		//	pEvent->WaitForCompletion(INFINITE, &evCode);
		//}
	}



	while(m_main_window.isOpen()) {
		sf::Event event;
		FILTER_STATE fs;
		hr = pControl->GetState(INFINITE, (OAFilterState*)&fs);

		while(m_main_window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				m_main_window.close();
			}
			else if(event.type == sf::Event::KeyPressed) {
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
					if(fs == State_Stopped || fs == State_Paused) {
						pControl->Run();
					}
					else {
						pControl->Pause();
					}
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
					REFERENCE_TIME rtNow = 5 * ONE_SECOND;
					hr = pSeek->SetPositions(
						&rtNow, AM_SEEKING_RelativePositioning,
						NULL, AM_SEEKING_NoPositioning);
				}
				else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
					REFERENCE_TIME rtNow = 0;
					pSeek->GetCurrentPosition(&rtNow);

					rtNow -= 5 * ONE_SECOND;

					hr = pSeek->SetPositions(
						&rtNow, AM_SEEKING_AbsolutePositioning,
						NULL, AM_SEEKING_NoPositioning);
				}
			}			
		}

		drawEverything();
	}

	pControl->Release();
	pEvent->Release();
	pGraph->Release();
	CoUninitialize();
}

void Game::drawEverything() {
	m_main_window.clear(sf::Color::Black);
	m_main_window.draw(m_player);
	m_main_window.display();
}
