#include "LinuxInput.h"

#if LINUX
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <asm-generic/ioctl.h>

#define die(str, args...) do { \
        perror(str); \
        exit(EXIT_FAILURE); \
    } while(0)

#endif

LinuxInput::LinuxInput()
{
#if LINUX
	fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
	if (fd < 0) {
		printf("Failed to open uinput file");
		exit(EXIT_FAILURE);
	}

	

	if (ioctl(fd, UI_SET_EVBIT, EV_SYN) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_KEYBIT, SYN_REPORT) < 0)
		die("error: ioctl");

	if (ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH) < 0)
		die("error: ioctl");

	if (ioctl(fd, UI_SET_EVBIT, EV_ABS) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_SLOT) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_X) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_Y) < 0)
		die("error: ioctl");
	if (ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID) < 0)
		die("error: ioctl");
	
	memset(&uidev, 0, sizeof(uidev));
	snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-sample");
	uidev.id.bustype = BUS_USB;
	uidev.id.vendor = 0x1;
	uidev.id.product = 0x1;
	uidev.id.version = 1;

	if (write(fd, &uidev, sizeof(uidev)) < 0)
		die("error: write");

	if (ioctl(fd, UI_DEV_CREATE) < 0)
		die("error: ioctl");

	sleep(2);

	inputObjects = new InputObject[ioCount];
	for (int i = 0; i < ioCount; ++i)
	{
		inputObjects[i].active = false;
		inputObjects[i].ID = -1;
		inputObjects[i].x = -1;
		inputObjects[i].y = -1;
	}
#endif
}


LinuxInput::~LinuxInput()
{
#if LINUX
	if (ioctl(fd, UI_DEV_DESTROY) < 0)
		die("error: ioctl");

	close(fd);
#endif
}

void LinuxInput::Update()
{
#if LINUX
	bool updatedInfo = false;

	for (int i = 0; i < ioCount; ++i)
	{
		if (inputObjects[i].active)
		{
			memset(&ev, 0, sizeof(struct input_event));
			ev.type = EV_ABS;
			ev.code = ABS_MT_SLOT;
			ev.value = i + 1;
			if (write(fd, &ev, sizeof(struct input_event)) < 0)
				die("error: write");

			memset(&ev, 0, sizeof(struct input_event));
			ev.type = EV_ABS;
			ev.code = ABS_MT_TRACKING_ID;
			ev.value = inputObjects[i].ID;
			if (write(fd, &ev, sizeof(struct input_event)) < 0)
				die("error: write");

			memset(&ev, 0, sizeof(struct input_event));
			ev.type = EV_ABS;
			ev.code = ABS_MT_POSITION_X;
			ev.value = inputObjects[i].x;
			if (write(fd, &ev, sizeof(struct input_event)) < 0)
				die("error: write");

			memset(&ev, 0, sizeof(struct input_event));
			ev.type = EV_ABS;
			ev.code = ABS_MT_POSITION_Y;
			ev.value = inputObjects[i].y;
			if (write(fd, &ev, sizeof(struct input_event)) < 0)
				die("error: write");

			memset(&ev, 0, sizeof(struct input_event));
			ev.type = EV_KEY;
			ev.code = BTN_TOUCH;
			ev.value = inputObjects[i].ID < 0 ? KEY_UP : KEY_DOWN;
			if (write(fd, &ev, sizeof(struct input_event)) < 0)
				die("error: write");
			
			if (inputObjects[i].ID < 0)
			{
				inputObjects[i].active = false;
			}

			updatedInfo = true;
		}
	}

	if (updatedInfo)
	{
		memset(&ev, 0, sizeof(struct input_event));
		ev.type = EV_SYN;
		ev.code = SYN_REPORT;
		ev.value = 0;
		if (write(fd, &ev, sizeof(struct input_event)) < 0)
			die("error: write");

		usleep(15000);
		sleep(0.5f);
	}
#endif
}

bool LinuxInput::StartTouch(int ID, int x, int y)
{
#if LINUX
	for (int i = 0; i < ioCount; ++i)
	{
		if (!inputObjects[i].active)
		{
			inputObjects[i].active = true;
			inputObjects[i].ID = ID;
			inputObjects[i].x = x;
			inputObjects[i].y = y;
			return true;
		}
	}

	return false;
#else
	return true;
#endif
}

void LinuxInput::EndTouch(int ID)
{
#if LINUX
	for (int i = 0; i < ioCount; ++i)
	{
		if (inputObjects[i].ID == ID && inputObjects[i].active)
		{
			inputObjects[i].ID = -1;
			return;
		}
	}
#endif
}

void LinuxInput::MoveTouch(int ID, int x, int y)
{
#if LINUX
	for (int i = 0; i < ioCount; ++i)
	{
		if (inputObjects[i].ID == ID && inputObjects[i].active)
		{
			inputObjects[i].x = x;
			inputObjects[i].y = y;
			return;
		}
	}
#endif
}

