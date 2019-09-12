/**
 * Gamepad Input Library
 * Sean Middleditch
 * Copyright (C) 2010  Sean Middleditch
 * LICENSE: MIT/X
 */

#include <math.h>
#include <string.h>
#include <errno.h>
#include <malloc.h>

#define GAMEPAD_EXPORT 1
#include "gamepad.h"

/* Platform-specific includes */
#if defined(_WIN32)
#	define WIN32_LEAN_AND_MEAN 1
#	undef UNICODE
#	include "windows.h"
#	include "xinput.h"
#	pragma comment(lib, "XINPUT9_1_0.lib")
#elif defined(__linux__)
#	include <linux/joystick.h>
#	include <stdio.h>
#	include <fcntl.h>
#	include <unistd.h>
#	include <libudev.h>
#else
#	error "Unknown platform in gamepad.c"
#endif

#define BUTTON_TO_FLAG(b) (1 << (b))

/* Axis information */
typedef struct GAMEPAD_AXIS GAMEPAD_AXIS;
struct GAMEPAD_AXIS {
	int x, y;
	float nx, ny;
	float length;
	float angle;
	GAMEPAD_STICKDIR dirLast, dirCurrent;
};

/* Trigger value information */
typedef struct GAMEPAD_TRIGINFO GAMEPAD_TRIGINFO;
struct GAMEPAD_TRIGINFO {
	int value;
	float length;
	GAMEPAD_BOOL pressedLast, pressedCurrent;
};

/* Structure for state of a particular gamepad */
typedef struct GAMEPAD_STATE GAMEPAD_STATE;
struct GAMEPAD_STATE {
	GAMEPAD_AXIS stick[STICK_COUNT];
	GAMEPAD_TRIGINFO trigger[TRIGGER_COUNT];
	int bLast, bCurrent, flags;
#if defined(__linux__)
	char* device;
	int fd;
	int effect;
#endif
};

/* State of the four gamepads */
static GAMEPAD_STATE STATE[4];

/* Note whether a gamepad is currently connected */
#define FLAG_CONNECTED	(1<<0)
#define FLAG_RUMBLE		(1<<1)

/* Prototypes for utility functions */
static void GamepadResetState		(GAMEPAD_DEVICE gamepad);
static void GamepadUpdateCommon		(void);
static void GamepadUpdateDevice		(GAMEPAD_DEVICE gamepad);
static void GamepadUpdateStick		(GAMEPAD_AXIS* axis, float deadzone);
static void GamepadUpdateTrigger	(GAMEPAD_TRIGINFO* trig);

/* Various values of PI */
#define PI_1_4	0.78539816339744f
#define PI_1_2	1.57079632679489f
#define PI_3_4	2.35619449019234f
#define PI		3.14159265358979f

/* Platform-specific implementation code */
#if defined(_WIN32)

void GamepadInit(void) {
	int i;
	for (i = 0; i != GAMEPAD_COUNT; ++i) {
		STATE[i].flags = 0;
	}
}

void GamepadUpdate(void) {
	GamepadUpdateCommon();
}

static void GamepadUpdateDevice(GAMEPAD_DEVICE gamepad) {
	XINPUT_STATE xs;
	if (XInputGetState(gamepad, &xs) == 0) {
		/* reset if the device was not already connected */
		if ((STATE[gamepad].flags & FLAG_CONNECTED) == 0) {
			GamepadResetState(gamepad);
		}

		/* mark that we are connected w/ rumble support */
		STATE[gamepad].flags |= FLAG_CONNECTED|FLAG_RUMBLE;

		/* update state */
		STATE[gamepad].bCurrent = xs.Gamepad.wButtons;
		STATE[gamepad].trigger[TRIGGER_LEFT].value = xs.Gamepad.bLeftTrigger;
		STATE[gamepad].trigger[TRIGGER_RIGHT].value = xs.Gamepad.bRightTrigger;
		STATE[gamepad].stick[STICK_LEFT].x = xs.Gamepad.sThumbLX;
		STATE[gamepad].stick[STICK_LEFT].y = xs.Gamepad.sThumbLY;
		STATE[gamepad].stick[STICK_RIGHT].x = xs.Gamepad.sThumbRX;
		STATE[gamepad].stick[STICK_RIGHT].y = xs.Gamepad.sThumbRY;
	} else {
		/* disconnected */
		STATE[gamepad].flags &= ~FLAG_CONNECTED;
	}
}

void GamepadShutdown(void) {
	/* no Win32 shutdown required */
}

void GamepadSetRumble(GAMEPAD_DEVICE gamepad, float left, float right) {
	if ((STATE[gamepad].flags & FLAG_RUMBLE) != 0) {
		XINPUT_VIBRATION vib;
		ZeroMemory(&vib, sizeof(vib));
		vib.wLeftMotorSpeed = (WORD)(left * 65535);
		vib.wRightMotorSpeed = (WORD)(right * 65535);
		XInputSetState(gamepad, &vib);
	}
}

#elif defined(__linux__)

/* UDev handles */
static struct udev* UDEV = NULL;
static struct udev_monitor* MON = NULL;

static void GamepadAddDevice(const char* devPath);
static void GamepadRemoveDevice(const char* devPath);

/* Helper to add a new device */
static void GamepadAddDevice(const char* devPath) {
	int i;

	/* try to find a free controller */
	for (i = 0; i != GAMEPAD_COUNT; ++i) {
		if ((STATE[i].flags & FLAG_CONNECTED) == 0) {
			break;
		}
	}
	if (i == GAMEPAD_COUNT) {
		return;
	}

	/* copy the device path */
	STATE[i].device = strdup(devPath);
	if (STATE[i].device == NULL) {
		return;
	}

	/* reset device state */
	GamepadResetState((GAMEPAD_DEVICE)i);

	/* attempt to open the device in read-write mode, which we need fo rumble */
	STATE[i].fd = open(STATE[i].device, O_RDWR|O_NONBLOCK);
	if (STATE[i].fd != -1) {
		STATE[i].flags = FLAG_CONNECTED|FLAG_RUMBLE;
		return;
	}

	/* attempt to open in read-only mode if access was denied */
	if (errno == EACCES) {
		STATE[i].fd = open(STATE[i].device, O_RDONLY|O_NONBLOCK);
		if (STATE[i].fd != -1) {
			STATE[i].flags = FLAG_CONNECTED;
			return;
		}
	}

	/* could not open the device at all */
	free(STATE[i].device);
	STATE[i].device = NULL;
}

/* Helper to remove a device */
static void GamepadRemoveDevice(const char* devPath) {
	int i;
	for (i = 0; i != GAMEPAD_COUNT; ++i) {
		if (STATE[i].device != NULL && strcmp(STATE[i].device, devPath) == 0) {
			if (STATE[i].fd != -1) {
				close(STATE[i].fd);
				STATE[i].fd = -1;
			}
			free(STATE[i].device);
			STATE[i].device = 0;
			STATE[i].flags = 0;
			break;
		}
	}
}

void GamepadInit(void) {
	struct udev_list_entry* devices;
	struct udev_list_entry* item;
	struct udev_enumerate* enu;
	int i;

	/* initialize connection state */
	for (i = 0; i != GAMEPAD_COUNT; ++i) {
		STATE[i].flags = 0;
		STATE[i].fd = STATE[i].effect = -1;
	}

	/* open the udev handle */
	UDEV = udev_new();
	if (UDEV == NULL) {
		/* FIXME: flag error? */
		return;
	}
	
	/* open monitoring device (safe to fail) */
	MON = udev_monitor_new_from_netlink(UDEV, "udev");
	/* FIXME: flag error if hot-plugging can't be supported? */
	if (MON != NULL) {
		udev_monitor_enable_receiving(MON);
		udev_monitor_filter_add_match_subsystem_devtype(MON, "input", NULL);
	}

	/* enumerate joypad devices */
	enu = udev_enumerate_new(UDEV);
	udev_enumerate_add_match_subsystem(enu, "input");
	udev_enumerate_scan_devices(enu);
	devices = udev_enumerate_get_list_entry(enu);

	udev_list_entry_foreach(item, devices) {
		const char* name;
		const char* sysPath;
		const char* devPath;
		struct udev_device* dev;

		name = udev_list_entry_get_name(item);
		dev = udev_device_new_from_syspath(UDEV, name);
		sysPath = udev_device_get_syspath(dev);
		devPath = udev_device_get_devnode(dev);

		if (sysPath != NULL && devPath != NULL && strstr(sysPath, "/js") != 0) {
			GamepadAddDevice(devPath);
		}

		udev_device_unref(dev);
	}

	/* cleanup */
	udev_enumerate_unref(enu);
}

void GamepadUpdate(void) {
	if (MON != NULL) {
		fd_set r;
		struct timeval tv;
		int fd = udev_monitor_get_fd(MON);

		/* set up a poll on the udev device */
		FD_ZERO(&r);
		FD_SET(fd, &r);

		tv.tv_sec = 0;
		tv.tv_usec = 0;

		select(fd + 1, &r, 0, 0, &tv);

		/* test if we have a device change */
		if (FD_ISSET(fd, &r)) {
			struct udev_device* dev = udev_monitor_receive_device(MON);
			if (dev) {
				const char* devNode = udev_device_get_devnode(dev);
				const char* sysPath = udev_device_get_syspath(dev);
				const char* action = udev_device_get_action(dev);
				sysPath = udev_device_get_syspath(dev);
				action = udev_device_get_action(dev);

				if (strstr(sysPath, "/js") != 0) {
					if (strcmp(action, "remove") == 0) {
						GamepadRemoveDevice(devNode);
					} else if (strcmp(action, "add") == 0) {
						GamepadAddDevice(devNode);
					}
				}

				udev_device_unref(dev);
			}
		}
	}

	GamepadUpdateCommon();
}

static void GamepadUpdateDevice(GAMEPAD_DEVICE gamepad) {
	if (STATE[gamepad].flags & FLAG_CONNECTED) {
		struct js_event je;
		while (read(STATE[gamepad].fd, &je, sizeof(je)) > 0) {
			int button;
			switch (je.type) {
			case JS_EVENT_BUTTON:
				/* determine which button the event is for */
				switch (je.number) {
				case 0: button = BUTTON_A; break;
				case 1: button = BUTTON_B; break;
				case 2: button = BUTTON_X; break;
				case 3: button = BUTTON_Y; break;
				case 4: button = BUTTON_LEFT_SHOULDER; break;
				case 5: button = BUTTON_RIGHT_SHOULDER; break;
				case 6: button = BUTTON_BACK; break;
				case 7: button = BUTTON_START; break;
				case 8: button = 0; break; /* XBOX button  */
				case 9: button = BUTTON_LEFT_THUMB; break;
				case 10: button = BUTTON_RIGHT_THUMB; break;
				default: button = 0; break;
				}

				/* set or unset the button */
				if (je.value) {
					STATE[gamepad].bCurrent |= BUTTON_TO_FLAG(button);
				} else {
					STATE[gamepad].bCurrent ^= BUTTON_TO_FLAG(button);
				}
					
				break;
			case JS_EVENT_AXIS:
				/* normalize and store the axis */
				switch (je.number) {
				case 0:	STATE[gamepad].stick[STICK_LEFT].x = je.value; break;
				case 1:	STATE[gamepad].stick[STICK_LEFT].y = -je.value; break;
				case 2:	STATE[gamepad].trigger[TRIGGER_LEFT].value = (je.value + 32768) >> 8; break;
				case 3:	STATE[gamepad].stick[STICK_RIGHT].x = je.value; break;
				case 4:	STATE[gamepad].stick[STICK_RIGHT].y = -je.value; break;
				case 5:	STATE[gamepad].trigger[TRIGGER_RIGHT].value = (je.value + 32768) >> 8; break;
				case 6:
					if (je.value == -32767) {
						STATE[gamepad].bCurrent |= BUTTON_TO_FLAG(BUTTON_DPAD_LEFT);
						STATE[gamepad].bCurrent &= ~BUTTON_TO_FLAG(BUTTON_DPAD_RIGHT);
					} else if (je.value == 32767) {
						STATE[gamepad].bCurrent |= BUTTON_TO_FLAG(BUTTON_DPAD_RIGHT);
						STATE[gamepad].bCurrent &= ~BUTTON_TO_FLAG(BUTTON_DPAD_LEFT);
					} else {
						STATE[gamepad].bCurrent &= ~BUTTON_TO_FLAG(BUTTON_DPAD_LEFT) & ~BUTTON_TO_FLAG(BUTTON_DPAD_RIGHT);
					}
					break;
				case 7:
					if (je.value == -32767) {
						STATE[gamepad].bCurrent |= BUTTON_TO_FLAG(BUTTON_DPAD_UP);
						STATE[gamepad].bCurrent &= ~BUTTON_TO_FLAG(BUTTON_DPAD_DOWN);
					} else if (je.value == 32767) {
						STATE[gamepad].bCurrent |= BUTTON_TO_FLAG(BUTTON_DPAD_DOWN);
						STATE[gamepad].bCurrent &= ~BUTTON_TO_FLAG(BUTTON_DPAD_UP);
					} else {
						STATE[gamepad].bCurrent &= ~BUTTON_TO_FLAG(BUTTON_DPAD_UP) & ~BUTTON_TO_FLAG(BUTTON_DPAD_DOWN);
					}
					break;
				default: break;
				}

				break;
			default:
				break;
			}
		}
	}
}

void GamepadShutdown(void) {
	int i;

	/* cleanup udev */
	udev_monitor_unref(MON);
	udev_unref(UDEV);

	/* cleanup devices */
	for (i = 0; i != GAMEPAD_COUNT; ++i) {
		if (STATE[i].device != NULL) {
			free(STATE[i].device);
		}

		if (STATE[i].fd != -1) {
			close(STATE[i].fd);
		}
	}
}

void GamepadSetRumble(GAMEPAD_DEVICE gamepad, float left, float right) {
	if (STATE[gamepad].fd != -1) {
		struct input_event play;

		/* delete any existing effect */
		if (STATE[gamepad].effect != -1) {
			/* stop the effect */
			play.type = EV_FF;
			play.code = STATE[gamepad].effect;
			play.value = 0;

			write(STATE[gamepad].fd, (const void*)&play, sizeof(play));

			/* delete the effect */
			ioctl(STATE[gamepad].fd, EVIOCRMFF, STATE[gamepad].effect);
		}

		/* if rumble parameters are non-zero, start the new effect */
		if (left != 0.f || right != 0.f) {
			struct ff_effect ff;

			/* define an effect for this rumble setting */
			ff.type = FF_RUMBLE;
			ff.id = -1;
			ff.u.rumble.strong_magnitude = (unsigned short)(left * 65535);
			ff.u.rumble.weak_magnitude = (unsigned short)(right * 65535);
			ff.replay.length = 5;
			ff.replay.delay = 0;

			/* upload the effect */
			if (ioctl(STATE[gamepad].fd, EVIOCSFF, &ff) != -1) {
				STATE[gamepad].effect = ff.id;
			}

			/* play the effect */
			play.type = EV_FF;
			play.code = STATE[gamepad].effect;
			play.value = 1;

			write(STATE[gamepad].fd, (const void*)&play, sizeof(play));
		}
	}
}

#else /* !defined(_WIN32) && !defined(__linux__) */

#	error "Unknown platform in gamepad.c"

#endif /* end of platform implementations */

GAMEPAD_BOOL GamepadIsConnected(GAMEPAD_DEVICE device) {
	return (STATE[device].flags & FLAG_CONNECTED) != 0 ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

GAMEPAD_BOOL GamepadButtonDown(GAMEPAD_DEVICE device, GAMEPAD_BUTTON button) {
	return (STATE[device].bCurrent & BUTTON_TO_FLAG(button)) != 0 ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

GAMEPAD_BOOL GamepadButtonTriggered(GAMEPAD_DEVICE device, GAMEPAD_BUTTON button) {
	return ((STATE[device].bLast & BUTTON_TO_FLAG(button)) == 0 &&
			(STATE[device].bCurrent & BUTTON_TO_FLAG(button)) != 0) ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

GAMEPAD_BOOL GamepadButtonReleased(GAMEPAD_DEVICE device, GAMEPAD_BUTTON button) {
	return ((STATE[device].bCurrent & BUTTON_TO_FLAG(button)) == 0 &&
			(STATE[device].bLast & BUTTON_TO_FLAG(button)) != 0) ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

int GamepadTriggerValue(GAMEPAD_DEVICE device, GAMEPAD_TRIGGER trigger) {
	return STATE[device].trigger[trigger].value;
}

float GamepadTriggerLength(GAMEPAD_DEVICE device, GAMEPAD_TRIGGER trigger) {
	return STATE[device].trigger[trigger].length;
}

GAMEPAD_BOOL GamepadTriggerDown(GAMEPAD_DEVICE device, GAMEPAD_TRIGGER trigger) {
	return STATE[device].trigger[trigger].pressedCurrent;
}

GAMEPAD_BOOL GamepadTriggerTriggered(GAMEPAD_DEVICE device, GAMEPAD_TRIGGER trigger) {
	return (STATE[device].trigger[trigger].pressedCurrent &&
			!STATE[device].trigger[trigger].pressedLast) ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

GAMEPAD_BOOL GamepadTriggerReleased(GAMEPAD_DEVICE device, GAMEPAD_TRIGGER trigger) {
	return (!STATE[device].trigger[trigger].pressedCurrent &&
			STATE[device].trigger[trigger].pressedLast) ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

void GamepadStickXY(GAMEPAD_DEVICE device, GAMEPAD_STICK stick, int *outX, int *outY) {
	*outX = STATE[device].stick[stick].x;
	*outY = STATE[device].stick[stick].y;
}

float GamepadStickLength(GAMEPAD_DEVICE device, GAMEPAD_STICK stick) {
	return STATE[device].stick[stick].length;
}

void GamepadStickNormXY(GAMEPAD_DEVICE device, GAMEPAD_STICK stick, float *outX, float *outY) {
	*outX = STATE[device].stick[stick].nx;
	*outY = STATE[device].stick[stick].ny;
}

float GamepadStickAngle(GAMEPAD_DEVICE device, GAMEPAD_STICK stick) {
	return STATE[device].stick[stick].angle;
}

GAMEPAD_STICKDIR GamepadStickDir(GAMEPAD_DEVICE device, GAMEPAD_STICK stick) {
	return STATE[device].stick[stick].dirCurrent;
}

GAMEPAD_BOOL GamepadStickDirTriggered(GAMEPAD_DEVICE device, GAMEPAD_STICK stick, GAMEPAD_STICKDIR dir) {
	return (STATE[device].stick[stick].dirCurrent == dir &&
			STATE[device].stick[stick].dirCurrent != STATE[device].stick[stick].dirLast) ? GAMEPAD_TRUE : GAMEPAD_FALSE;
}

/* initialize common gamepad state */
static void GamepadResetState(GAMEPAD_DEVICE gamepad) {
	memset(STATE[gamepad].stick, 0, sizeof(STATE[gamepad].stick));
	memset(STATE[gamepad].trigger, 0, sizeof(STATE[gamepad].trigger));
	STATE[gamepad].bLast = STATE[gamepad].bCurrent = 0;
}

/* Update individual sticks */
static void GamepadUpdateCommon(void) {
	int i;
	for (i = 0; i != GAMEPAD_COUNT; ++i) {
		/* store previous button state */
		STATE[i].bLast = STATE[i].bCurrent;

		/* per-platform update routines */
		GamepadUpdateDevice((GAMEPAD_DEVICE)i);

		/* calculate refined stick and trigger values */
		if ((STATE[i].flags & FLAG_CONNECTED) != 0) {
			GamepadUpdateStick(&STATE[i].stick[STICK_LEFT], GAMEPAD_DEADZONE_LEFT_STICK);
			GamepadUpdateStick(&STATE[i].stick[STICK_RIGHT], GAMEPAD_DEADZONE_RIGHT_STICK);

			GamepadUpdateTrigger(&STATE[i].trigger[TRIGGER_LEFT]);
			GamepadUpdateTrigger(&STATE[i].trigger[TRIGGER_RIGHT]);
		}
	}
}

/* Update stick info */
static void GamepadUpdateStick(GAMEPAD_AXIS* axis, float deadzone) {
	// determine magnitude of stick
	axis->length = sqrtf((float)(axis->x*axis->x) + (float)(axis->y*axis->y));

	if (axis->length > deadzone) {
		// clamp length to maximum value
		if (axis->length > 32767.0f) {
			axis->length = 32767.0f;
		}

		// normalized X and Y values
		axis->nx = axis->x / axis->length;
		axis->ny = axis->y / axis->length;

		//fix special case
		if (axis->nx < -1.0) axis->nx = -1.0;
		if (axis->ny < -1.0) axis->ny = -1.0;

		// adjust length for deadzone and find normalized length
		axis->length -= deadzone;
		axis->length /= (32767.0f - deadzone);

		// find angle of stick in radians
		axis->angle = atan2f((float)axis->y, (float)axis->x);
	} else {
		axis->x = axis->y = 0;
		axis->nx = axis->ny = 0.0f;
		axis->length = axis->angle = 0.0f;
	}

	/* update the stick direction */
	axis->dirLast = axis->dirCurrent;
	axis->dirCurrent = STICKDIR_CENTER;

	/* check direction to see if it's non-centered */
	if (axis->length != 0.f) {
		if (axis->angle >= PI_1_4 && axis->angle < PI_3_4) {
			axis->dirCurrent = STICKDIR_UP;
		} else if (axis->angle >= -PI_3_4 && axis->angle < -PI_1_4) {
			axis->dirCurrent = STICKDIR_DOWN;
		} else if (axis->angle >= PI_3_4 || axis->angle < -PI_3_4) {
			axis->dirCurrent = STICKDIR_LEFT;
		} else /* if (axis->angle < PI_1_4 && axis->angle >= -PI_1_4) */ {
			axis->dirCurrent = STICKDIR_RIGHT;
		}
	}
}

/* Update trigger info */
static void GamepadUpdateTrigger(GAMEPAD_TRIGINFO* trig) {
	trig->pressedLast = trig->pressedCurrent;

	if (trig->value > GAMEPAD_DEADZONE_TRIGGER) {
		trig->length = ((trig->value - GAMEPAD_DEADZONE_TRIGGER) / (255.0f - GAMEPAD_DEADZONE_TRIGGER));
		trig->pressedCurrent = GAMEPAD_TRUE;
	} else {
		trig->value = 0;
		trig->length = 0.0f;
		trig->pressedCurrent = GAMEPAD_FALSE;
	}
}
