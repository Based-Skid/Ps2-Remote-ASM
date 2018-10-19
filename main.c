// All Includes are in Main.h, additional C Files should include main.h
#include "main.h" 
// App Strings
#include "strings.h"

void menu_Text(void)
{
	scr_clear();
	scr_printf(appName);
	scr_printf(appVer);
	printf(appAuthor);
	printf(appNotice);
	scr_printf(txtcrossBtn);
	//scr_printf(txtsqrBtn);
	scr_printf(" \n");
}

void initialize(void)
{

	int ret;

	SifInitRpc(0);
	// init debug screen
	init_scr();
	scr_printf("Loading... Please Wait. \n");
	// load all modules
	LoadModules();
	

	// init pad
	padInit(0);
	if ((ret = padPortOpen(0, 0, padBuf)) == 0)
	{
		#if defined DEBUG
			scr_printf("padOpenPort failed: %d\n", ret);
		#endif
		SleepThread();
	}

	if (!initializePad(0, 0))
	{
		#if defined DEBUG
			scr_printf("pad initalization failed!\n");
		#endif
		SleepThread();

	}
}

int LoadIRX()
{
	int a;
	printf(" Loading IRX!\n");

	a = SifExecModuleBuffer(&poweroff, size_poweroff, 0, NULL, NULL);
	if (a < 0 )
	{
    scr_printf(" Could not load POWEROFF.IRX! %d\n", a);
	return -1;
	}

	printf(" Loaded POWEROFF.IRX!\n");
	return 0;
	


}

void LoadModules(void)
{
	int ret;
	
	
	ret = SifExecModuleBuffer(&freesio2, size_freesio2, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load freesio2 sw module");
		ret = SifLoadModule("rom0:XSIO2MAN", 0, NULL);
		if (ret < 0) 
		{
			gotoOSDSYS(1);
		}
	}	
	

	ret = SifExecModuleBuffer(&iomanX, size_iomanX, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load iomanx sw module");

	}
	
	ret = SifExecModuleBuffer(&fileXio, size_fileXio, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load freesio2 sw module");

	}
	
	
	ret = SifExecModuleBuffer(&freepad, size_freepad, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load freepad sw module");
		ret = SifLoadModule("rom0:XPADMAN", 0, NULL);
		if (ret < 0) 
		{
			gotoOSDSYS(3);
		}
	}
	
	ret = SifExecModuleBuffer(&mcman, size_mcman, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load mcman sw module");
		ret = SifLoadModule("rom0:XMCMAN", 0, NULL);
		if (ret < 0) 
		{
			gotoOSDSYS(4);
		}
	}
	
	ret = SifExecModuleBuffer(&mcserv, size_mcserv, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf("Failed to Load mcserv sw module");
		ret = SifLoadModule("rom0:XMCSERV", 0, NULL);
		if (ret < 0) 
		{
			gotoOSDSYS(5);
		}
	}
	
	ret = SifExecModuleBuffer(&ps2dev9, size_ps2dev9, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf(" Could not load ps2dev9.IRX! %d\n", ret);
		SleepThread();
	}
	

	ret = SifExecModuleBuffer(&netman, size_netman, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf(" Could not load netman.IRX! %d\n", ret);
		SleepThread();
	}

	ret = SifExecModuleBuffer(&smap, size_smap, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf(" Could not load smap.IRX! %d\n", ret);
		SleepThread();
	}
	
	ret = SifExecModuleBuffer(&ps2ipnm, size_ps2ipnm, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf(" Could not load ps2ip.IRX! %d\n", ret);
		SleepThread();
	}
	
	ret = SifExecModuleBuffer(&ps2ips, size_ps2ips, 0, NULL, NULL);
	if (ret < 0) 
	{
		printf(" Could not load ps2ips.IRX! %d\n", ret);
		SleepThread();
	}
	ps2ip_init();
	
	ret = SifExecModuleBuffer(&ps2http, size_ps2http, 0, NULL, NULL);
	if (ret < 0)
	{
        scr_printf("	Could not load ps2http.IRX! %d\n", ret);
	return -1;
	}
 
	}
	


/////////////////////////////////////////////////////////////////////
//waitPadReady
/////////////////////////////////////////////////////////////////////
static int waitPadReady(int port, int slot)
{
	int state;
	int lastState;
	char stateString[16];

	state = padGetState(port, slot);
	lastState = -1;
	while ((state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
		if (state != lastState) {
			padStateInt2String(state, stateString);
		}
		lastState = state;
		state = padGetState(port, slot);
	}
	// Were the pad ever 'out of sync'?
	if (lastState != -1) {

	}
	return 0;
}



/////////////////////////////////////////////////////////////////////
//initalizePad
/////////////////////////////////////////////////////////////////////
static int initializePad(int port, int slot)
{

	int ret;
	int modes;
	int i;

	waitPadReady(port, slot);
	modes = padInfoMode(port, slot, PAD_MODETABLE, -1);
	if (modes > 0) {
		for (i = 0; i < modes; i++) {
		}

	}
	if (modes == 0) {
		return 1;
	}

	i = 0;
	do {
		if (padInfoMode(port, slot, PAD_MODETABLE, i) == PAD_TYPE_DUALSHOCK)
			break;
		i++;
	} while (i < modes);
	if (i >= modes) {
		return 1;
	}

	ret = padInfoMode(port, slot, PAD_MODECUREXID, 0);
	if (ret == 0) {
		return 1;
	}
	padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_LOCK);

	waitPadReady(port, slot);
	padInfoPressMode(port, slot);

	waitPadReady(port, slot);
	padEnterPressMode(port, slot);

	waitPadReady(port, slot);
	actuators = padInfoAct(port, slot, -1, 0);

	if (actuators != 0) {
		actAlign[0] = 0;
		actAlign[1] = 1;
		actAlign[2] = 0xff;
		actAlign[3] = 0xff;
		actAlign[4] = 0xff;
		actAlign[5] = 0xff;

		waitPadReady(port, slot);

		padSetActAlign(port, slot, actAlign);
	}
	else {
	}
	return 1;
}

/////////////////////////////////////////////////////////////////////
//buttonStatts
/////////////////////////////////////////////////////////////////////
static void buttonStatts(int port, int slot)
{
	int ret;
	ret = padRead(port, slot, &buttons);

	if (ret != 0) {
		paddata = 0xffff ^ buttons.btns;

		new_pad = paddata & ~old_pad;
		old_pad = paddata;
	}
}

/////////////////////////////////////////////////////////////////////
//checkPadConnected
/////////////////////////////////////////////////////////////////////
void checkPadConnected(void)
{
	int ret, i;
	ret = padGetState(0, 0);
	while ((ret != PAD_STATE_STABLE) && (ret != PAD_STATE_FINDCTP1)) {
		if (ret == PAD_STATE_DISCONN) {
			#if defined DEBUG
				scr_printf("Controller(%d, %d) is disconnected\n", 0, 0);
			#endif
		}
		ret = padGetState(0, 0);
	}
	if (i == 1) {
	}
}

/////////////////////////////////////////////////////////////////////
//pad_wat_button
/////////////////////////////////////////////////////////////////////
void pad_wait_button(u32 button)
{
	while (1)
	{
		buttonStatts(0, 0);
		if (new_pad & button) return;
	}
}

void ResetIOP()
{
	// Thanks To SP193 For Clarifying This
	SifInitRpc(0);           //Initialize SIFRPC and SIFCMD. Although seemingly unimportant, this will update the addresses on the EE, which can prevent a crash from happening around the IOP reboot.
	SifIopReset("", 0);      //Reboot IOP with default modules (empty command line)
	while(!SifIopSync()){}   //Wait for IOP to finish rebooting.
	SifInitRpc(0);           //Initialize SIFRPC and SIFCMD.
	SifLoadFileInit();       //Initialize LOADFILE RPC.
	fioInit();               //Initialize FILEIO RPC.
	
	// SBV Patches Are Not part of a Normal IOP Reset.
	sbv_patch_enable_lmb(); //SBV Patches
	sbv_patch_disable_prefix_check(); //SBV Patch Load Executable IRX And ELF Files From User-Writable Storage
	sbv_patch_user_mem_clear(0x00100000); // You Can Specify a Starting Address for the Wipe
	//sbv_patch_user_mem_clear(0x02000000); // Disable Clear Memory With LoadExecPS2() when 0x02000000 is passed as an arg
}


void gotoOSDSYS(int sc)
{
	if (sc != 0)
	{
		scr_printf(appFail);
		if(sc ==1 || sc ==2 || sc ==3 || sc ==4 || sc ==5)
		{
			scr_printf(modloadfail);
		}
		if (sc == 1)
		{
			scr_printf("SIO2MAN\n");
		}
		if (sc == 2)
		{
			scr_printf("CDVDMAN\n");
		}
		if (sc == 3)
		{
			scr_printf("PADMAN\n");
		}
		if (sc == 4)
		{
			scr_printf("MCMAN\n");
		}
		if (sc == 5)
		{
			scr_printf("MCSERV\n");
		}
		if (sc == 6)
		{
			scr_printf("ERROR: Unknown\n");
		}
		sleep(5);
	}
	ResetIOP();
	scr_printf(osdmsg);
	LoadExecPS2("rom0:OSDSYS", 0, NULL);
}


void loadGame()
{
	char *largs[3];
	largs[1] = NULL;
	largs[2] = NULL;
	largs[3] = NULL;
	LoadExecPS2("cdrom0:\\SCUS_XXX.XX;1", 3, largs);
}


int downloadfile(char *url)
{
     int fd, size, result;
     void *buffer;
	
	fd = open(url, O_RDONLY);	
     
     if (fd >= 0)
     {
           size = lseek(fd, 0, SEEK_END);    //Get size of file
           lseek(fd, 0, SEEK_SET);

           if((buffer = malloc(size)) != NULL)
           {
               result = read(fd, buffer, size) == size ? 0 : -EIO; //Check that the whole file was read.

               if(result != 0)
                 free(buffer);
           } else
               result = -ENOMEM;

           close(fd);
           
           // apply update to memory
           apply_Update(buffer);
           return 0;
           
     } else
        result = fd;
        return -1;

}

int apply_Update(unsigned int *buffer[])
{
    DI();
    ee_kmode_enter();


    // address variable
    unsigned int *address = 0;    
    // data variable
    unsigned int *data = 0;
    // buffer position variable
    int b = 0;

    // Version print
    scr_printf("\nVersion: ");
    scr_printf(&buffer[0]); // this will read until the last byte = 00
    scr_printf(" found, Applying update...\n "); // example: "Version: 59 found. Applying update..."

    // set buffer position to start of patch
    b += 2; // 8 bytes total. Each array element in an int is 4 bytes.
    
    while(buffer[b] != 0x00000000) // check if array element is all zeros, this will ALWAYS check the address, not the data.
    {            
        // decrypt address;
        address = &buffer[b];
        // increment buffer position
        b++;
        // decrypt data
        data = &buffer[b];
        // increment buffer position
        b++;                
        // copy data to address
        // check if addrss is too low
        if(&address >= 0x000820000)
        {
            // write to memory
            memcpy(address, &data, 4);
        }
        else
        {
            // something went wrong
            scr_printf("\n\nUpdate failed, restart PS2.");
            // force loop to stop
            break;
        }
    }    
    ee_kmode_exit();
    EI();
}

void endRasm(void)
{
	padPortClose(0, 0);
	padEnd();
	NetManDeinit();
	sleep(3);
	scr_printf(terrorist);
	scr_clear();
	SifExitRpc();
	//Load Game
	loadGame();
}

int main(int argc, char *argv[]) 
{

	ResetIOP();
	
	// initialize
	initialize();
	// "Load IRX Modules"
	LoadIRX();
	sleep(1);
	//scr_printf("Modules Loaded Up. Starting up DHCP \n");
	dhcpmain();
	menu_Text();
	//motd();
	//motdTap();



	while (1)
	{
		//check to see if the pad is still connected
		checkPadConnected();
		//read pad 0
		buttonStatts(0, 0);

		// if (new_pad & PAD_TRIANGLE)
		// {
			// menu_Text();
		// }
		
		if(new_pad & PAD_CROSS)
		{
			int dlf,dlt,dlbk;
			scr_printf("Applying Patch From the Internet....");
			dlf = downloadfile(mainDownload);
			if (dlf < 0) 
			{
				
				
				scr_printf("Failed \nReverting to Offline Version \n");
				PasteASM();
				endRasm();

				

			}
			else
			{
				printf("\nPatch Loaded Successfully.\n\0");
				endRasm();
			}
		}
	}
	return 0;
}
