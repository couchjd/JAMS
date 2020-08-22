#include "Game.h"

#define ONE_SECOND 10000000

Game::Game() {
}


Game::~Game() {
	stopVideo();
}

void Game::initialize() {
	m_main_window.create(sf::VideoMode(800, 600), "JAM 01");
	m_player.setFillColor(sf::Color::Green);
	m_player.setRadius(10.0);

	IGraphBuilder*	m_pGraph = NULL;
	IMediaControl*	m_pControl = NULL;
	IMediaEvent*	m_pEvent = NULL;
	IMediaSeeking*	m_pSeek = NULL;

}

void Game::playVideo(LPCWSTR filename) {
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr)) {
		printf("ERROR - Could not initialize COM library");
		return;
	}

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder, (void**)&m_pGraph);

	if (FAILED(hr)) {
		printf("ERROR - Could not create the Filter Graph Manager.");
		return;
	}

	hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pControl);
	hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void**)&m_pEvent);
	hr = m_pGraph->QueryInterface(IID_IMediaSeeking, (void**)&m_pSeek);

	hr = m_pGraph->RenderFile(filename, NULL);

	if (SUCCEEDED(hr)) {
		hr = m_pControl->Run();
		//if(SUCCEEDED(hr)) {
		//	long evCode;
		//	m_pEvent->WaitForCompletion(INFINITE, &evCode);
		//}
	}
}

void Game::stopVideo() {
	m_pControl->Stop();
	if(m_pControl)
		m_pControl->Release();
	if(m_pEvent)
		m_pEvent->Release();
	if(m_pGraph)
		m_pGraph->Release();
	CoUninitialize();
}

void Game::handleVideoInput() {
	FILTER_STATE fs;
	HRESULT hr = m_pControl->GetState(INFINITE, (OAFilterState*)&fs);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (fs == State_Stopped || fs == State_Paused) {
			m_pControl->Run();
		}
		else {
			m_pControl->Pause();
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		REFERENCE_TIME rtNow = 5 * ONE_SECOND;
		hr = m_pSeek->SetPositions(
			&rtNow, AM_SEEKING_RelativePositioning,
			NULL, AM_SEEKING_NoPositioning);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		REFERENCE_TIME rtNow = 0;
		m_pSeek->GetCurrentPosition(&rtNow);

		rtNow -= 5 * ONE_SECOND;

		hr = m_pSeek->SetPositions(
			&rtNow, AM_SEEKING_AbsolutePositioning,
			NULL, AM_SEEKING_NoPositioning);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		stopVideo();
	}
}

void Game::run() {

	while(m_main_window.isOpen()) {
		sf::Event event;
		
		while(m_main_window.pollEvent(event)) {
			if(event.type == sf::Event::Closed) {
				m_main_window.close();
			}
			
			if (event.type == sf::Event::KeyPressed) {

				// Only do this if there is no video currently playing
				if (!m_pControl && sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
					playVideo(L"C:\\Users\\jeff\\Documents\\test_vid.avi");
				}

				// Only do this if there is a video playing.
				else if(m_pControl) {
					handleVideoInput();
				}
			}
		}

		drawEverything();
	}

}

void Game::drawEverything() {
	m_main_window.clear(sf::Color::Black);
	m_main_window.draw(m_player);
	m_main_window.display();
}
