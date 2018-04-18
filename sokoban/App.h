#pragma once

#include"my2d.h"
#include"clientSocket.h"
#include"DXInput.h"
#include"user.h"
#include"BitmapLoader.h"
#include"tools.h"
using cris::direction;
using cris::down;
using cris::up;
using cris::left;
using cris::right;
#define CLASSNAME _T("sokoban")


namespace cris {
	class App {
		
	public:
		//当前在哪个菜单
		int scene;
		std::vector<Scene> scenelist;

		HWND hedit;
		HWND hpasswd;
		HWND hlist;		
		HWND hwnd;
		HCURSOR arrow;
		HCURSOR hand;

		MySocket client;

		cris::my2d my2ddraw = { 0 };
		cris::DXInput input;
		cris::user u;
		cris::BitmapLoader d2;
		cris::BitmapLoader d12;
		cris::BitmapLoader d11;
		cris::BitmapLoader u2;
		cris::BitmapLoader u12;
		cris::BitmapLoader u11;
		cris::BitmapLoader l1;
		cris::BitmapLoader l2;
		cris::BitmapLoader r1;
		cris::BitmapLoader r2;

		cris::BitmapLoader target;
		cris::BitmapLoader wall;
		cris::BitmapLoader ground;
		cris::BitmapLoader box;
		cris::BitmapLoader bot;
	
		static LRESULT CALLBACK windProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		
		App(HINSTANCE hinst, int nCmdShow);
		void Run();
		void winLoop();
		~App();
	
	};

	class Scene 
	{
	public:
		int SceneNum;
		virtual void run() {}
	};
}
