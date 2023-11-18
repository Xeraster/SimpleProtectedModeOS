//returns true if success, false if timed out
bool keyboardWaitReadReady()
{
    bool ready = false;
    while (!ready)
    {
        //get status byte from 8042
        //forceIOPL_High();
        char status = asmInb(0x64);
        //printCharAdr('K', 0x0E, 1790);

        //we only care about the bit in position 0
        status = status & 0x01;
        //printCharAdr(status, 0x0D, 1793)
        //print8bitNumber((unsigned int)status, 1795);

        if (status == 1) ready = true;
    }

    //printCharAdr('b', 0x0E, 1791);

    return ready;
}

//returns true if success, false if timed out
bool keyboardWaitWriteReady()
{
    bool ready = false;
    while(!ready)
    {
        //get status byte from 8042
        //forceIOPL_High();
        char status = asmInb(0x64);

        //we only care about the bit in position 1, the write ready bit
        status = status & 0x02;

        //bit 2 has to be 0 for it to be ready for a write
        if (status == 0) ready = true;
    }

    return ready;
}

//waits for acknoledge signal from keyboard.
//returns true is success, false if timed out
bool keyboardWaitAck()
{
    //wait for keyboard to be ready before requesting 
    keyboardWaitReadReady();

    return true;
}

char waitChar()
{
    char kdbData = 0;
    bool lastCodeE0 = false;
    //this HAS to be cast to an unsigned int for some reason.
    //pressing caps lock and other special keyboard buttons outputs garbage on the screen if this isn't unsigned int
    while ((unsigned int)kdbData > 87 || kdbData == 0)
    {
        *(char*)0xB8936 = 'W';
		*(char*)0xB8937 = 0x04;//red
        //wait for the keyboard to be ready
        //keyboardWaitReadReady();
        kbd8042WaitReadReady(); //use this instead maybe it will work on physical pcs and not just emulators (sppiler alert: nope)
        *(char *)0xB8936 = 'W';
        *(char*)0xB8937 = 0x0E;//yellow
        //once the keyboard is ready, read a byte from the data port
        //kdbData = asmInb(0x60);
        kdbData = inb(0x60);
        *(char *)0xB8936 = 'W';
        *(char*)0xB8937 = 0x02;//green

        //now see what the scancode is
        if (manageSpecialKeyboardFunctions(kdbData, lastCodeE0))
        {
            kdbData = 0;
        }

        *(char*)0xB8938 = 'D';
		*(char*)0xB8939 = 0x0E;

        //keyboardWaitReadReady();
        //forceIOPL_High();
        //kbd8042WaitReadReady(); //use this instead maybe it will work on physical pcs and not just emulators
        //kdbData = inb(0x60);//this fucks everything up dont do it
        //kdbData = asmInb(0x60);
        
        *(char*)0xB8938 = 'D';
		*(char*)0xB8939 = 0x0D;

        
        //for some reaon I'm not getting the E0 code
        //i dont remember what this does but 06/02/23 i changed it to unsigned int because i486 g++ likes it when you do that 
        if (kdbData == 0xE0)
        {
            lastCodeE0 = true;
            //print8bitNumber(kdbData, 1803);
        }
        else lastCodeE0 = false;

        *(char*)0xB893A = 'E';
		*(char*)0xB893B = 0x0D;
        
    }

    //return the scancode
    return scancodeToChar(kdbData, 0);
    //return 'F';
   // return kdbData;

}

//deals with stuff related to special keyboard functions (i.e. caps lock or shift)
bool manageSpecialKeyboardFunctions(char function, bool lastKeyAlt)
{
    //left arrow key
    if (function == 0x4B)
    {
        shiftCursor(-1);
        return true;
    }

    //right arrow key
    else if (function == 0x4D)
    {
        shiftCursor(1);
        return true;
    }

    //caps lock key
    else if (function == 0x3A)
    {
        capsLock = !capsLock;
        setKeyboardLeds(capsLock, numLock, scrollLock);
        //keyboardWaitWriteReady();
        return true;
    }

    //num lock key
    else if (function == 0x45)
    {
        numLock = !numLock;
        setKeyboardLeds(capsLock, numLock, scrollLock);
        return true;
    }

    //scroll lock
    else if (function == 0x46)
    {
        scrollLock = !scrollLock;
        setKeyboardLeds(capsLock, numLock, scrollLock);
        return true;
    }

    //backspace
    else if (function == 0x0E)
    {
        backspaceScreen();
        return false;
    }

    //delete key
    //if delete key pressed and num lock = true
    else if (function == 0x53 && numLock)
    {
        backspaceScreen();
        return true;
    }
    
    return false;
}

/*converts inputted scancode into a character
0 = original XT scancode set
1 = PS/2 scan code set for AT motherboards
2 = default scan code set for "modern" keyboards
*/
char scancodeToChar(char scancode, unsigned int scancodeSet)
{
    if (capsLock)
    {
        return scancodesXT_uppercase.at(scancode);
    }
    else
    {
        return scancodesXT_lowercase.at(scancode);
    }
}

//anything other than xt scancode set isn't supported right now. Note that HT6542B ps/2 controllers default to "modern" keyboard scancode set but others default to xt
void setupScancodeMap()
{
    *(char*)0xB8018 = 'K';
	*(char*)0xB8019 = 0x0E;

    scancodesXT_lowercase="**";//escape key
    *(char*)0xB801A = 'K';
	*(char*)0xB801B = 0x0E;

    scancodesXT_lowercase+="1234567890-=";
    *(char*)0xB801C = 'K';
	*(char*)0xB801D = 0x0E;
    scancodesXT_lowercase+=0x08;//backspace
    scancodesXT_lowercase+="*";//tab
    scancodesXT_lowercase+="qwertyuiop[]";
    scancodesXT_lowercase+=char(10);//enter key
    *(char*)0xB801E = 'K';
	*(char*)0xB801F = 0x0E;
    scancodesXT_lowercase+="*";//L CTRL key
    scancodesXT_lowercase+="asdfghjkl;'`";
    scancodesXT_lowercase+="*";//L SHIFT
    scancodesXT_lowercase+=char(92);
    scancodesXT_lowercase+="zxcvbnm,./";
    scancodesXT_lowercase+="*";//R SHIFT
    scancodesXT_lowercase+="*";//KP *
    scancodesXT_lowercase+="*";//L ALT
    scancodesXT_lowercase+=" ";//space
    scancodesXT_lowercase+="*";//CAPS
    scancodesXT_lowercase+="**********";//F1-F10
    scancodesXT_lowercase+="*";//num lock
    scancodesXT_lowercase+="*";//scroll lock
    scancodesXT_lowercase+="789-456+1230.";//scroll lock
    scancodesXT_lowercase+="**";//F11 and F12
    //that's it for the non E0 scancodes

    *(char*)0xB8020 = 'r';
	*(char*)0xB8021 = 0x0E;

    //now for the upper case
    scancodesXT_uppercase="**";//escape key
    scancodesXT_uppercase+="!@#$%^&*()_+";
    scancodesXT_uppercase+=0x08;//backspace
    scancodesXT_uppercase+="*";//tab
    *(char*)0xB8022 = 'A';
	*(char*)0xB8023 = 0x0E;
    scancodesXT_uppercase+="QWERTYUIOP{}";
    scancodesXT_uppercase+=char(10);//enter key
    scancodesXT_uppercase+="*";//L CTRL key
    scancodesXT_uppercase+="ASDFGHJKL:\"~";
    *(char*)0xB8022 = '2';
	*(char*)0xB8023 = 0x0E;
    *(char*)0xB8022 = 'B';
	*(char*)0xB8023 = 0x0E;
    scancodesXT_uppercase+="*";//L SHIFT
    scancodesXT_uppercase+=char(92);
    scancodesXT_uppercase+="ZXCVBNM<>?";
    scancodesXT_uppercase+="*";//R SHIFT
    *(char*)0xB8022 = 'C';
	*(char*)0xB8023 = 0x0E;
    scancodesXT_uppercase+="*";//KP *
    scancodesXT_uppercase+="*";//L ALT
    scancodesXT_uppercase+=" ";//space
    *(char*)0xB8022 = 'D';
	*(char*)0xB8023 = 0x0E;
    scancodesXT_uppercase+="*";//CAPS
    scancodesXT_uppercase+="**********";//F1-F10
    scancodesXT_uppercase+="*";//num lock
    *(char*)0xB8022 = 'E';
	*(char*)0xB8023 = 0x0E;
    scancodesXT_uppercase+="*";//scroll lock
    scancodesXT_uppercase+="789-456+1230.";//scroll lock
    *(char*)0xB8022 = 'F';
	*(char*)0xB8023 = 0x0E;
    scancodesXT_uppercase+="**";//F11 and F12
    //that's it for the non E0 scancodes

    *(char*)0xB8022 = 'K';
	*(char*)0xB8023 = 0x0E;

    return;
}

//sets the keyboard status lights to whatever you want them to be
void setKeyboardLeds(bool caps, bool num, bool scroll)
{
    char lightByte = 0x00;
    //if caps lock should be enabled, set bit 2
    if (caps) lightByte = lightByte|4;
    else lightByte = lightByte & 251;

    //if num lock should be enabled, set bit 1
    if (num) lightByte = lightByte|2;
    else lightByte = lightByte & 253;

    //if scroll lock should be enabled, set bit 0
    if (scroll) lightByte = lightByte|1;
    else lightByte = lightByte & 254;

    //printInt(lightByte, 0x0E, true);

    //wait for keyboard to be ready for a write
    keyboardWaitWriteReady();
    //kbd8042WaitWriteReady();

    //send command 0xED to indicate the next data byte is going to be for the keyboard status lights
    //forceIOPL_High();
    outb(0xED, 0x60);

    //wait for keyboard to be ready for another write
    keyboardWaitWriteReady();
    //kbd8042WaitWriteReady();

    //send the byte
    outb(lightByte, 0x60);

    //done

    return;
}
