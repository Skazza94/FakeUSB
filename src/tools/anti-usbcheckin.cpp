/*
 * This file is part of USBProxy.
 */

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <memory.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/utsname.h>

#include <iostream>

#include "TRACE.h"
#include "Manager.h"
#include "ConfigParser.h"
#include "version.h"

static unsigned debug=0;

Manager* manager;

/*
 * sigterm: stop forwarding threads, and/or hotplug loop and exit
 * sighup: reset forwarding threads, reset device and gadget
 */
void handle_signal(int signum)
{
	struct sigaction action;
	switch (signum) {
		case SIGTERM:
		case SIGINT:
			if(signum == SIGTERM)
				fprintf(stderr, "Received SIGTERM, stopping relaying...\n");
			else
				fprintf(stderr, "Received SIGINT, stopping relaying...\n");
			if (manager) {manager->stop_relaying();}
			fprintf(stderr, "Exiting\n");
			memset(&action, 0, sizeof(struct sigaction));
			action.sa_handler = SIG_DFL;
			sigaction(SIGTERM, &action, NULL);
			break;
		case SIGHUP:
			fprintf(stderr, "Received SIGHUP, restarting relaying...\n");
			if (manager) {manager->stop_relaying();}
			if (manager) {manager->start_control_relaying();}
			break;
	}
}

extern "C" int main(int argc, char **argv)
{
	int opt;

	struct sigaction action;
	memset(&action, 0, sizeof(struct sigaction));
	action.sa_handler = handle_signal;
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGHUP, &action, NULL);
	sigaction(SIGINT, &action, NULL);
	
	ConfigParser *cfg = new ConfigParser();

	while ((opt = getopt (argc, argv, "v:p:P:D:H:dsc:C:lmik::w:hx")) != EOF) {
		switch (opt) {
		case 'd':		/* verbose */
			debug++;
			cfg->debugLevel = debug;
			break;
		}
	}

	if (debug) {
		std::cerr << "Version " VERSION << '\n';
		struct utsname sysinfo;
		uname(&sysinfo);
		std::cerr << "Running under kernel "<< sysinfo.release << '\n';
	}

	cfg->set("DeviceProxy", "DeviceProxy_Emulation");
	cfg->set("HostProxy", "HostProxy_GadgetFS");

	int status;
	do {
		manager=new Manager(debug);
		manager->load_plugins(cfg);
		cfg->print_config();

		manager->start_control_relaying();
		while ( ( status = manager->get_status()) == USBM_RELAYING) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		manager->stop_relaying();
		manager->cleanup();
		delete(manager);
	} while ( status == USBM_RESET);

	printf("done\n");
	return 0;
}
