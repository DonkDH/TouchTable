#pragma once

#define LINUX 1

#if LINUX
#include <linux/input.h>
#include <linux/uinput.h>

struct InputObject
{
	bool active;
	int ID;
	uint x;
	uint y;
};
#endif

class LinuxInput
{
public:
	LinuxInput();
	~LinuxInput();

	void Update();
	bool StartTouch( int ID, int x, int y );
	void EndTouch(int ID);
	void MoveTouch( int ID, int x, int y );


private:
#if LINUX
	int fd;
	struct input_event ev;
	struct uinput_user_dev uidev;

	InputObject* inputObjects;
	const uint ioCount = 10;
#endif
};